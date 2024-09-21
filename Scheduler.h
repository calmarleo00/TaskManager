#include "qprocess.h"
#include "qtimer.h"
#include <Task.h>
#ifndef SCHEDULER_H
#define SCHEDULER_H
class Scheduler : public QObject{
    Q_OBJECT
public:
    static Scheduler* schedulerInstance;
    QProcess* process;
private:
    struct TaskScheduleQueue{
        Task* task;
        TaskScheduleQueue* nextScheduleTask;
        TaskScheduleQueue(Task* task){this->task = task, this->nextScheduleTask = nullptr;}
    };
    TaskScheduleQueue* headTaskQueue = nullptr;
    TaskScheduleQueue* tailTaskQueue = nullptr;
    // Setup scheduler timer so that every midnight it collects the task that needs to be run
    QTime currentTime;
    int msecsToMidnight;
    // Setup the timer for the execution of the first task
    QTimer timerFirstTask;

    private:
        Scheduler(){};
    public:
        void addTaskToQueue(Task* task);
        void removeTaskFromQueue(QString taskName);
        void executeTask();
        void postExecuteTask(Task* task);
        void populateTaskScheduleQueue();
        void updateTimerFirstTask();
        void updateTaskSchedule(Task* task);
        static Scheduler* getInstance();
};

#endif // SCHEDULER_H
