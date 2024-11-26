#include "task.h"

Task::Task(int _id, QString _name, QString _description,QDate _settingdate, QDate _ddldate,\
               Kind_Tag _task_kind, Attribute_Tag _task_attribute, Taskstatus _status, bool _trigger, QWidget* _parent)
    :id(_id), name(_name), description(_description), settingdate(_settingdate), ddldate(_ddldate),\
    task_kind(_task_kind), task_attribute(_task_attribute), status(_status), trigger(_trigger)
{
    this->setParent(_parent);
    taskwindow = new QWidget(_parent);
}

Task::~Task()
{
    //delete taskwindow;
    qDebug() << "Task with id: " << id << " is deleted";
}

int Task::getid() const
{
    return id;
}

QString Task::getname() const {
    return name;
}

QString Task::getdescription() const {
    return description;
}

QDate Task::get_settingdate() const {
    return settingdate;
}

QDate Task::get_ddldate() const {
    return ddldate;
}

QTime Task::get_ddltime() const {
    return ddltime;
}

Kind_Tag Task::get_task_kind() const {
    return task_kind;
}

Attribute_Tag Task::get_task_attribute() const {
    return task_attribute;
}

Taskstatus Task::get_status() const {
    return status;
}

bool Task::get_trigger() const {
    return trigger;
}

void Task::setname(QString _name){
    name = _name;
}

void Task::setdescription(QString _description){
    description = _description;
}

void Task::set_settingdate(QDate _settingdate){
    settingdate = _settingdate;
}

void Task::set_ddldate(QDate _ddldate){
    ddldate = _ddldate;
}

void Task::set_ddltime(QTime _ddltime){
    ddltime = _ddltime;
}

void Task::set_task_kind(Kind_Tag _task_kind){
    task_kind = _task_kind;
}

void Task::set_task_attribute(Attribute_Tag _task_attribute){
    task_attribute = _task_attribute;
}

void Task::set_status(Taskstatus _status){
    status = _status;
}

void Task::set_trigger(bool _trigger){
    trigger = _trigger;
}

QJsonObject Task::toJsonObject() const {
    QJsonObject json;
    //json["id"] = id;
    json["name"] = name;
    json["description"] = description;
    json["settingdate"] = settingdate.toString("yyyy-MM-dd");
    json["ddldate"] = ddldate.toString("yyyy-MM-dd");
    json["ddltime"] = ddltime.toString("hh:mm");
    json["task_kind"] = task_kind.getname();
    switch (task_attribute.get_priority()) {
    case TaskPriority::Important:
        json["task_attribute"] = "Important";
        break;
    case TaskPriority::Normal:
        json["task_attribute"] = "Normal";
        break;
    case TaskPriority::Urgent:
        json["task_attribute"] = "Urgent";
        break;
    default:
        json["task_attribute"] = "Normal";
    }
    switch (status) {
    case Taskstatus::COMPLETED:
        json["status"] = "completed";
        break;
    case Taskstatus::INCOMPLETE:
        json["status"] = "incomplete";
        break;
    default:
        json["status"] = "incomplete";
    }
    //json["alarm"] = alarm.path();
    json["trigger"] = trigger;
    return json;
}

void Task::fromJsonObject(const QJsonObject &json)
{
    name = json["name"].toString();
    description = json["description"].toString();
    settingdate = QDate::fromString(json["settingdate"].toString(), "yyyy-MM-dd");
    ddldate = QDate::fromString(json["ddldate"].toString(), "yyyy-MM-dd");
    ddltime = QTime::fromString(json["ddltime"].toString(), "hh:mm");
    task_kind = Kind_Tag(json["task_kind"].toString());
    QString task_attribute_string = json["task_attribute"].toString();
    if(task_attribute_string == "Important"){
        task_attribute = Attribute_Tag(TaskPriority::Important);
    }
    else if(task_attribute_string == "Urgent"){
        task_attribute = Attribute_Tag(TaskPriority::Urgent);
    }
    else task_attribute = Attribute_Tag(TaskPriority::Normal);
    QString status_string = json["status"].toString();
    if(status_string == "completed") status = Taskstatus::COMPLETED;
    else status = Taskstatus::INCOMPLETE;
    //QString alarm_path = json["alarm"].toString();
    trigger = json["trigger"].toBool();
}
