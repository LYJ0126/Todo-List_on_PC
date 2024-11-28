#ifndef TASKLIST_H
#define TASKLIST_H

#include <QWidget>
#include <QVector>
#include <QDate>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include "kind_taglist.h"
//#include <QDebug>
#include "task.h"

class TodoList;//前向声明

enum Sortmethod
{
    Setdate,
    Ddl,
    Attribute
};

class TaskList : public QWidget
{
    Q_OBJECT
private:
    QVector<Task*> tasklist;
    Sortmethod sortmethod;
public:
    TaskList(QWidget *parent = nullptr, Sortmethod sortmethod= Setdate);
    ~TaskList();
    int taskpos;//taskid编号到了哪
    Kind_TagList* kind_taglist;
    void loadFromJson(const QString& filepath);//本地读取数据到tasklist
    void saveToJson(const QString& filepath);
    void addTask(Task* task);
    void removeTask(int id);
    Task* getTask(int pos);//获取任务,这里是pos而不是id,pos是任务在tasklist中的位置
    int tasknum() const;
    void set_sortmethod(Sortmethod sortmethod);
    Sortmethod get_sortmethod() const;
    void sortlist();
    //void displayTasks() const;
    static bool compareBySetDate(Task* a, Task* b);
    static bool compareByDdl(Task* a, Task* b);
    static bool compareByAttribute(Task* a, Task* b);
    void display_add_taskwindow(QWidget* parent);//显示添加任务窗口(由TodoList中的添加任务按钮触发)
    QVector<QWidget*> tasklist_widgets;//一页显示3个任务widget,总共显示1 + tasknum()/3个页面
    //void generate_tasklist_widgets();

//signals:
};

#endif // TASKLIST_H
