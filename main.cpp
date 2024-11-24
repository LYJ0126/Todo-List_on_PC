#include "todolist.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TodoList w;
    w.init();
    w.initUI();

    w.show();
    //测试,目前没有GUI部分

    return a.exec();
}
