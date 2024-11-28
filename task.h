#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QCheckBox>
//#include <QDebug>
#include "kind_tag.h"
#include "attribute_tag.h"

enum Taskstatus
{
    COMPLETED,
    INCOMPLETE
};

class Task : public QWidget
{
    Q_OBJECT
private:
    int id;//任务id。本地json文件是按照任务设置时间存储的,因此初始化后任务id顺序(从1开始)即任务设置时间顺序。
    //注意到删除任务只会让某个任务id的任务消失，但任务仍然按照设置时间顺序排序。
    QString name;//任务名
    QString description;//任务描述
    QDate settingdate;//设置的日期
    QDate ddldate;//deadline date,初始默认为明天
    QTime ddltime;//deadline time
    Kind_Tag task_kind;//任务种类
    Attribute_Tag task_attribute;//任务属性
    Taskstatus status;//任务状态(是否完成)
    //QDate completingdate;//完成日期
    //QDate autocleardate;//历史任务自动删除日期
    //Alarm alarm;//闹钟
    bool trigger;//是否要闹钟提醒
public:
    Task(int _id = 0, QString _name = "", QString _description = "", \
         QDate _settingdate = QDate::currentDate(), QDate _ddldate = QDate:: currentDate().addDays(1), QTime _ddltime = QTime(23, 59, 59), \
         Kind_Tag _task_kind = Kind_Tag("学习任务"), Attribute_Tag _task_attribute = TaskPriority::Normal, \
         Taskstatus _status = Taskstatus::INCOMPLETE, bool _trigger = false, QWidget* _parent = nullptr);
    ~Task();
    int getid() const;
    QString getname() const;
    QString getdescription() const;
    QDate get_settingdate() const;
    QDate get_ddldate() const;
    QTime get_ddltime() const;
    Kind_Tag get_task_kind() const;
    Attribute_Tag get_task_attribute() const;
    Taskstatus get_status() const;
    //QDate get_completingdate() const;
    //QDate get_autocleardate() const;
    bool get_trigger() const;
    void setname(QString _name);
    void setdescription(QString _description);
    void set_settingdate(QDate _settingdate);
    void set_ddldate(QDate _ddldate);
    void set_ddltime(QTime _ddltime);
    void set_task_kind(Kind_Tag _task_kind);
    void set_task_attribute(Attribute_Tag _task_attribute);
    void set_status(Taskstatus _status);
    //void triggeralrm();
    void set_trigger(bool _trigger);
    QJsonObject toJsonObject() const;//把Task转换为QJsonObject
    void fromJsonObject(const QJsonObject &json);//从QJsonObject获取信息
    QWidget* taskwindow;//任务窗口,而任务列表的显示(在其parent TaskList的一个实例中)则是靠taskwindow组成的。
    //void generate_taskwindow();//生成任务窗口
    //重载"="运算符
    //Task& operator=(const Task& other);
};

#endif // TASK_H
