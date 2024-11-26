#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <QToolButton>
#include <QTimer>
#include <QVector>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "tasklist.h"
//#include "kind_taglist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TodoList;
}
QT_END_NAMESPACE

class TodoList : public QMainWindow
{
    Q_OBJECT
public:
    TodoList(QWidget *parent = nullptr);
    ~TodoList();
    void init();//初始化数据
    void initUI();//初始化UI

private:
    Ui::TodoList *ui;
    TaskList* tasklist;
    //Kind_TagList* kind_taglist;
    //下面为UI部分
    QWidget* topBar;//顶栏
    QWidget* leftBar;//左侧菜单栏
    QWidget* centralwidget;
    QWidget* displayArea;//显示区
    QStackedWidget* stackedWidget;
    QStackedWidget* tasklistWidget;//任务列表,因为有可能要翻页,所以用QStackedWidget
    QWidget* historyWidget;
    QWidget* feedbackWidget;
    QWidget* settingsWidget;
    void generate_taskwindow(int pos);//为下标为pos的任务生成一个任务窗口
    void generate_tasklist_widgets();//生成任务列表窗口(若干QWidget*，存储在tasklist->tasklist_widgets中)
    void update_tasklist_display();//更新任务列表展示页面(更新tasklistWidget)

private slots:
    void showTaskList();
    void showHistory();
    void showFeedback();
    void showSettings();
    
};
#endif // TODOLIST_H
