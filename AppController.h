#include "qcheckbox.h"
#include "qdatetimeedit.h"
#include "qjsonobject.h"
#include "qspinbox.h"
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
    Task* tmpTask;
    // Define a pointer that let iterate when necessary through the list of tasks
    QJsonObject jsonObject;
    AppController(){};
    virtual ~AppController(){};

    void createNewTask(QGridLayout* attributesGrid, bool isExternal);
    void setTaskAttributes(Task* newTask, QGridLayout* attributesGrid, bool isExternal);
    void saveTaskToDatabase();
    void updateTaskInDatabase(Task* task);
    void setTaskRepeatableScheduleValues(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount);
    void setTaskFixedScheduleValues(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startDateEdit, QDateEdit* endDateEdit);
    void callAddTaskToSchedule();
    void deleteTask(QString taskName);

    static AppController* getInstance();
};

#endif // APPCONTROLLER_H
