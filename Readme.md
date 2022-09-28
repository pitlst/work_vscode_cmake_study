# cmake_study

## 简介
这是一个本人用于学习cmake的基本模板，把我常用的一些功能在cmake中进行实现

这个项目将作为本人以后cmake组织项目的一个模板，也是学习c++的一个方式

## 项目组成

    ├─.vscode
    ├─build
    │  ├─Release
    │  └─Testing
    │      └─Temporary
    ├─include
    └─src

src存放源代码，include存放头文件，build存放编译完的文件

.vscode文件夹存放vscode的json配置文件，gitignore文件用于标记不同步更改的文件

## 部署方式

本项目依赖opencv和fmt，目前仅在windows平台上，后续会继续完事

把项目克隆下来后进入项目文件夹

>    mkdir build
>
>    cd build
>
>    cmake ..
>
>    cmake --build . 
>
>    ./Debug/cmake_study.exe