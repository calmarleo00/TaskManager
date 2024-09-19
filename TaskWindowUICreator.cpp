#include <TaskWindowUICreator.h>
#include <TaskUpdateWindowUIDecorator.h>
#include <TaskCreationWindowUI.h>
#include <MainWindowUI.h>

TaskWindowUI* TaskWindowUICreator::createTaskWindow(QString mode){
    if(mode == "create"){
        TaskCreationWindowUI* taskCreationWindow = new TaskCreationWindowUI();
        taskCreationWindow->setupTaskWidgetsUI();
        connect(taskCreationWindow, &TaskCreationWindowUI::signalAddNewTaskToUI, MainWindowUI::getInstance(), &MainWindowUI::addNewTaskToMainWindowUI);
        return taskCreationWindow;
    }
    else if(mode == "update"){
        Task* task = AppController::getInstance()->taskHead;
        while(task->getTaskName() != taskName){
            task = task->getTaskNext();
        }
        TaskUpdateWindowUIDecorator* taskUpdateWindow = new TaskUpdateWindowUIDecorator(new TaskCreationWindowUI(), task);
        taskUpdateWindow->setupTaskWidgetsUI();
        connect(taskUpdateWindow, &TaskUpdateWindowUIDecorator::signalUpdateTaskToMainWindowUI, MainWindowUI::getInstance(), &MainWindowUI::updateTaskToMainWindowUI);
        return taskUpdateWindow;
    }
    else{
        return nullptr;
    }
}
