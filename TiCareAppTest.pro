TEMPLATE = app
TARGET = TiCareAppTest

QT = core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    AppController.cpp \
    MainWindowUI.cpp \
    Schedule.cpp \
    Task.cpp \
    Task2.cpp \
    TaskCreationAttributeUI.cpp \
    TaskCreationWindowUI.cpp \
    TaskRepeatableSchedulingUIBuilder.cpp \
    main.cpp

HEADERS += \
    AppController.h \
    MainWindowUI.h \
    Schedule.h \
    Task.h \
    TaskCreationAttributeUI.h \
    TaskCreationSchedulingUIBuilder.h \
    TaskCreationSchedulingUIDirector.h \
    TaskCreationWindowUI.h \
    TaskFixedSchedulingUIBuilder.h \
    TaskRepeatableSchedulingUIBuilder.h



