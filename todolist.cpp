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
    }*/
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
        this->tasklist->display_add_taskwindow(displayArea);
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
    date_and_week->setStyleSheet("QLabel{font-family:Microsoft YaHei;font-size:20px;background-color:transparent;color:#9b9b9a;}");
    QLocale locale = QLocale::Chinese;
    date_and_week->setText("设置日期:  " + cur_task->get_settingdate().toString("yyyy年MM月dd日  ") + locale.toString(cur_task->get_settingdate(), "ddd"));
    date_and_week->setAlignment(Qt::AlignLeft);
    QLabel* task_name = new QLabel(taskwindow);
    task_name->setGeometry(30, 60, 500, 50);
    task_name->setStyleSheet("QLabel{font-size:40px;font-weight:bold;background-color:transparent;color:#1c1c19;}");
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
    ddl_label->setStyleSheet("QLabel{background-color:transparent;font-size:28px;color:#eea041;}");
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
    complete_checkbox->setChecked(false);//默认未完成,点击后弹出确认框,确认后任务删除
    //确认完成按钮点击事件
    //connect(complete_checkbox, &QCheckBox::clicked, this, &Task::complete_task);
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
    // pass
    //删除按钮
    QPushButton* delete_button = new QPushButton(taskwindow);
    delete_button->setGeometry(840, 20, 30, 30);
    QPixmap delete_icon(":/pic_resources/delete.png");
    delete_button->setIcon(QIcon(delete_icon));
    delete_button->setIconSize(QSize(24, 24));
    delete_button->setStyleSheet("QPushButton{background-color:transparent;border-radius:3px;}\
        QPushButton:hover{ background-color:#d5d7d8; }"
    );
    //点击删除按钮弹出删除确认框
    // pass
}

void TodoList::generate_tasklist_widgets()
{
    //生成任务列表页面(若干)并添加到tasklist_widgets中,每页显示3个任务
    int widget_num = tasklist->tasknum() / 3 + 1;
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
        //connet;
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
        tasklistWidget->removeWidget(tempwidget);
    }
    for (QWidget* tasklist_widget : tasklist->tasklist_widgets) {
        delete tasklist_widget;
    }
    tasklist->tasklist_widgets.clear();
    //重新生成tasklist_widgets
    generate_tasklist_widgets();
    //重新显示任务列表页面
    for (QWidget* tasklist_widget : tasklist->tasklist_widgets) {
        tasklistWidget->addWidget(tasklist_widget);
    }
    tasklistWidget->setCurrentIndex(0);
}
