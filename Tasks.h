#ifndef TASKS_H
#define TASKS_H
#include "QtWidgets"
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
        static void createNewTask(QGridLayout* grid, Task* tail, QString name, int* position, int* col){
            Task* newTask = new Task();
            (*newTask).name = name;
            (*newTask).next = nullptr;
            tail->next = newTask;
            QPushButton* newTaskButton = new QPushButton(name);
            grid->addWidget(newTaskButton, 0, *col, 1, 1);
            //buttonBox->addWidget(newTaskButton);
            newTaskButton->setFixedSize(220, 220);
            newTaskButton->move(*position, 0);
            *position += 220;
            *col += 1;
            newTaskButton->show();
        }
};

#endif // TASKS_H
