#include<iostream>
#include<thread>
//Qt
#include <QApplication>
#include <QTextCodec>
#include <QIcon>
//导入自己写的头文件
#include"utils.hpp"
#include"Screen.hpp"

void thread_print(){
    helloworld_print();
    midi_print();
}


// int main(){
//     std::thread t1_p(thread_print);
//     std::thread t2_s(thread_screen);
//     t1_p.join();
//     t2_s.join();
//     std::cout << "hello world" << std::endl;
//     return 0;
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));

    #if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    #if _MSC_VER
        QTextCodec *codec = QTextCodec::codecForName("gbk");
    #else
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
    #endif
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
        QTextCodec::setCodecForTr(codec);
    #else
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        QTextCodec::setCodecForLocale(codec);
    #endif

    ScreenWidget::Instance()->showFullScreen();   //直接调用实例

    return a.exec();
} 