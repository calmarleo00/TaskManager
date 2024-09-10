#include <QApplication>
#include <QtWidgets>
#include <tasks.h>
#include <QObject>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Task* taskHead = new Task("Task1");
    Task* task2 = new Task("Task2");
    Task* taskTail = task2;
    (*taskHead).setTaskNext(task2);
    QWidget mainWindow;
    mainWindow.setWindowState(Qt::WindowMaximized);
    mainWindow.show();
    Task* taskIter = taskHead;
    int position = 0;
    int* xPosition = &position;
    while(taskIter){
        QPushButton* button = new QPushButton((*taskIter).getTaskName(), &mainWindow);
        button->setFixedSize(220, 220);
        button->move(*xPosition, 0);
        button->show();
        *xPosition += 220;
        taskIter = (*taskIter).getTaskNext();
    }

    QPushButton* newTaskButton = new QPushButton("Create a new task", &mainWindow);
    newTaskButton->move(440, 440);
    newTaskButton->show();
    newTaskButton->connect(newTaskButton, &QPushButton::released, qApp, [&mainWindow, taskTail, &xPosition]() {Task::createNewTask(&mainWindow, taskTail, "Task 3", xPosition);});
    return app.exec();
}


