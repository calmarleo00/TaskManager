#include <QtWidgets>
#include <Task.h>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <TaskCreationWindowUI.h>
#include <AppController.h>
#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

class MainWindowUI: public QWidget{
    public:
        QGridLayout* gridTaskMainWindowLayout = new QGridLayout();
        static MainWindowUI* mainWindowUI;
        int col = 0;
    private:
        MainWindowUI(){
            // Initialize the main window class which handle the business logic
            AppController::getInstance();
        };

    public:
        static MainWindowUI* getInstance();
        void setupMainWindowUI();
        void addNewTaskToMainWindowUI(QString taskName, QString taskDescrption);
        void deleteTaskFromMainWindowUI();
    private slots:
        void openNewTaskCreationWindowUI();
};

#endif // MAINWINDOWUI_H
