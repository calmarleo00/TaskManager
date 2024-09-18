TEMPLATE = app
TARGET = TiCareAppTest

QT = core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    AppController.cpp \
    MainWindowUI.cpp \
    Schedule.cpp \
    Scheduler.cpp \
    Task.cpp \
    Task2.cpp \
    TaskAttributeUI.cpp \
    TaskCreationWindowUI.cpp \
    TaskRepeatableSchedulingUIBuilder.cpp \
    TaskUpdateWindowUI.cpp \
    main.cpp

HEADERS += \
    AppController.h \
    MainWindowUI.h \
    Schedule.h \
    Scheduler.h \
    Task.h \
    TaskAttributeUI.h \
    TaskCreationSchedulingUIBuilder.h \
    TaskCreationSchedulingUIDirector.h \
    TaskCreationWindowUI.h \
    TaskFixedSchedulingUIBuilder.h \
    TaskRepeatableSchedulingUIBuilder.h \
    TaskUpdateWindowUI.h \
    TaskWindowUI.h \
    TaskWindowUICreator.h



