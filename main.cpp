#include "todolist.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TodoList w;
    w.init();
    w.initUI();

    w.show();
    //����,Ŀǰû��GUI����

    return a.exec();
}
