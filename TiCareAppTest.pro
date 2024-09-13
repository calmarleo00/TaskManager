TEMPLATE = app
TARGET = TiCareAppTest

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    MainWindow.cpp \
    MainWindowUI.cpp \
    Task.cpp \
    Task2.cpp \
    TaskCreationWindow.cpp \
    main.cpp

HEADERS += \
    MainWindow.h \
    MainWindowUI.h \
    Task.h \
    TaskCreationWindowUI.h



