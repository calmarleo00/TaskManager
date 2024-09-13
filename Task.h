#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
class Task: public QObject{
    Q_OBJECT
    QString name;
    QString description;
    Task* next;

    public:
        Task();
        Task(QString name);
        QString getTaskName();
        void setTaskName(QString name);
        Task* getTaskDescription();
        void setTaskDescription(Task* next);
        Task* getTaskNext();
        void setTaskNext(Task* next);
        void execute();
};

#endif // TASK_H
