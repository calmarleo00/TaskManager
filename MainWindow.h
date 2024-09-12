#include<QWidget>
#include<Task.h>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
class MainWindow : public QWidget{
    public:
        static MainWindow* instance;
    int col = 0;
    int* colPointer = &col;
    // Define two pointers to the first and last task in the list
    Task* taskHead = nullptr;
    Task* taskTail = nullptr;
    // Define a pointer that let iterate when necessary through the list of tasks
    Task* taskIter = nullptr;

    MainWindow(){};
    virtual ~MainWindow(){};

    public slots:
        void addNewTask();

    static MainWindow* getInstance();
};

#endif // MAINWINDOW_H
