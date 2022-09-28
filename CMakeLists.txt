# 要求并设置cmake的最低版本，版本不符合要求不允许编译
# cmake从3.1开始支持c++11
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# 设置项目名
set(PROJECT_NAME cmake_study)
# 在这里直接指定项目为c++项目，节约查找tool chain的时间
project(${PROJECT_NAME} LANGUAGES CXX)
# 设置c++标准等级和编译后输出路径
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
# 强制化c++标准，当标准不符合要求时强制终止编译而不是采用更旧的标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 搜索src下文件存入DIR_SRCS变量
aux_source_directory(src DIR_SRCS)
# 搜索include下头文件存入HEADERS变量
aux_source_directory(include HEADERS)
# 指定头文件在include文件夹中寻找
INCLUDE_DIRECTORIES(include)
# 指定默认的编译等级
IF(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
ENDIF()
# 判断操作系统平台,根据平台指定opencv安装路径
MESSAGE(STATUS "目标平台是 ${CMAKE_SYSTEM}")
MESSAGE(STATUS "编译等级是 ${CMAKE_BUILD_TYPE}")
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # 由于没有设置环境变量，cmake找不到opencv的安装路径，需要手动指定OpenCVConfig.cmake的位置
    # 注意，这里的OpenCVConfig.cmake应当是指向二进制文件的
    MESSAGE(STATUS "设置opencv安装路径为:  D:/opencv/build/x64/vc15/lib")
    set(OpenCV_DIR D:/opencv/build/x64/vc15/lib)
    # 解决MSVC下的字符编码问题，使不带标签的utf-8编码的文件不发出代码页警告
    # 默认windows下使用msvc
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ELSE()
	MESSAGE(FATAL_ERROR "我们还没有适配目标平台，抱歉，请等待一段时间，非常感谢")
ENDIF()

# 寻找opencv
find_package(OpenCV REQUIRED)
# 寻找fmt输入输出库，其已经安装到系统中
# 对于fmt的安装：如果使用c++20标准则不用安装，标准库实现了fmt的大部分功能
# 如果使用c++11需要自行安装fmt，对于linux而言使用包管理系统即可，对于windows要么使用vcpkg，要么自行编译安装，编译时需要指定为release模式
# 需要在vs的界面中手动更改或者手动指定msvc的参数，不能默认编译，那样出来的fmt为debug模式的，没有控制台输出
find_package(fmt REQUIRED)
# 如果找到了opencv就开始进行编译
IF(OpenCV_FOUND)
    # 导入头文件
	include_directories(${OpenCV_INCLUDE_DIRS})
    # 生成目标可执行文件
	add_executable(${PROJECT_NAME} ${DIR_SRCS} ${HEADERS})
    # 链接动态库
	target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
    target_link_libraries(${PROJECT_NAME} fmt::fmt)#我也不知道为啥这么写
ENDIF()

# 根据不同的编译器指定警告级别
IF(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options(${PROJECT_NAME} PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn )
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /w44265 /w44061 /w44062 )
ENDIF()


# >>>>>>>>测试部分>>>>>>>>
enable_testing()
# 仅运行生成文件
add_test(NAME plain-run COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
# <<<<<<<<测试部分结束<<<<<<<<