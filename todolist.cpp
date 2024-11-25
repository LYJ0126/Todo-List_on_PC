#include "todolist.h"
#include "ui_todolist.h"
#include <QPushButton>

TodoList::TodoList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TodoList),
    centralwidget(new QWidget(this)),
    displayArea(new QWidget(this)),
    stackedWidget(new QStackedWidget(displayArea)),
    tasklistWidget(new QWidget(stackedWidget)),
    historyWidget(new QWidget(stackedWidget)),
    feedbackWidget(new QWidget(stackedWidget)),
    settingsWidget(new QWidget(stackedWidget))
{
    ui->setupUi(this);
    this->setWindowTitle("Happy Todo List");
    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground); // 可选：如果希望窗口背景透明
    //this->setStyleSheet("QMainWindow{background-color : #f7f7f7;}");
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
}

void TodoList::initUI()
{
    //初始化UI界面
    centralwidget->setGeometry(QRect(QPoint(0,0),QSize(1080,720)));
    centralwidget->setStyleSheet("QWidget{background-color : black}");
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
    tasklistWidget->setParent(stackedWidget);
    historyWidget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    historyWidget->setParent(stackedWidget);
    feedbackWidget->setGeometry(QRect(QPoint(0,0), QSize(1020,680)));
    feedbackWidget->setParent(stackedWidget);
    settingsWidget->setGeometry(QRect(QPoint(0, 0), QSize(1020, 680)));
    tasklistWidget->setStyleSheet("QWidget{background-color : lightblue;}");
    historyWidget->setStyleSheet("QWidget{background-color : lightgreen;}");
    feedbackWidget->setStyleSheet("QWidget{background-color : lightyellow;}");
    settingsWidget->setStyleSheet("QWidget{background-color : lightcoral;}");

    // 将这些窗口添加到 QStackedWidgets
    stackedWidget->addWidget(tasklistWidget);
    stackedWidget->addWidget(historyWidget);
    stackedWidget->addWidget(feedbackWidget);
    stackedWidget->addWidget(settingsWidget);


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
