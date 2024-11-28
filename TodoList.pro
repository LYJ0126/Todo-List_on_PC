QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    attribute_tag.cpp \
    kind_tag.cpp \
    kind_taglist.cpp \
    main.cpp \
    tag.cpp \
    task.cpp \
    tasklist.cpp \
    todolist.cpp

HEADERS += \
    attribute_tag.h \
    kind_tag.h \
    kind_taglist.h \
    tag.h \
    task.h \
    tasklist.h \
    todolist.h

FORMS += \
    todolist.ui

RC_FILE = TodoList.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
