#include "todolist.h"
#include "ui_todolist.h"
#include <QPushButton>

TodoList::TodoList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TodoList),
    centralwidget(new QWidget(this)),
    displayArea(new QWidget(this)),
    stackedWidget(new QStackedWidget(displayArea)),
    tasklistWidget(new QStackedWidget(stackedWidget)),
    historyWidget(new QWidget(stackedWidget)),
    feedbackWidget(new QWidget(stackedWidget)),
    settingsWidget(new QWidget(stackedWidget))
{
    ui->setupUi(this);
    this->setWindowTitle("Happy Todo List");
    setWindowIcon(QIcon(":/TodoList.ico"));
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground); // 可选：如果希望窗口背景透明
    this->setStyleSheet("QMainWindow{background-color : #f7f7f7;}");
}

TodoList::~TodoList()
{
    delete ui;
    //delete kind_taglist;
    //qDebug()<<"Kind_taglist deleted";
    qDebug()<<"Good bye!";
}

void TodoList::init()
{
    tasklist = new TaskList(this);
    tasklist->loadFromJson("tasklist.json");//目前固定filepath为可执行文件所在文件夹下的json文件

    //kind_taglist = new Kind_TagList();//初始化kind_tag列表，默认4种类种类标签
    //输出tasklist信息
    /*
    for (int i = 0; i < tasklist->tasknum(); i++) {
        qDebug() << "Task[" << i << "]:";
        qDebug() << "id:" << tasklist->getTask(i)->getid();
        qDebug() << "name:" << tasklist->getTask(i)->getname();
        qDebug() << "description:" << tasklist->getTask(i)->getdescription();
        qDebug() << "settingdate:" << tasklist->getTask(i)->get_settingdate().toString();
        qDebug() << "ddldate:" << tasklist->getTask(i)->get_ddldate().toString();
        qDebug() << "ddltime:" << tasklist->getTask(i)->get_ddltime().toString();
        qDebug() << "kind:" << tasklist->getTask(i)->get_task_kind().getname();
        qDebug() << "attribute" << tasklist->getTask(i)->get_task_attribute().get_priority();
        qDebug() << "status" << tasklist->getTask(i)->get_status();
        qDebug() << "trigger" << tasklist->getTask(i)->get_trigger();
        qDebug() << '\n';
    }
    */
}

void TodoList::initUI()
{
    //初始化UI界面
    centralwidget->setGeometry(QRect(QPoint(0,0),QSize(1080,720)));
    centralwidget->setStyleSheet("QWidget{background-color : pink}");
    setCentralWidget(centralwidget);
    this->setFixedSize(centralwidget->size());
    displayArea->setGeometry(QRect(QPoint(60,40),QSize(1020,680)));
    displayArea->setStyleSheet("QWidget{background-color : transparent}");
    //顶栏
    topBar = new QWidget(this);
    topBar->setGeometry(QRect(QPoint(0, 0), QSize(1080, 40)));
    topBar->setStyleSheet("QWidget{background-color : #d0f5ff}");
    topBar->show();
    //左上角logo
    QLabel* logoLabel = new QLabel(topBar); // 将QLabel创建为topBar的子控件
    logoLabel->setGeometry(QRect(QPoint(0, 0), QSize(75, 40))); // 设置QLabel的大小和位置
    QPixmap logo(":/pic_resources/logo.png");
    if(logo.isNull()) {
        qDebug() << "Failed to load logo!";
        return;
    }
    logo = logo.scaled(QSize(75, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoLabel->setPixmap(logo); // 设置QLabel的Pixmap
    logoLabel->show();
    //右上角添加按钮
    QToolButton* add = new QToolButton(topBar);
    //add->setFixedSize(38,38);
    //设置按钮图标
    QPixmap addpic(":/pic_resources/add.png");
    add->setIcon(QIcon(addpic));
    add->setIconSize(QSize(38,38));
    add->setGeometry(QRect(QPoint(920,0),QSize(40,40)));
    //添加任务按钮样式表
    QString add_styleSheet = R"(
        QToolButton {
            background-color: transparent; /* 或者与顶栏相同的颜色，但设置为透明以显示图像 */
            border : 1px #dedede;
        }
        QToolButton:hover {
            background-color: #a6c4cc; /* 悬停时的背景色，注意与图像透明部分兼容 */
        }
    )";
    add->setStyleSheet(add_styleSheet);

    QSize add_initialsize = add->size();
    connect(add, &QPushButton::clicked, [this, add, add_initialsize](){
        QSize add_newsize = add_initialsize * 0.8;
        add->resize(add_newsize);
        //缩小后，按钮的位置也要相应变化
        add->move(920 + (add_initialsize.width() - add_newsize.width()) / 2,  (add_initialsize.height() - add_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [add, add_initialsize]() {
            add->resize(add_initialsize);
            add->move(920, 0);
        });
        //展示添加任务窗口
        show_add_taskwindow(displayArea);
    });
    add->show();

    QPushButton* quit = new QPushButton("退出", topBar);
    quit->setGeometry(QRect(QPoint(1000, 5), QSize(60, 30)));
    QSize quit_initialsize = quit->size();
    quit->setStyleSheet("QPushButton{background-color : #a6c4cc; border : 2px #dedede;}\
                         QPushButton:hover{background-color : #f7e8a3;}");
    connect(quit, &QPushButton::clicked, this, [quit, this, quit_initialsize]() {
        QSize quit_newsize = quit_initialsize * 0.8;
        quit->resize(quit_newsize);
        //缩小后，按钮的位置也要相应变化
        quit->move(1000 + (quit_initialsize.width() - quit_newsize.width()) / 2,  5 + (quit_initialsize.height() - quit_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [quit, quit_initialsize]() {
            quit->resize(quit_initialsize);
            quit->move(1000, 5);
        });
        //退出程序
        this->close();
        });
    quit->show();

    stackedWidget->setGeometry(QRect(QPoint(0,0),QSize(1020,680)));
    stackedWidget->setStyleSheet("QStackedWidget{background-color : transparent;}");

    // 初始化 tasklist, history, settings 窗口内容
    tasklistWidget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    historyWidget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    feedbackWidget->setGeometry(QRect(QPoint(0,0), QSize(1020,680)));
    settingsWidget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    tasklistWidget->setStyleSheet("QWidget{background-color : orange;}");
    historyWidget->setStyleSheet("QWidget{background-color : lightgreen;}");
    feedbackWidget->setStyleSheet("QWidget{background-color : lightyellow;}");
    settingsWidget->setStyleSheet("QWidget{background-color : lightcoral;}");

    // 将这些窗口添加到 QStackedWidgets
    stackedWidget->addWidget(tasklistWidget);
    stackedWidget->addWidget(historyWidget);
    stackedWidget->addWidget(feedbackWidget);
    stackedWidget->addWidget(settingsWidget);
    /*
    //测试tasklistWidget
    QWidget* test_widget1 = new QWidget(tasklistWidget);
    test_widget1->setGeometry(QRect(QPoint(0,0),QSize(500,500)));
    test_widget1->setStyleSheet("QWidget{background-color:lightblue;}");
    QWidget* test_widget2 = new QWidget(tasklistWidget);
    test_widget2->setGeometry(QRect(QPoint(0,0),QSize(500,500)));
    test_widget2->setStyleSheet("QWidget{background-color:pink;}");
    QWidget* test_widget3 = new QWidget(tasklistWidget);
    test_widget3->setGeometry(QRect(QPoint(0,0),QSize(500,500)));
    test_widget3->setStyleSheet("QWidget{background-color:orange;}");
    tasklistWidget->addWidget(test_widget1);
    tasklistWidget->addWidget(test_widget2);
    tasklistWidget->addWidget(test_widget3);
    tasklistWidget->setCurrentIndex(0);//默认显示第一个窗口
    //测试按钮,测试翻页
    QPushButton* test_button = new QPushButton("上一页", test_widget1);
    test_button->setGeometry(QRect(QPoint(200,600),QSize(100,40)));
    test_button->setStyleSheet("QPushButton{background-color : #a6c4cc; border : 2px #dedede;}\
                               QPushButton:hover{background-color : #f7e8a3;}");
    connect(test_button, &QPushButton::clicked, [this]() {
        if(tasklistWidget->currentIndex() == 0) {
            tasklistWidget->setCurrentIndex(2);
        } else {
            tasklistWidget->setCurrentIndex(tasklistWidget->currentIndex() - 1);
        }
    });
    QPushButton* test_button2 = new QPushButton("下一页", test_widget1);
    test_button2->setGeometry(QRect(QPoint(400,600),QSize(100,40)));
    test_button2->setStyleSheet("QPushButton{background-color : #a6c4cc; border : 2px #dedede;}\
                                QPushButton:hover{background-color : #f7e8a3;}");
    connect(test_button2, &QPushButton::clicked, [this]() {
        if(tasklistWidget->currentIndex() == 2) {
            tasklistWidget->setCurrentIndex(0);
        } else {
            tasklistWidget->setCurrentIndex(tasklistWidget->currentIndex() + 1);
        }
    });
    */
    /*
    QLabel* test = new QLabel(tasklistWidget);
    test->setGeometry(QRect(QPoint(0,0),QSize(200,100)));
    test->setStyleSheet("QLabel{font:30px;color:red;background-color:lightgreen;}");
    test->setText("测试tasklistWidget");
    QToolButton* test2 = new QToolButton(tasklistWidget);
    test2->setGeometry(100,100,150,40);
    //test2->setStyleSheet("QToolButton{background-color:red;} QToolButton:hover{background-color:lightblue}");
    QString test2_styleSheet = R"(
        QToolButton {
            background-color: red;
            border : 2px #dedede;
        }
        QToolButton:hover {
            background-color: lightblue;
        }
    )";
    test2->setStyleSheet(test2_styleSheet);
    test2->setText("测试按钮");
    connect(test2, &QToolButton::clicked, [test](){
        test->setText("测试成功");
    });
    */

    //生成任务窗口
    for(int i = 0; i < tasklist->tasknum(); i++) {
        generate_taskwindow(i);
    }
    //生成任务列表窗口
    generate_tasklist_widgets();
    //将tasklist->tasklist_widgets中的窗口添加到tasklistWidget
    for (QWidget* tasklist_widget : tasklist->tasklist_widgets) {
        tasklistWidget->addWidget(tasklist_widget);
    }
    //设置默认显示第一个窗口
    tasklistWidget->setCurrentIndex(0);

    //左侧菜单栏
    leftBar = new QWidget(this);
    leftBar->setGeometry(QRect(QPoint(0, 40), QSize(60, 680)));
    leftBar->setStyleSheet("QWidget{background-color : #f8eaac;}");
    leftBar->show();
    //左侧菜单栏按钮
    QToolButton* display_tasklist = new QToolButton(leftBar);
    QPixmap display_tasklist_pic(":/pic_resources/show_tasklist.png");
    display_tasklist->setIcon(display_tasklist_pic);
    display_tasklist->setIconSize(QSize(20,20));
    display_tasklist->setGeometry(QRect(QPoint(10,80),QSize(40,40)));
    QString display_tasklist_styleSheet = R"(
        QToolButton {
            background-color: transparent;
            border : 2px #dedede;
        }
        QToolButton:hover {
            background-color: #c6ba82;
        }
    )";

    QSize display_tasklist_initialsize = display_tasklist->size();
    display_tasklist->setStyleSheet(display_tasklist_styleSheet);
    connect(display_tasklist, &QToolButton::clicked, [this, display_tasklist, display_tasklist_initialsize]() {
        QSize display_all_newsize = display_tasklist_initialsize * 0.8;
        display_tasklist->resize(display_all_newsize);
        //缩小后，按钮的位置也要相应变化
        display_tasklist->move(10 + (display_tasklist_initialsize.width() - display_all_newsize.width()) / 2,  80 + (display_tasklist_initialsize.height() - display_all_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [display_tasklist, display_tasklist_initialsize]() {
            display_tasklist->resize(display_tasklist_initialsize);
            display_tasklist->move(10, 80);
        });
        //展示所有任务
        this->showTaskList();
    });

    QToolButton *display_history = new QToolButton(leftBar);
    QPixmap display_history_pic(":/pic_resources/show_history.png");
    display_history->setIcon(display_history_pic);
    display_history->setIconSize(QSize(20,20));
    display_history->setGeometry(QRect(QPoint(10,160),QSize(40,40)));
    QString display_history_styleSheet = R"(
        QToolButton {
            background-color: transparent;
            border : 2px #dedede;
        }
        QToolButton:hover {
            background-color: #c6ba82;
        }
    )";
    display_history->setStyleSheet(display_history_styleSheet);
    QSize display_history_initialsize = display_history->size();
    connect(display_history, &QToolButton::clicked, [this, display_history, display_history_initialsize]() {
        QSize display_history_newsize = display_history_initialsize * 0.8;
        display_history->resize(display_history_newsize);
        //缩小后，按钮的位置也要相应变化
        display_history->move(10 + (display_history_initialsize.width() - display_history_newsize.width()) / 2,  160 + (display_history_initialsize.height() - display_history_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [display_history, display_history_initialsize]() {
            display_history->resize(display_history_initialsize);
            display_history->move(10, 160);
        });
        //展示历史记录
        this->showHistory();
    });

    QToolButton *display_feedback = new QToolButton(leftBar);
    QPixmap display_feedback_pic(":/pic_resources/show_feedback.png");
    display_feedback->setIcon(display_feedback_pic);
    display_feedback->setIconSize(QSize(20,20));
    display_feedback->setGeometry(QRect(QPoint(10,240),QSize(40,40)));
    QString display_feedback_styleSheet = R"(
        QToolButton {
            background-color: transparent;
            border : 2px #dedede;
        }
        QToolButton:hover {
            background-color: #c6ba82;
        }
    )";
    display_feedback->setStyleSheet(display_feedback_styleSheet);
    QSize display_feedback_initialsize = display_feedback->size();
    connect(display_feedback, &QToolButton::clicked, [this, display_feedback, display_feedback_initialsize]() {
        QSize display_feedback_newsize = display_feedback_initialsize * 0.8;
        display_feedback->resize(display_feedback_newsize);
        //缩小后，按钮的位置也要相应变化
        display_feedback->move(10 + (display_feedback_initialsize.width() - display_feedback_newsize.width()) / 2,  240 + (display_feedback_initialsize.height() - display_feedback_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [display_feedback, display_feedback_initialsize]() {
            display_feedback->resize(display_feedback_initialsize);
            display_feedback->move(10, 240);
        });
        //展示反馈页面
        this->showFeedback();
    });

    QToolButton *display_settings = new QToolButton(leftBar);
    QPixmap display_settings_pic(":/pic_resources/show_settings.png");
    display_settings->setIcon(display_settings_pic);
    display_settings->setIconSize(QSize(20,20));
    display_settings->setGeometry(QRect(QPoint(10,320),QSize(40,40)));
    QString display_settings_styleSheet = R"(
        QToolButton {
            background-color: transparent;
            border : 2px #dedede;
        }
        QToolButton:hover {
            background-color: #c6ba82;
        }
    )";
    display_settings->setStyleSheet(display_settings_styleSheet);
    QSize display_settings_initialsize = display_settings->size();
    connect(display_settings, &QToolButton::clicked, [this, display_settings, display_settings_initialsize]() {
        QSize display_settings_newsize = display_settings_initialsize * 0.8;
        display_settings->resize(display_settings_newsize);
        //缩小后，按钮的位置也要相应变化
        display_settings->move(10 + (display_settings_initialsize.width() - display_settings_newsize.width()) / 2,  320 + (display_settings_initialsize.height() - display_settings_newsize.height()) / 2);
        //按钮要恢复原状
        QTimer::singleShot(250, [display_settings, display_settings_initialsize]() {
            display_settings->resize(display_settings_initialsize);
            display_settings->move(10, 320);
        });
        //展示设置页面
        this->showSettings();
    });

    //初始展示页面
    stackedWidget->setCurrentIndex(0);
}


void TodoList::showTaskList()
{
    stackedWidget->setCurrentIndex(0);
    //qDebug()<<"show page 0";
}

void TodoList::showHistory()
{
    stackedWidget->setCurrentIndex(1);
    //qDebug()<<"show page 1";
}

void TodoList::showFeedback()
{
    stackedWidget->setCurrentIndex(2);
    //qDebug()<<"show page 2";
}

void TodoList::showSettings()
{
    stackedWidget->setCurrentIndex(3);
    //qDebug()<<"show page 3";
}

void TodoList::generate_taskwindow(int pos)
{
    //生成任务窗口
    Task* cur_task = tasklist->getTask(pos);//获取下标为pos的任务。用下标是因为任务的呈现方式是按照tasklist里的任务列表顺序展现的
    QWidget* taskwindow = cur_task->taskwindow;
    taskwindow->resize(900, 180);
    taskwindow->setStyleSheet("QWidget{background-color:#ffffff;border-radius:10px;}");
    //任务设置日期
    QLabel* date_and_week = new QLabel(taskwindow);
    date_and_week->setGeometry(10, 10, 360, 30);
    //设置日期和星期,格式如"2024年12月31日 星期二",字体为微软雅黑,大小为18
    date_and_week->setStyleSheet("QLabel{font-family:Microsoft YaHei;font-size:20px;background-color:#ffffff;color:#9b9b9a;}");
    QLocale locale = QLocale::Chinese;
    date_and_week->setText("设置日期:  " + cur_task->get_settingdate().toString("yyyy年MM月dd日  ") + locale.toString(cur_task->get_settingdate(), "ddd"));
    date_and_week->setAlignment(Qt::AlignLeft);
    QLabel* task_name = new QLabel(taskwindow);
    task_name->setGeometry(30, 60, 500, 50);
    task_name->setStyleSheet("QLabel{font-size:40px;font-weight:bold;background-color:#ffffff;color:#1c1c19;}");
    task_name->setText(cur_task->getname());
    task_name->setAlignment(Qt::AlignLeft);
    //"截止日期"
    QLabel* deadline = new QLabel(taskwindow);
    deadline->setGeometry(10, 130, 130, 40);
    deadline->setStyleSheet("QLabel{background-color:transparent;font-size:30px;color:#5db1fe;}");
    deadline->setText("截止日期:");
    deadline->setAlignment(Qt::AlignLeft);
    //截止日期和时间
    QDateTime ddl_datetime = QDateTime(cur_task->get_ddldate(), cur_task->get_ddltime());
    QLabel* ddl_label = new QLabel(taskwindow);
    ddl_label->setGeometry(150, 130, 440, 40);
    ddl_label->setStyleSheet("QLabel{background-color:#ffffff;font-size:28px;color:#eea041;}");
    ddl_label->setText(ddl_datetime.toString("yyyy年MM月dd日 hh:mm"));
    ddl_label->setAlignment(Qt::AlignLeft);
    //任务种类
    QLabel* taskkind = new QLabel(taskwindow);
    taskkind->setGeometry(530, 10, 120, 30);
    taskkind->setStyleSheet("QLabel{background-color:#d5f6ff;font-size:22px;color:#49565a;border-radius:5px;}");
    taskkind->setText(cur_task->get_task_kind().getname());
    taskkind->setAlignment(Qt::AlignCenter);
    //任务属性
    QLabel* taskattribute = new QLabel(taskwindow);
    taskattribute->setGeometry(690, 10, 60, 30);
    //taskattribute->setStyleSheet("QLabel{background-color:#9edf9b;font-size:22px;}");
    switch (cur_task->get_task_attribute().get_priority()) {
    case TaskPriority::Urgent:
        taskattribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#e34631;border-radius:5px;}");
        taskattribute->setText("紧急");
        break;
    case TaskPriority::Important:
        taskattribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#eea144;border-radius:5px;}");
        taskattribute->setText("重要");
        break;
    case TaskPriority::Normal:
        taskattribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#1a251a;border-radius:5px;}");
        taskattribute->setText("普通");
        break;
    default:
        taskattribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#1a251a;border-radius:5px;}");
        taskattribute->setText("普通");
    }
    taskattribute->setAlignment(Qt::AlignCenter);
    //编辑按钮
    QPushButton* edit_button = new QPushButton(taskwindow);
    edit_button->setGeometry(780, 20, 30, 30);
    QPixmap edit_icon(":/pic_resources/edit.png");
    edit_button->setIcon(QIcon(edit_icon));
    edit_button->setIconSize(QSize(24, 24));
    edit_button->setStyleSheet("QPushButton{background-color:transparent;border-radius:3px;}\
        QPushButton:hover{ background-color:#d5d7d8; }"
    );
    //点击编辑按钮弹出编辑窗口
    connect(edit_button, &QPushButton::clicked, [this, cur_task,task_name, ddl_label,taskkind,taskattribute]() {
        show_edit_taskwindow(displayArea, cur_task, task_name, ddl_label, taskkind, taskattribute);
    });
    //任务状态设置
    QLabel* status_label = new QLabel(taskwindow);
    status_label->setGeometry(700, 70, 90, 30);
    status_label->setStyleSheet("QLabel{background-color:transparent;font-size:22px;color:#e563b8;}");
    status_label->setText("已完成");
    status_label->setAlignment(Qt::AlignCenter);
    //确认完成按钮
    QCheckBox* complete_checkbox = new QCheckBox(taskwindow);
    complete_checkbox->setGeometry(800, 70, 30, 30);
    complete_checkbox->setStyleSheet("QCheckBox{background-color:transparent}");
    complete_checkbox->setChecked(false);//默认未完成,点击后弹出确认框
    //根据任务是否完成设置
    if (cur_task->get_status() == Taskstatus::COMPLETED) {
        complete_checkbox->setChecked(true);
        complete_checkbox->setEnabled(false);//已完成的任务无法再修改确认完成按钮状态
    }
    //点击确认完成按钮后弹出确认框,确认后将任务状态设置为已完成,并且无法再修改确认完成按钮状态,且编辑按钮变为不可点击
    connect(complete_checkbox, &QCheckBox::clicked, [this, cur_task, complete_checkbox, edit_button]() {
        if (complete_checkbox->isChecked()) {
            QMessageBox::StandardButton button = QMessageBox::question(this, "确认完成", "确认将该任务标记为已完成?", QMessageBox::Yes | QMessageBox::No);
            if (button == QMessageBox::Yes) {
                cur_task->set_status(Taskstatus::COMPLETED);
                complete_checkbox->setChecked(true);
                //无法再修改确认完成按钮状态
                complete_checkbox->setEnabled(false);
                //编辑按钮变为不可点击
                edit_button->setEnabled(false);
            }
            else {
                complete_checkbox->setChecked(false);
            }
        }
        });
    //删除按钮
    QPushButton* delete_button = new QPushButton(taskwindow);
    delete_button->setGeometry(840, 20, 30, 30);
    QPixmap delete_icon(":/pic_resources/delete.png");
    delete_button->setIcon(QIcon(delete_icon));
    delete_button->setIconSize(QSize(24, 24));
    delete_button->setStyleSheet("QPushButton{background-color:transparent;border-radius:3px;}\
        QPushButton:hover{ background-color:#d5d7d8; }"
    );
    //点击删除按钮弹出删除确认框,确认后删除任务,并更新tasklist显示
    connect(delete_button, &QPushButton::clicked, [this, cur_task]() {
        QMessageBox::StandardButton button = QMessageBox::question(this, "确认删除", "确认删除该任务?", QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes) {
            tasklist->removeTask(cur_task->getid());
            tasklist->sortlist();
            update_tasklist_display();//更新tasklist显示
        }
    });
}

void TodoList::generate_tasklist_widgets()
{
    //生成任务列表页面(若干)并添加到tasklist_widgets中,每页显示3个任务
    int widget_num = (tasklist->tasknum() + 2) / 3;
    //qDebug()<<"widget_num:"<<widget_num;
    //打印任务列表
    /*
    for (int i = 0; i < tasklist->tasknum(); i++) {
        Task* task = tasklist->getTask(i);
        //qDebug()<<"task"<<i<<"name:"<<task->getname();
    }*/
    QPixmap toleft = QPixmap(":/pic_resources/toleft.png");
    QPixmap toright = QPixmap(":/pic_resources/toright.png");
    for (int i = 0; i < widget_num; i++) {
        QWidget* tasklist_widget = new QWidget(tasklistWidget);
        tasklist_widget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
        tasklist_widget->setStyleSheet("QWidget{background-color : #f7f7f7;}");
        tasklist->tasklist_widgets.append(tasklist_widget);
        //把任务列表中下标i*3到i*3+2的任务添加到tasklist_widget中(若没有则不添加)
        for (int j = i * 3; j < i * 3 + 3 && j < tasklist->tasknum(); j++) {
            Task* task = tasklist->getTask(j);
            int posid = j - i * 3;//这个页面中的第几个任务(从0开始)
            task->taskwindow->setParent(tasklist_widget);
            task->taskwindow->move(60, 60 + posid * 210);
        }
        //qDebug()<<"第"<<i<<"个任务列表页面完成添加任务窗口";
        //顶部添加排序方式选择框
        QLabel* sort_label = new QLabel(tasklist_widget);
        sort_label->setGeometry(QRect(QPoint(560, 10), QSize(100, 30)));
        sort_label->setStyleSheet("QLabel{font-size: 22px; font-weight: bold; color: #aaaaac;background-color : transparent;}");
        sort_label->setText("排序方式:");
        QComboBox* sort_combo = new QComboBox(tasklist_widget);
        sort_combo->setGeometry(QRect(QPoint(690, 10), QSize(180, 30)));
        sort_combo->setStyleSheet("QComboBox{font-size: 22px; border: 2px #dedede; border-radius: 5px;\
            color: #747578; background-color: #edeff0;}"
        );
        sort_combo->addItem("任务设置时间");
        sort_combo->addItem("任务截止时间");
        sort_combo->addItem(" 任务属性 ");
        //排序方式按照tasklist的排序方式设置
        switch (tasklist->get_sortmethod()) {
        case Sortmethod::Setdate:
            sort_combo->setCurrentIndex(0);
            break;
        case Sortmethod::Ddl:
            sort_combo->setCurrentIndex(1);
            break;
        case Sortmethod::Attribute:
            sort_combo->setCurrentIndex(2);
            break;
        default:
            break;
        }
        //修改排序方式
        connect(sort_combo, &QComboBox::currentIndexChanged, [this, sort_combo]() {
            int index = sort_combo->currentIndex();
            switch (index) {
            case 0:
                tasklist->set_sortmethod(Sortmethod::Setdate);
                tasklist->sortlist();
                break;
            case 1:
                tasklist->set_sortmethod(Sortmethod::Ddl);
                tasklist->sortlist();
                break;
            case 2:
                tasklist->set_sortmethod(Sortmethod::Attribute);
                tasklist->sortlist();
                break;
            default:
                break;
            }
            update_tasklist_display();
            //qDebug()<<"到了这里";
        });
        //左右两边添加窗口滑动选择按钮
        //注意第一个窗口没有左边按钮，最后一个窗口没有右边按钮
        if (i > 0) {
            QPushButton* left_button = new QPushButton(tasklist_widget);
            left_button->setGeometry(QRect(QPoint(10, 320), QSize(40, 40)));
            left_button->setIcon(QIcon(toleft));
            left_button->setIconSize(QSize(38, 38));
            left_button->setStyleSheet("QPushButton{background-color : transparent;border-radius: 5px;}\
                                        QPushButton:hover{background-color : #c6c6c6}"
            );
            //翻到上一页
            connect(left_button, &QPushButton::clicked, [this, i]() {
                tasklistWidget->setCurrentIndex(i - 1);
            });
        }
        if (i < widget_num - 1) {
            QPushButton* right_button = new QPushButton(tasklist_widget);
            right_button->setGeometry(QRect(QPoint(970, 320), QSize(40, 40)));
            right_button->setIcon(QIcon(toright));
            right_button->setIconSize(QSize(38, 38));
            right_button->setStyleSheet("QPushButton{background-color : transparent;border-radius: 5px;}\
                                        QPushButton:hover{background-color : #c6c6c6}"
            );
            //翻到下一页
            connect(right_button, &QPushButton::clicked, [this, i]() {
                tasklistWidget->setCurrentIndex(i + 1);
            });
        }
    }
}

void TodoList::update_tasklist_display()
{
    //在添加、删除任务以及选择新的排序方式后，会更新tasklist里的tasklist
    //需要更新tasklist->tasklist_widgets中的任务显示,先清空原有的任务显示
    while (tasklistWidget->count() > 0) {
        QWidget* tempwidget = tasklistWidget->widget(0);
        tempwidget->close();
        //tempwidget->deleteLater();

        tasklistWidget->removeWidget(tempwidget);
        /*
        if (tempwidget->parentWidget() == tasklistWidget) {
            qDebug() << "tempwidget的父窗口仍然是tasklistWidget";
        } else {
            qDebug() << "tempwidget的父窗口已经改变";
        }*/
    }
    //qDebug()<<"remove all widgets";
    //tasklistWidget->removeWidget()并不会真正删除tasklist_widget,只是从tasklistWidget中移除了它
    //tasklist_widget的父窗口还是tasklistWidget。不需要手动删除tasklist_widget,因为最后删除tasklistWidget时
    //会顺着对象树一路删除所有子对象,包括tasklist_widget。
    /*
    for (QWidget* tasklist_widget : tasklist->tasklist_widgets) {
        //delete tasklist_widget;
        //if(tasklist_widget->parentWidget() == tasklistWidget){
        //    qDebug()<<"tasklist_widget的父窗口仍然是tasklistWidget";
        //}
        //else{
        //    qDebug()<<"tasklist_widget的父窗口已经改变";
        //}
    }*/
    tasklist->tasklist_widgets.clear();
    //qDebug()<<"delete all tasklist_widgets";
    //qDebug()<<"tasklist num:"<<tasklist->tasknum();
    for (int i = 0; i < tasklist->tasknum(); i++) {
        //Task* task = tasklist->getTask(i);
        //task->taskwindow = new QWidget(tasklist);//先设置它的parent为tasklist,后面generate_tasklist_widgets()会重新设置它的parent为tasklist_widget
        generate_taskwindow(i);
    }
    //重新生成tasklist_widgets
    //qDebug()<<"before tasklist_widgets num:"<<tasklist->tasklist_widgets.size();
    generate_tasklist_widgets();
    //qDebug()<<"after tasklist_widgets num:"<<tasklist->tasklist_widgets.size();
    //重新显示任务列表页面
    for (QWidget* tasklist_widget : tasklist->tasklist_widgets) {
        tasklistWidget->addWidget(tasklist_widget);
    }
    tasklistWidget->setCurrentIndex(0);
    //qDebug()<<"update tasklist display";
}


void TodoList::show_add_taskwindow(QWidget* _parent)
{
    //_parent是TodoList::displayArea
    //下方添加两个按钮，确认和取消
    //点击确认按钮，检查输入的任务设置参数是否合法，如果合法，则创建Task对象，并添加到tasklist中，关闭窗口并释放窗口占用的内存
    //然后更新tasklist显示
    //点击取消按钮，关闭窗口并释放窗口占用的内存
    QWidget* addTaskWindow = new QWidget(_parent);
    //addTaskWindow->setAttribute(Qt::WA_DeleteOnClose, true);//设置窗口关闭时自动释放内存
    addTaskWindow->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    addTaskWindow->setStyleSheet("QWidget{background-color : #f7f7f7;}");
    Task* newtask = new Task(tasklist->taskpos + 1);
    newtask->setParent(tasklist);//这一步很关键，因为有可能用户直接退出程序导致newtask内存泄漏，
    //所以需要将newtask的父对象设置为tasklist，这样程序退出时，顺着对象树最后会释放newtask的内存
    newtask->taskwindow->setParent(tasklist);

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
    std::vector<Kind_Tag> kindtags = tasklist->kind_taglist->get_allkinds();
    for (const Kind_Tag& kindtag : kindtags) {
        tasktypeCombo->addItem(kindtag.getname());
    }
    //设置默认选择第一个任务种类
    tasktypeCombo->setCurrentIndex(0);
    tasktypeCombo->setGeometry(QRect(QPoint(180, 490), QSize(160, 30)));
    tasktypeCombo->setStyleSheet("QComboBox{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #d0f5ff;}");
    connect(tasktypeCombo, &QComboBox::currentTextChanged, [this, newtask, tasktypeCombo]() {
        newtask->set_task_kind(this->tasklist->kind_taglist->get_kind(tasktypeCombo->currentText()));
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
    confirmButton->setGeometry(QRect(QPoint(300, 630), QSize(100, 30)));
    confirmButton->setText("确认");
    confirmButton->setStyleSheet("QPushButton{background-color : #92bd6c; border : 2px #dedede; border-radius: 5px;}\
                                  QPushButton:hover{background-color : lightgreen;}"
    );
    connect(confirmButton, &QPushButton::clicked, [this, addTaskWindow, newtask]() {
        //检查输入的任务设置参数是否合法
        if (newtask->getname().isEmpty()) {
            QMessageBox::warning(addTaskWindow, "错误", "任务名不能为空！");
            return;
        }
        if (newtask->getdescription().isEmpty()) {
            QMessageBox::warning(addTaskWindow, "错误", "任务描述不能为空！");
            return;
        }
        if (newtask->get_ddldate().isNull() || newtask->get_ddltime().isNull()) {
            QMessageBox::warning(addTaskWindow, "错误", "截止时间不能为空！");
            return;
        }
        if (newtask->get_ddldate() < QDate::currentDate()) {
            QMessageBox::warning(addTaskWindow, "错误", "截止日期不能小于当前日期！");
            return;
        }
        if (newtask->get_ddldate() == QDate::currentDate() && newtask->get_ddltime() <= QTime::currentTime()) {
            QMessageBox::warning(addTaskWindow, "错误", "截止时间不能小于当前时间！");
            return;
        }
        //添加到tasklist中
        newtask->set_settingdate(QDate::currentDate());
        tasklist->taskpos++;
        tasklist->addTask(newtask);
        addTaskWindow->close();
        delete addTaskWindow;
        //更新tasklist显示
        tasklist->sortlist();
        update_tasklist_display();
        //qDebug()<<"tasklist num:"<<tasklist->tasknum();
    });
    QPushButton* cancelButton = new QPushButton(addTaskWindow);
    cancelButton->setGeometry(QRect(QPoint(700, 630), QSize(100, 30)));
    cancelButton->setText("取消");
    cancelButton->setStyleSheet("QPushButton{background-color : #f0b987; border : 2px #dedede; border-radius: 5px;}\
                                 QPushButton:hover{background-color : #f7e8a3;}"
    );
    connect(cancelButton, &QPushButton::clicked, [addTaskWindow, newtask]() {
        delete newtask->taskwindow;
        delete newtask;//放弃创建的Task对象
        addTaskWindow->close();
        delete addTaskWindow;
    });
    addTaskWindow->show();
}

void TodoList::show_edit_taskwindow(QWidget* _parent, Task* edit_task, QLabel* task_name, QLabel* ddl_label, QLabel* task_kind, QLabel* task_attribute)
{
    //_parent是TodoList::displayArea
    //和show_add_taskwindow()类似，只是显示的是编辑的任务，而不是新建任务
    //下方添加两个按钮，确认和取消
    //点击确认按钮，检查输入的任务设置参数是否合法，如果合法，则创建Task对象，并添加到tasklist中，关闭窗口并释放窗口占用的内存
    //然后更新tasklist显示
    //点击取消按钮，关闭窗口并释放窗口占用的内存
    QWidget* editTaskWindow = new QWidget(_parent);
    editTaskWindow->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    editTaskWindow->setStyleSheet("QWidget{background-color: #f7f7f7;}");
    Task* temp_task = new Task(0, edit_task->getname(), edit_task->getdescription(), edit_task->get_settingdate(), edit_task->get_ddldate(), edit_task->get_ddltime(), edit_task->get_task_kind(), edit_task->get_task_attribute(),Taskstatus::INCOMPLETE, edit_task->get_trigger());
    /*
    qDebug() << "temp_task:";
    qDebug() << "name:" << temp_task->getname();
    qDebug() << "description:" << temp_task->getdescription();
    qDebug() << "setting_date:" << temp_task->get_settingdate();
    qDebug() << "ddl_date:" << temp_task->get_ddldate();
    qDebug() << "task_kind:" << temp_task->get_task_kind().getname();
    qDebug() << "task_attribute:" << temp_task->get_task_attribute().get_priority();
    qDebug() << "task_status:" << temp_task->get_status();
    qDebug() << "alarming:" << temp_task->get_trigger();
    */
    temp_task->setParent(tasklist);
    QLabel* tasktitle = new QLabel();
    tasktitle->setParent(editTaskWindow);
    tasktitle->move(60, 30);
    tasktitle->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    tasktitle->setText("任务名");
    //任务标题输入框
    QLineEdit* tasktitleEdit = new QLineEdit(editTaskWindow);
    tasktitleEdit->setGeometry(QRect(QPoint(60, 60), QSize(900, 40)));
    //输入字体大小
    tasktitleEdit->setStyleSheet("QLineEdit{font-size: 22px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    //tasktitleEdit->setPlaceholderText(edit_task->getname());
    tasktitleEdit->setText(edit_task->getname());
    //QString new_name = edit_task->getname();
    connect(tasktitleEdit, &QLineEdit::editingFinished, [temp_task, tasktitleEdit]() {
        temp_task->setname(tasktitleEdit->text());
        //qDebug() << "input_name:" << temp_task->getname();
    });
    QLabel* taskdesc = new QLabel();
    taskdesc->setParent(editTaskWindow);
    taskdesc->move(60, 120);
    taskdesc->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    taskdesc->setText("任务描述");
    //任务描述输入框
    QTextEdit* taskdescEdit = new QTextEdit(editTaskWindow);
    //任务描述输入框
    taskdescEdit->setGeometry(QRect(QPoint(60, 150), QSize(900, 200)));
    //taskdescEdit->setPlaceholderText(edit_task->getdescription());
    taskdescEdit->setStyleSheet("QTextEdit{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    taskdescEdit->setText(edit_task->getdescription());
    QPushButton* confirm_desc = new QPushButton(editTaskWindow);
    confirm_desc->setGeometry(QRect(QPoint(255, 360), QSize(60, 30)));
    confirm_desc->setText("确认");
    confirm_desc->setStyleSheet("QPushButton{background-color : #92bd6c; border : 2px #dedede; border-radius: 4px;}\
                                 QPushButton:hover{background-color : lightgreen;}"
    );
    //QString new_desc = edit_task->getdescription();
    connect(confirm_desc, &QPushButton::clicked, [temp_task, taskdescEdit]() {
        temp_task->setdescription(taskdescEdit->toPlainText());
        //qDebug() << "input_desc:" << temp_task->getdescription();
    });
    QPushButton* clear_desc = new QPushButton(editTaskWindow);
    clear_desc->setGeometry(QRect(QPoint(705, 360), QSize(60, 30)));
    clear_desc->setText("清空");
    clear_desc->setStyleSheet("QPushButton{background-color : #f0b987; border : 2px #dedede; border-radius: 4px;}\
                               QPushButton:hover{background-color : #f7e8a3;}"
    );
    connect(clear_desc, &QPushButton::clicked, [taskdescEdit]() {
        taskdescEdit->clear();
    });
    QLabel* ddltime = new QLabel(editTaskWindow);
    ddltime->move(60, 400);
    ddltime->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    ddltime->setText("截止时间");
    //截止时间输入框
    QDateTimeEdit* ddltimeEdit = new QDateTimeEdit(editTaskWindow);
    ddltimeEdit->setGeometry(QRect(QPoint(90, 430), QSize(200, 30)));
    ddltimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");//设置显示格式为年-月-日 时:分
    ddltimeEdit->setCalendarPopup(true);
    ddltimeEdit->setDate(edit_task->get_ddldate());
    ddltimeEdit->setTime(edit_task->get_ddltime());
    ddltimeEdit->setStyleSheet("QDateTimeEdit{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #e6e7e7;}");
    //QDate new_ddldate = edit_task->get_ddldate();
    //QTime new_ddltime = edit_task->get_ddltime();
    connect(ddltimeEdit, &QDateTimeEdit::dateTimeChanged, [temp_task, ddltimeEdit]() {
        temp_task->set_ddldate(ddltimeEdit->date());
        temp_task->set_ddltime(ddltimeEdit->time());
        //qDebug() << "ddl_date:" << temp_task->get_ddldate() << "ddl_time:" << temp_task->get_ddltime();
    });
    //闹钟提醒
    QLabel* alarming = new QLabel(editTaskWindow);
    alarming->move(510, 400);
    alarming->setStyleSheet("QLabel{font-size: 15px; font-weight: bold; color: #b5b5b4;}");
    alarming->setText("闹钟提醒");
    //这里以后是滑动条来实现闹钟提醒功能
    QCheckBox* alarmingCheck = new QCheckBox(editTaskWindow);
    alarmingCheck->setChecked(edit_task->get_trigger());
    alarmingCheck->setGeometry(QRect(QPoint(510, 430), QSize(100, 30)));
    alarmingCheck->setStyleSheet("QCheckBox{font-size: 18px; color: black;}");
    //bool new_trigger = edit_task->get_trigger();
    connect(alarmingCheck, &QCheckBox::stateChanged, [temp_task, alarmingCheck]() {
        temp_task->set_trigger(alarmingCheck->isChecked());
        //qDebug() << "alarming:" << temp_task->get_trigger();
    });
    //任务种类
    QLabel* tasktype = new QLabel(editTaskWindow);
    tasktype->move(60, 497);
    tasktype->setStyleSheet("QLabel{font-size: 16px; font-weight: bold; color: #b5b5b4;}");
    tasktype->setText("任务种类");
    //任务种类选择框
    QComboBox* tasktypeCombo = new QComboBox(editTaskWindow);
    //获取任务种类列表
    std::vector<Kind_Tag> kindtags = tasklist->kind_taglist->get_allkinds();
    for (const Kind_Tag& kindtag : kindtags) {
        tasktypeCombo->addItem(kindtag.getname());
    }
    //tasktypeCombo->setCurrentIndex(0);
    for (int i = 0; i < (int)kindtags.size(); i++) {
        if (kindtags[i].getname() == edit_task->get_task_kind().getname()) {
            tasktypeCombo->setCurrentIndex(i);
            break;
        }
    }
    tasktypeCombo->setGeometry(QRect(QPoint(180, 490), QSize(160, 30)));
    tasktypeCombo->setStyleSheet("QComboBox{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #d0f5ff;}");
    //Kind_Tag new_kindtag = edit_task->get_task_kind();
    connect(tasktypeCombo, &QComboBox::currentTextChanged, [this, temp_task, tasktypeCombo]() {
        temp_task->set_task_kind(this->tasklist->kind_taglist->get_kind(tasktypeCombo->currentText()));
        //qDebug() << "task_kind:" << temp_task->get_task_kind().getname();
    });
    //任务属性
    QLabel* taskattribute = new QLabel(editTaskWindow);
    taskattribute->move(500, 497);
    taskattribute->setStyleSheet("QLabel{font-size: 16px; font-weight: bold; color: #b5b5b4;}");
    taskattribute->setText("任务属性");
    //任务属性选择框
    QComboBox* taskattributeCombo = new QComboBox(editTaskWindow);
    taskattributeCombo->addItem("紧急");
    taskattributeCombo->addItem("重要");
    taskattributeCombo->addItem("普通");
    taskattributeCombo->setCurrentIndex(edit_task->get_task_attribute().get_priority());
    taskattributeCombo->setGeometry(QRect(QPoint(620, 490), QSize(160, 30)));
    taskattributeCombo->setStyleSheet("QComboBox{font-size: 18px; border: 2px #dedede; border-radius: 5px; color: black; background-color: #d0f5ff;}");
    //Attribute_Tag new_attribute = edit_task->get_task_attribute();
    connect(taskattributeCombo, &QComboBox::currentTextChanged, [temp_task, taskattributeCombo]() {
        switch (taskattributeCombo->currentIndex())
        {
        case 0:
            temp_task->set_task_attribute(Attribute_Tag(TaskPriority::Urgent));
            break;
        case 1:
            temp_task->set_task_attribute(Attribute_Tag(TaskPriority::Important));
            break;
        case 2:
            temp_task->set_task_attribute(Attribute_Tag(TaskPriority::Normal));
            break;
        default:
            temp_task->set_task_attribute(Attribute_Tag(TaskPriority::Normal));
            break;
        }
        //qDebug() << "task_attribute:" << temp_task->get_task_attribute().get_priority();
    });
    //确认和取消按钮
    QPushButton* confirmButton = new QPushButton(editTaskWindow);
    confirmButton->setGeometry(QRect(QPoint(300, 630), QSize(100, 30)));
    confirmButton->setText("确认");
    confirmButton->setStyleSheet("QPushButton{background-color : #92bd6c; border : 2px #dedede; border-radius: 5px;}\
                                  QPushButton:hover{background-color : lightgreen;}"
    );
    connect(confirmButton, &QPushButton::clicked, [editTaskWindow, temp_task, edit_task, task_name, ddl_label, task_kind, task_attribute]() {
        //检查输入的任务设置参数是否合法
        if (temp_task->getname().isEmpty()) {
            QMessageBox::warning(editTaskWindow, "错误", "任务名不能为空！");
            return;
        }
        if (temp_task->getdescription().isEmpty()) {
            QMessageBox::warning(editTaskWindow, "错误", "任务描述不能为空！");
            return;
        }
        if (temp_task->get_ddldate().isNull() || temp_task->get_ddltime().isNull()) {
            QMessageBox::warning(editTaskWindow, "错误", "截止时间不能为空！");
            return;
        }
        if (temp_task->get_ddldate() < QDate::currentDate()) {
            QMessageBox::warning(editTaskWindow, "错误", "截止日期不能小于当前日期！");
            return;
        }
        if (temp_task->get_ddldate() == QDate::currentDate() && temp_task->get_ddltime() <= QTime::currentTime()) {
            QMessageBox::warning(editTaskWindow, "错误", "截止时间不能小于当前时间！");
            return;
        }
        //修改edit_task的属性
        edit_task->setname(temp_task->getname());
        edit_task->setdescription(temp_task->getdescription());
        edit_task->set_ddldate(temp_task->get_ddldate());
        edit_task->set_ddltime(temp_task->get_ddltime());
        edit_task->set_trigger(temp_task->get_trigger());
        edit_task->set_task_kind(temp_task->get_task_kind());
        edit_task->set_task_attribute(temp_task->get_task_attribute());
        //修改edit_task的显示
        //需要修改的显示内容有:任务名,任务截止时间,任务种类,任务属性
        task_name->setText(edit_task->getname());
        QDateTime ddl_datetime = QDateTime(edit_task->get_ddldate(), edit_task->get_ddltime());
        ddl_label->setText(ddl_datetime.toString("yyyy年MM月dd日 hh:mm"));
        task_kind->setText(edit_task->get_task_kind().getname());
        switch (edit_task->get_task_attribute().get_priority())
        {
        case TaskPriority::Urgent:
            task_attribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#e34631;border-radius:5px;}");
            task_attribute->setText("紧急");
            break;
        case TaskPriority::Important:
            task_attribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#eea144;border-radius:5px;}");
            task_attribute->setText("重要");
            break;
        case TaskPriority::Normal:
            task_attribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#1a251a;border-radius:5px;}");
            task_attribute->setText("普通");
            break;
        default:
            task_attribute->setStyleSheet("QLabel{background-color:#c5ecc3;font-size:22px;color:#1a251a;border-radius:5px;}");
            task_attribute->setText("普通");
            break;
        }
        delete temp_task->taskwindow;
        delete temp_task;
        editTaskWindow->close();
        delete editTaskWindow;
        //更新tasklist显示
        //tasklist->sortlist();
        //update_tasklist_display();
        //qDebug() << "tasklist num:" << tasklist->tasknum();
        //更新
    });
    QPushButton* cancelButton = new QPushButton(editTaskWindow);
    cancelButton->setGeometry(QRect(QPoint(700, 630), QSize(100, 30)));
    cancelButton->setText("取消");
    cancelButton->setStyleSheet("QPushButton{background-color : #f0b987; border : 2px #dedede; border-radius: 5px;}\
                                 QPushButton:hover{background-color : #f7e8a3;}"
    );
    connect(cancelButton, &QPushButton::clicked, [editTaskWindow, temp_task]() {
        delete temp_task->taskwindow;
        delete temp_task;
        editTaskWindow->close();
        delete editTaskWindow;
    });
    editTaskWindow->show();
}
