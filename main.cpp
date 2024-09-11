#include <QApplication>
#include <QtWidgets>
#include <tasks.h>
#include <QObject>
#include <QGridlayout>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    Task* taskHead = new Task("Task1");
    Task* task2 = new Task("Task2");
    Task* taskTail = task2;
    int nRows = 2;
    int nCols = 3;
    (*taskHead).setTaskNext(task2);
    QWidget mainWindow;
    mainWindow.setWindowState(Qt::WindowMaximized);
    mainWindow.show();
    Task* taskIter = taskHead;
    int position = 0;
    int* xPosition = &position;
    QGridLayout* mainGrid = new QGridLayout();
    mainWindow.setLayout(mainGrid);
    QGridLayout* taskGrid = new QGridLayout(&mainWindow);

    QWidget* containerWidget = new QWidget();
    containerWidget->setLayout(taskGrid);

    QScrollArea* scroll_area = new QScrollArea();
    scroll_area->setWidget(containerWidget);
    scroll_area->setWidgetResizable(true);

    mainGrid->addWidget(scroll_area, 0, 0, 1, 3);

    int col = 0;
    int* colPointer = &col;
    while(taskIter){
        QPushButton* button = new QPushButton((*taskIter).getTaskName());
        taskGrid->addWidget(button, 0, col, 1, 1);
        //buttonBox->addWidget(button);
        button->setFixedSize(220, 220);
        button->move(*xPosition, 0);
        button->show();
        *xPosition += 220;
        taskIter = (*taskIter).getTaskNext();
        col += 1;
    }

    QPushButton* newTaskButton = new QPushButton("Create a new task", &mainWindow);
    mainGrid->addWidget(newTaskButton, 1, 0, 1, 1);
    newTaskButton->move(440, 440);
    newTaskButton->show();
    newTaskButton->connect(newTaskButton, &QPushButton::released, qApp, [taskGrid, taskTail, &xPosition, colPointer]() {Task::createNewTask(taskGrid, taskTail, "Task 3", xPosition, colPointer);});
    QPushButton* modifyTaskButton = new QPushButton("Modify selected tasks", &mainWindow);
    mainGrid->addWidget(modifyTaskButton, 1, 1, 1, 1);
    modifyTaskButton->show();
    QPushButton* deleteTaskButton = new QPushButton("Delete selected tasks", &mainWindow);
    mainGrid->addWidget(deleteTaskButton, 1, 2, 1, 1);
    deleteTaskButton->show();
    return app.exec();
}


