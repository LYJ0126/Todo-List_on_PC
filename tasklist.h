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
//#include <QDebug>
#include "task.h"

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
    int taskpos;//记录编号到了哪
    void loadFromJson(const QString& filepath);//本地读取数据到tasklist
    void saveToJson(const QString& filepath);
    void addTask(Task* task);
    void removeTask(int id);
    void updateTaskList();
    void set_sortmethod(Sortmethod sortmethod);
    void sortlist();
    void displayTasks() const;
    static bool compareBySetDate(Task* a, Task* b);
    static bool compareByDdl(Task* a, Task* b);
    static bool compareByAttribute(Task* a, Task* b);



//signals:
};

#endif // TASKLIST_H
