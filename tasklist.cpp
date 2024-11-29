#include "tasklist.h"
#include "task.h"
#include <algorithm>

TaskList::TaskList(QWidget *parent, Sortmethod sortmethod)
    : QWidget{parent}, sortmethod(sortmethod), taskpos(0)//初始没有任务
{
    tasklist.clear();
    kind_taglist = new Kind_TagList();
    tasklist_widgets.clear();
}

TaskList::~TaskList()
{
    //退出的时候保存tasklist到本地
    sortmethod = Setdate;//保存的时候按照设置日期排序(默认排序方式)
    sortlist();
    saveToJson("tasklist.json");
    qDebug()<<"tasklist saved";
    qDebug()<<"tasklist deleted";
    //for(Task* task : tasklist) delete task;
    //qDebug()<<"tasklist deleted";
    delete kind_taglist;
    qDebug()<<"kind_taglist deleted";
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
            Task* tmptask = tasklist[i];
            tasklist.remove(i);
            delete tmptask;
            return;
        }
    }
    qDebug()<<"Task with ID "<< id <<" not found!";
}

Task* TaskList::getTask(int pos)
{
    return tasklist[pos];
}

int TaskList::tasknum() const
{
    return tasklist.size();
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

Sortmethod TaskList::get_sortmethod() const
{
    return sortmethod;
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
            //检查当前时间是否超过截止日期，如果超过,则不load该任务
            //这个以后会改成将过期任务放到历史任务列表里
            QDateTime now = QDateTime::currentDateTime();
            if (task->get_ddldate() < now.date() || (task->get_ddldate() == now.date() && task->get_ddltime() < now.time())) {
                //qDebug() << "Task with ID:" << task->getid() << " has expired.";
                taskpos--;//编号减一
                delete task;
                continue;
            }
            task->setParent(this);
            task->taskwindow->setParent(this);
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

/*
void TaskList::display_add_taskwindow(QWidget* parent)
{
    //parent是TodoList::displayArea
    //下方添加两个按钮，确认和取消
    //点击确认按钮，检查输入的任务设置参数是否合法，如果合法，则创建Task对象，并添加到tasklist中，关闭窗口并释放窗口占用的内存
    //然后更新tasklist显示
    //点击取消按钮，关闭窗口并释放窗口占用的内存
    QWidget* addTaskWindow = new QWidget(parent);
    //addTaskWindow->setAttribute(Qt::WA_DeleteOnClose, true);//设置窗口关闭时自动释放内存
    addTaskWindow->setGeometry(QRect(QPoint(0, 0), QSize(1020,680)));
    addTaskWindow->setStyleSheet("QWidget{background-color : #f7f7f7;}");
    Task* newtask = new Task(taskpos + 1);
    newtask->setParent(this);//这一步很关键，因为有可能用户直接退出程序导致newtask内存泄漏，
    //所以需要将newtask的父对象设置为this，这样程序退出时，顺着对象树最后会释放newtask的内存
    newtask->taskwindow->setParent(this);

    QLabel* tasktitle = new QLabel();
    tasktitle->setParent(addTaskWindow);
    tasktitle->move(60, 30);
    tasktitle->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    tasktitle->setText("任务名");
    //任务标题输入框
    QLineEdit* tasktitleEdit = new QLineEdit(addTaskWindow);
    tasktitleEdit->setGeometry(QRect(QPoint(60, 60), QSize(900, 40)));
    tasktitleEdit->setPlaceholderText("请输入任务标题...");
    //输入字体大小
    tasktitleEdit->setStyleSheet("QLineEdit{font-size: 22px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    connect(tasktitleEdit, &QLineEdit::editingFinished, [newtask, tasktitleEdit]() {
        newtask->setname(tasktitleEdit->text());
        //qDebug() << "input_name:" << newtask->getname();
    });
    QLabel* taskdesc = new QLabel();
    taskdesc->setParent(addTaskWindow);
    taskdesc->move(60, 120);
    taskdesc->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    taskdesc->setText("任务描述");
    //任务描述输入框
    QTextEdit* taskdescEdit = new QTextEdit(addTaskWindow);
    //任务描述输入框
    taskdescEdit->setGeometry(QRect(QPoint(60, 150), QSize(900, 200)));
    taskdescEdit->setPlaceholderText("请输入任务描述...");
    taskdescEdit->setStyleSheet("QTextEdit{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    QString input_desc;
    QPushButton* confirm_desc = new QPushButton(addTaskWindow);
    confirm_desc->setGeometry(QRect(QPoint(255, 360), QSize(60, 30)));
    confirm_desc->setText("确认");
    confirm_desc->setStyleSheet("QPushButton{background-color : #92bd6c; border : 2px #dedede; border-radius: 4px;}\
                                 QPushButton:hover{background-color : lightgreen;}"
    );
    connect(confirm_desc, &QPushButton::clicked, [newtask, taskdescEdit]() {
        newtask->setdescription(taskdescEdit->toPlainText());
        //qDebug() << "input_desc:" << newtask->getdescription();
    });
    QPushButton* clear_desc = new QPushButton(addTaskWindow);
    clear_desc->setGeometry(QRect(QPoint(705, 360), QSize(60, 30)));
    clear_desc->setText("清空");
    clear_desc->setStyleSheet("QPushButton{background-color : #f0b987; border : 2px #dedede; border-radius: 4px;}\
                               QPushButton:hover{background-color : #f7e8a3;}"
    );
    connect(clear_desc, &QPushButton::clicked, [taskdescEdit]() {
        taskdescEdit->clear();
    });
    QLabel* ddltime = new QLabel(addTaskWindow);
    ddltime->move(60, 400);
    ddltime->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    ddltime->setText("截止时间");
    //截止时间输入框,截止时间默认当前时间+1天
    QDateTimeEdit* ddltimeEdit = new QDateTimeEdit(addTaskWindow);
    QDateTime current_time = QDateTime::currentDateTime();
    current_time = current_time.addDays(1);
    ddltimeEdit->setGeometry(QRect(QPoint(90, 430), QSize(200, 30)));
    ddltimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");//设置显示格式为年-月-日 时:分
    ddltimeEdit->setCalendarPopup(true);
    ddltimeEdit->setDate(current_time.date());
    ddltimeEdit->setTime(current_time.time());
    ddltimeEdit->setStyleSheet("QDateTimeEdit{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    connect(ddltimeEdit, &QDateTimeEdit::dateTimeChanged, [newtask, ddltimeEdit]() {
        newtask->set_ddldate(ddltimeEdit->date());
        newtask->set_ddltime(ddltimeEdit->time());
        //qDebug() << "ddl_date:" << newtask->get_ddldate() << "ddl_time:" << newtask->get_ddltime();
    });
    //闹钟提醒
    QLabel* alarming = new QLabel(addTaskWindow);
    alarming->move(510, 400);
    alarming->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    alarming->setText("闹钟提醒");
    //这里以后是滑动条来实现闹钟提醒功能
    QCheckBox* alarmingCheck = new QCheckBox(addTaskWindow);
    //默认不提醒
    alarmingCheck->setChecked(false);
    alarmingCheck->setGeometry(QRect(QPoint(510, 430), QSize(100, 30)));
    alarmingCheck->setStyleSheet("QCheckBox{font-size: 18px; color: black;}");
    connect(alarmingCheck, &QCheckBox::stateChanged, [newtask, alarmingCheck]() {
        newtask->set_trigger(alarmingCheck->isChecked());
        //qDebug() << "alarming:" << newtask->get_trigger();
    });
    //任务种类
    QLabel* tasktype = new QLabel(addTaskWindow);
    tasktype->move(60, 497);
    tasktype->setStyleSheet("QLabel{font-size: 16px; font-weight: bold; color: #b5b5b4;}");
    tasktype->setText("任务种类");
    //任务种类选择框
    QComboBox* tasktypeCombo = new QComboBox(addTaskWindow);
    //获取任务种类列表
    std::vector<Kind_Tag> kindtags = kind_taglist->get_allkinds();
    for(const Kind_Tag& kindtag : kindtags){
        tasktypeCombo->addItem(kindtag.getname());
    }
    //设置默认选择第一个任务种类
    tasktypeCombo->setCurrentIndex(0);
    tasktypeCombo->setGeometry(QRect(QPoint(180, 490), QSize(160, 30)));
    tasktypeCombo->setStyleSheet("QComboBox{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #d0f5ff;}");
    connect(tasktypeCombo, &QComboBox::currentTextChanged, [this, newtask, tasktypeCombo]() {
        newtask->set_task_kind(this->kind_taglist->get_kind(tasktypeCombo->currentText()));
        //qDebug() << "task_kind:" << newtask->get_task_kind().getname();
    });
    //任务属性
    QLabel* taskattribute = new QLabel(addTaskWindow);
    taskattribute->move(500, 497);
    taskattribute->setStyleSheet("QLabel{font-size: 16px; font-weight: bold; color: #b5b5b4;}");
    taskattribute->setText("任务属性");
    //任务属性选择框
    QComboBox* taskattributeCombo = new QComboBox(addTaskWindow);
    taskattributeCombo->addItem("紧急");
    taskattributeCombo->addItem("重要");
    taskattributeCombo->addItem("普通");
    //设置默认选择第三个任务属性
    taskattributeCombo->setCurrentIndex(2);
    taskattributeCombo->setGeometry(QRect(QPoint(620, 490), QSize(160, 30)));
    taskattributeCombo->setStyleSheet("QComboBox{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #d0f5ff;}");
    connect(taskattributeCombo, &QComboBox::currentTextChanged, [newtask, taskattributeCombo]() {
        switch (taskattributeCombo->currentIndex())
        {
        case 0:
            newtask->set_task_attribute(Attribute_Tag(TaskPriority::Urgent));
            break;
        case 1:
            newtask->set_task_attribute(Attribute_Tag(TaskPriority::Important));
            break;
        case 2:
            newtask->set_task_attribute(Attribute_Tag(TaskPriority::Normal));
            break;
        default:
            newtask->set_task_attribute(Attribute_Tag(TaskPriority::Normal));
            break;
        }
        //qDebug() << "task_attribute:" << newtask->get_task_attribute().get_priority();
    });
    
    //确认和取消按钮
    QPushButton* confirmButton = new QPushButton(addTaskWindow);
    confirmButton->setGeometry(QRect(QPoint(235, 630), QSize(100, 30)));
    confirmButton->setText("确认");
    confirmButton->setStyleSheet("QPushButton{background-color : #92bd6c; border : 2px #dedede; border-radius: 5px;}\
                                  QPushButton:hover{background-color : lightgreen;}"
    );
    connect(confirmButton, &QPushButton::clicked, [this, addTaskWindow, newtask]() {
        //检查输入的任务设置参数是否合法
        if(newtask->getname().isEmpty()){
            QMessageBox::warning(addTaskWindow, "错误", "任务名不能为空！");
            return;
        }
        if(newtask->getdescription().isEmpty()){
            QMessageBox::warning(addTaskWindow, "错误", "任务描述不能为空！");
            return;
        }
        if(newtask->get_ddldate().isNull() || newtask->get_ddltime().isNull()){
            QMessageBox::warning(addTaskWindow, "错误", "截止时间不能为空！");
            return;
        }
        if (newtask->get_ddldate() < QDate::currentDate()) {
            QMessageBox::warning(addTaskWindow, "错误", "截止时间不能小于当前日期！");
            return;
        }
        if (newtask->get_ddldate() == QDate::currentDate() && newtask->get_ddltime() <= QTime::currentTime()) {
            QMessageBox::warning(addTaskWindow, "错误", "截止时间不能小于当前时间！");
            return;
        }
        //添加到tasklist中
        newtask->set_settingdate(QDate::currentDate());
        this->taskpos++;//编号加一
        this->tasklist.append(newtask);
        //更新tasklist显示
        //updateTaskList();
        addTaskWindow->close();
        delete addTaskWindow;
    });
    QPushButton* cancelButton = new QPushButton(addTaskWindow);
    cancelButton->setGeometry(QRect(QPoint(685, 630), QSize(100, 30)));
    cancelButton->setText("取消");
    cancelButton->setStyleSheet("QPushButton{background-color : #f0b987; border : 2px #dedede; border-radius: 5px;}\
                                 QPushButton:hover{background-color : #f7e8a3;}"
    );
    connect(cancelButton, &QPushButton::clicked, [addTaskWindow, newtask]() {
        delete newtask;//放弃创建的Task对象
        addTaskWindow->close();
        delete addTaskWindow;
    });
    addTaskWindow->show();
}*/
