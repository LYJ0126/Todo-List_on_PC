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
#include "tasklist.h"
#include "kind_taglist.h"

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
    Kind_TagList* kind_taglist;
    QWidget* topBar;//顶栏
    QWidget* leftBar;//左侧菜单栏
    QWidget* displayArea;//显示区(顶栏下方和左侧菜单栏右侧)
    QStackedWidget* stackedWidget;
    QWidget* tasklistWidget;
    QWidget* historyWidget;
    QWidget* settingsWidget;
    QWidget* feedbackWidget;

private slots:
    void showTaskList();
    void showHistory();
    void showFeedback();
    void showSettings();
    
};
#endif // TODOLIST_H