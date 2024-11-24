#include "tasklist.h"
#include <algorithm>

TaskList::TaskList(QWidget *parent, Sortmethod sortmethod)
    : QWidget{parent}, sortmethod(sortmethod), taskpos(0)//初始没有任务
{tasklist.clear();}

TaskList::~TaskList()
{
    //退出的时候保存tasklist到本地
    saveToJson("tasklist.json");
    for(Task* task : tasklist) delete task;
    qDebug()<<"TaskList deleted\n";
}

void TaskList::addTask(Task* task)
{
    //taskpos++;//编号加一
    tasklist.append(task);
}

void TaskList::removeTask(int id)
{
    for(int i = 0; i < tasklist.size(); ++i){
        if(tasklist[i]->getid() == id) {
            delete tasklist[i];
            tasklist.remove(i);
            return;
        }
    }
    qDebug()<<"Task with ID "<< id <<" not found!";
}



bool TaskList::compareBySetDate(Task* a, Task* b)
{
    if(a->get_settingdate() != b->get_settingdate())
        return a->get_settingdate() < b->get_settingdate();
    return a->getid() < b->getid();
}

bool TaskList::compareByDdl(Task* a, Task* b)
{
    if(a->get_ddldate() != b->get_ddldate())
        return a->get_ddldate() < b->get_ddldate();
    if(a->get_ddltime() != b->get_ddltime())
        return a->get_ddltime() < b->get_ddltime();
    return a->getid() < b->getid();
}

bool TaskList::compareByAttribute(Task*a ,Task* b)
{
    if(a->get_task_attribute().get_priority() != b->get_task_attribute().get_priority())
        return a->get_task_attribute().get_priority() < b->get_task_attribute().get_priority();
    return a->getid() < b->getid();
}

void TaskList::set_sortmethod(Sortmethod sortmethod)
{
    this->sortmethod = sortmethod;
}

void TaskList::sortlist()
{
    switch(sortmethod){
    case Setdate:
        std::sort(tasklist.begin(), tasklist.end(), compareBySetDate);
        break;
    case Ddl:
        std::sort(tasklist.begin(), tasklist.end(), compareByDdl);
        break;
    case Attribute:
        std::sort(tasklist.begin(), tasklist.end(), compareByAttribute);
    }
}

void TaskList::loadFromJson(const QString& filepath)
{
    QString applicationDirPath = QCoreApplication::applicationDirPath();
    QString absolutePath = QDir(applicationDirPath).filePath(filepath);

    QFile file(absolutePath);
    //qDebug()<<"here\n";
    if(!file.exists()){//第一次运行程序的时候,tasklist.json文件不存在
        //创建一个空的tasklist.json文件
        QJsonDocument jsonDoc;
        QFile file2(absolutePath);
        if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing:" << absolutePath;
            return;
        }
        file2.write(jsonDoc.toJson(QJsonDocument::Indented));
        file2.close();
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Cannot open file: "<< filepath;
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    QJsonArray jsonArray = jsonDoc.array();
    file.close();

    tasklist.clear();
    for(const QJsonValue& jsonValue : jsonArray){
        if(jsonValue.isObject()){
            QJsonObject jsonObj = jsonValue.toObject();
            taskpos++;//编号加一
            Task* task = new Task(taskpos);
            task->fromJsonObject(jsonObj);
            tasklist.append(task);
        }
        else{
            qDebug()<<"Expected QJsonObject, but got:"<<jsonValue.type();
        }
    }
}

void TaskList::saveToJson(const QString& filepath)
{
    //将任务列表保存到JSON文件
    //按照设置日期排序
    //每次都是在推出时候保存到本地，之前保留的文件要覆盖掉
    
    //将任务列表保存到JSON文件
    //按照设置日期排序
    sortmethod = Setdate;
    sortlist();
    QJsonArray jsonArray;
    for(const Task* task : tasklist){
        jsonArray.append(task->toJsonObject());
    }
    QJsonDocument jsonDoc(jsonArray);

    //QString absolutePath = QCoreApplication::applicationDirPath() + "/" + filepath;
    QString applicationDirPath = QCoreApplication::applicationDirPath();
    QString absolutePath = QDir(applicationDirPath).filePath(filepath);

    // 检查原来的json文件是否存在
    QFile file(absolutePath);
    if (!file.exists()) {
        qDebug() << "File does not exist:" << absolutePath;
        return;
    }

    // 删除文件
    bool result = file.remove();
    if (!result) {
        qDebug() << "Failed to delete file:" << absolutePath;
        return;
    }
    // 保存文件
    //QFile file(filepath);

    // 创建并打开文件
    QFile file2(absolutePath);
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << absolutePath;
        return;
    }
    //写入
    file2.write(jsonDoc.toJson(QJsonDocument::Indented));
    file2.close();
}
