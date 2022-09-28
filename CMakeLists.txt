# 要求并设置cmake的最低版本，版本不符合要求不允许编译
# 我电脑上的qt需要至少3.16的cmake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
# 指定默认的编译等级
IF(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CONFIGURATION_TYPES "Release")    #这行用于解决msbuild的编译模式不能设置为release的问题
ENDIF()
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
# 有些项目会动态生成头文件，项目中需要引入它，因此需要将output目录也include进来
# 等同于INCLUDE_DIRECTORY(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 搜索src下文件存入DIR_SRCS变量
aux_source_directory(src DIR_SRCS)
# 搜索include下头文件存入HEADERS变量
aux_source_directory(include HEADERS)
# 指定头文件在include文件夹中寻找
INCLUDE_DIRECTORIES(include)
# 判断操作系统平台,根据平台指定opencv安装路径
MESSAGE(STATUS "目标平台是 ${CMAKE_SYSTEM}")
MESSAGE(STATUS "编译等级是 ${CMAKE_BUILD_TYPE}")
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # 由于没有设置环境变量，cmake找不到opencv的安装路径，需要手动指定OpenCVConfig.cmake的位置
    # 注意，这里的OpenCVConfig.cmake应当是指向二进制文件的
    MESSAGE(STATUS "设置opencv安装路径为:  D:/opencv/build/x64/vc15/lib")
    set(OpenCV_DIR D:/opencv/build/x64/vc15/lib)
    # 手动指定QT的安装路径
    set(QT_DIR D:/QT)
    # Qt特有的编译器需要打开，默认是关闭的
    set(CMAKE_AUTOMOC ON) # Meta-Object Compiler
    set(CMAKE_AUTORCC ON) # Resource Compiler
    set(CMAKE_AUTOUIC ON) # User Interface Compiler
    # qt生成的中间文件都会在build目录下，这样的话，编译器则不能定位由uic程序产生的诸如_ui_mainwindow.h等文件。所以，我们需要把build目录添加到包含目录中
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    # 解决MSVC下的字符编码问题，使不带标签的utf-8编码的文件不发出代码页警告
    # 默认windows下使用msvc
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    # 增加编译宏，方便判断操作系统
    add_definitions(-DWIN)
    link_directories(C:\\Program Files \(x86\)\\Windows Kits\\10\\Lib\\10.0.18362.0\\um\\x64)
ELSE()
	MESSAGE(FATAL_ERROR "我们还没有适配目标平台，抱歉，请等待一段时间，非常感谢")
ENDIF()

# 寻找opencv
find_package(OpenCV REQUIRED)
# 寻找QT
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# 如果找到了相关依赖就开始进行编译
IF(OpenCV_FOUND AND QT_FOUND)
    # 导入头文件
	include_directories(${OpenCV_INCLUDE_DIRS})
    # 生成目标可执行文件
	add_executable(${PROJECT_NAME} ${DIR_SRCS} ${HEADERS})
    # 链接动态库
	target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} Qt5::Widgets)
ENDIF()

# >>>>>>>>测试部分>>>>>>>>
enable_testing()
# <<<<<<<<测试部分结束<<<<<<<<