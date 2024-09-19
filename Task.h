#ifndef TASK_H
#define TASK_H
#include "qjsonobject.h"
#include <QString>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <QDate>
#include <Schedule.h>
class Task: public QObject{
    Q_OBJECT
    protected:
        QString name;
        QString description;
        bool isExternal;
        bool isRepeatable;
        QString command;
        QJsonObject parameters;
        QDate startDate;
        QDate endDate;
        Schedule** scheduling = new Schedule*[7];
        Task* nextTask;

    public:
        Task();
        Task(QString name);
        QString getTaskName();
        void setTaskName(QString name);
        QString getTaskDescription();
        void setTaskDescription(QString description);
        bool getIsExternal();
        void setIsExternal(bool isExternal);
        bool getIsRepeatable();
        void setIsRepeatable(bool isRepeatable);
        QString getTaskCommand();
        void setTaskCommand(QString command);
        Task* getTaskNext();
        void setTaskNext(Task* next);
        QJsonObject getTaskParameters();
        void setTaskParameters(QJsonObject jsonObject);
        QDate getStartDate();
        void setStartDate(QDate startDate);
        QDate getEndDate();
        void setEndDate(QDate endDate);
        virtual void execute() {};
        Schedule** getScheduling();
};

#endif // TASK_H
