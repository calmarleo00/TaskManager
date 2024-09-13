#include <QtWidgets>
#include <Task.h>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <TaskCreationWindowUI.h>
#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

class MainWindowUI{
    QGridLayout* mainGrid = new QGridLayout();
    QGridLayout* taskGrid = new QGridLayout();
    // A container useful for defining a scroll area for taskGrid
    QWidget* containerWidget = new QWidget();
    // A scroll area that will make available scrolling through the tasks
    QScrollArea* scrollArea = new QScrollArea();
    QPushButton* newTaskButton = new QPushButton("Create a new task");
    QPushButton* modifyTaskButton = new QPushButton("Modify selected tasks");
    QPushButton* deleteTaskButton = new QPushButton("Delete selected tasks");

    private:
        MainWindowUI(){};
        static MainWindowUI* instance;
    public:
        void setupMainWindowUI(QWidget *mainWindow){
            mainWindow->setWindowState(Qt::WindowMaximized);
            mainWindow->setLayout(mainGrid);
            // Make the container follow the taskGrid's layout
            containerWidget->setLayout(taskGrid);
            // Set the scroll area's widget to the container
            scrollArea->setWidget(containerWidget);
            scrollArea->setWidgetResizable(true);
            // Insert inside the first row of the main grid the scroll area
            mainGrid->addWidget(scrollArea, 0, 0, 1, 3);
            // Create three button at the bottom of the window
            newTaskButton->setParent(mainWindow);
            newTaskButton->connect(newTaskButton, &QPushButton::released, new TaskCreationWindowUI(), &TaskCreationWindowUI::setupTaskUI);
            modifyTaskButton->setParent(mainWindow);
            deleteTaskButton->setParent(mainWindow);
            mainGrid->addWidget(newTaskButton, 1, 0, 1, 1);
            newTaskButton->show();
            mainGrid->addWidget(modifyTaskButton, 1, 1, 1, 1);
            modifyTaskButton->show();
            deleteTaskButton->setParent(mainWindow);
            mainGrid->addWidget(deleteTaskButton, 1, 2, 1, 1);
            deleteTaskButton->show();
            mainWindow->show();
        }

        void addNewTaskUI(Task* newTask, int* col){
            QPushButton* newTaskButton = new QPushButton(newTask->getTaskName());
            newTaskButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            taskGrid->addWidget(newTaskButton, 0, *col, 1, 1);
            taskGrid->setColumnMinimumWidth(*col, 220);
            newTaskButton->show();
        }

        static MainWindowUI* getInstance();
};

#endif // MAINWINDOWUI_H
