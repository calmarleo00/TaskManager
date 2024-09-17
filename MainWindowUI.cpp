#include "qsqldatabase.h"
#include "qsqlquery.h"
#include<MainWindowUI.h>

MainWindowUI *MainWindowUI::mainWindowUI = nullptr;


MainWindowUI *MainWindowUI::getInstance()
{
    if(MainWindowUI::mainWindowUI == nullptr){
        mainWindowUI = new MainWindowUI();
    }
    return MainWindowUI::mainWindowUI;
}

void MainWindowUI::setupMainWindowUI(){
    // Set main window to maximum and do not let modify the size
    mainWindowUI->setWindowState(Qt::WindowMaximized);
    mainWindowUI->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // Set a grid layout for the main window
    QGridLayout* gridMainWindowLayout = new QGridLayout();
    mainWindowUI->setLayout(gridMainWindowLayout);
    // A container useful for defining a scroll area for the task grid
    QWidget* taskGridContainer = new QWidget();
    taskGridContainer->setLayout(gridTaskMainWindowLayout);
    // A scroll area that will make available scrolling through the tasks
    QScrollArea* taskScrollArea = new QScrollArea();
    // Set the scroll area's widget to the container
    taskScrollArea->setWidget(taskGridContainer);
    taskScrollArea->setWidgetResizable(true);
    // Insert inside the first row of the main grid the scroll area
    gridMainWindowLayout->addWidget(taskScrollArea, 1, 0, 1, 3);
    // Create two button at the bottom of the window, one for task creation the other for task deletion
    QPushButton* newTaskButton = new QPushButton("Create a new task");
    QPushButton* deleteTaskButton = new QPushButton("Delete selected tasks");

    gridMainWindowLayout->addWidget(newTaskButton, 0, 0, 1, 1);
    gridMainWindowLayout->addWidget(deleteTaskButton, 0, 2, 1, 1);

    newTaskButton->connect(newTaskButton, &QPushButton::released, this, &MainWindowUI::openNewTaskCreationWindowUI);

    mainWindowUI->show();
}


void MainWindowUI::openNewTaskCreationWindowUI() {
    // Create a new instance of TaskCreationWindowUI
    TaskCreationWindowUI *taskWindow = new TaskCreationWindowUI();
    taskWindow->setupTaskCreationWidgetsUI();
    connect(taskWindow, &TaskCreationWindowUI::signalAddNewTaskToMainWindowUI, this, &MainWindowUI::addNewTaskToMainWindowUI);

}


void MainWindowUI::addNewTaskToMainWindowUI(QString taskName, QString taskDescription){
    QGroupBox* taskInformationGroup = new QGroupBox();
    taskInformationGroup->setMaximumSize(300, 700);
    QVBoxLayout* taskInformationGroupLayout = new QVBoxLayout();
    taskInformationGroup->setLayout(taskInformationGroupLayout);
    taskInformationGroup->setCheckable(true);

    QLabel* nameLabel = new QLabel(taskName);
    QLabel* descriptionLabel = new QLabel(taskDescription);

    taskInformationGroupLayout->addWidget(nameLabel, 0, Qt::AlignCenter);
    taskInformationGroupLayout->addWidget(descriptionLabel, 0, Qt::AlignCenter);

    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPushButton* detailButton = new QPushButton("View Task Details");
    detailButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    taskInformationGroupLayout->addWidget(detailButton, 0, Qt::AlignBottom);

    mainWindowUI->gridTaskMainWindowLayout->addWidget(taskInformationGroup, 0, col, 1, 1);
    mainWindowUI->gridTaskMainWindowLayout->setColumnMinimumWidth(col, 220);
    col += 1;
}
