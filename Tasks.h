#ifndef TASKS_H
#define TASKS_H
#include <QString>
#include <QObject>
#include <iostream>
#include <QPushButton>

class Task: public QObject{
    Q_OBJECT
    QString name;
    Task* next;
    virtual ~Task() {};
    public:
        Task(){
            name = "default";
            this->next = nullptr;
        }
    public:
        Task(QString name)
        {
            this->name = name;
            this->next = nullptr;
        }

    QString getTaskName(){
        return name;
    }

    void setTaskName(QString name){
        this->name = name;
    }

    Task* getTaskNext(){
        return next;
    }

    void setTaskNext(Task* next){
        this->next = next;
    }

    public slots:
        static void createNewTask(QWidget* mainWindow, Task* tail, QString name, int* position){
            Task* newTask = new Task();
            (*newTask).name = name;
            (*newTask).next = nullptr;
            tail->next = newTask;
            QPushButton* newTaskButton = new QPushButton(name, mainWindow);
            newTaskButton->setFixedSize(220, 220);
            newTaskButton->move(*position, 0);
            *position += 220;
            newTaskButton->show();
        }
};

#endif // TASKS_H
