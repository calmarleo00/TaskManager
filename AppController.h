#include "qjsonobject.h"
#include<QWidget>
#include<Task.h>
#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
class AppController : public QWidget{
    public:
        static AppController* instance;
    // Define two pointers to the first and last task in the list
    Task* taskHead = nullptr;
    Task* taskTail = nullptr;
    // Define a pointer that let iterate when necessary through the list of tasks
    Task* taskIter = nullptr;
    QJsonObject jsonObject;
    AppController(){};
    virtual ~AppController(){};

    void createNewTask(QGridLayout* attributesGrid, bool isExternal);
    void saveTaskToDatabase();
    void setTaskRepeatableScheduleValues(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QHBoxLayout* hoursGroupLayout, QVBoxLayout* checkBoxLayout);
    public slots:
        void addNewTask(Task* newTask);

    static AppController* getInstance();
};

#endif // APPCONTROLLER_H
