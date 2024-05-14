/*
 * SCNU编译原理实验二
 *实验内容：
 *设计一个应用软件，以实现将正则表达式-->NFA--->DFA-->DFA最小化-->词法分析程序
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
