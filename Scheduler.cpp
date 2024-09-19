#include <Scheduler.h>
#include <winsock.h>
#include <AppController.h>

Scheduler* Scheduler::schedulerInstance = nullptr;

Scheduler* Scheduler::getInstance(){
    if(schedulerInstance == nullptr){
        schedulerInstance = new Scheduler();
        schedulerInstance->currentTime = QTime::currentTime();
        QTime midnight(0, 0, 0);
        schedulerInstance->msecsToMidnight = schedulerInstance->currentTime.msecsTo(midnight);
        if (schedulerInstance->msecsToMidnight <= 0) {
            // If it is already past midnight, adjust the time to the next day's midnight
            schedulerInstance->msecsToMidnight += 24 * 60 * 60 * 1000;
        }
        QTimer::singleShot(schedulerInstance->msecsToMidnight, schedulerInstance, &Scheduler::populateTaskScheduleQueue);
        schedulerInstance->timerFirstTask.connect(&schedulerInstance->timerFirstTask, &QTimer::timeout, schedulerInstance, &Scheduler::executeTask);

    }
    return schedulerInstance;
}

void Scheduler::updateTimerFirstTask(){
    if(timerFirstTask.isActive()){
        timerFirstTask.stop();
    }

    int day = QDate::currentDate().dayOfWeek() - 1;
    QTime firstTaskTime = headTaskQueue->task->getScheduling()[day]->getIterTime()->time;
    int interval = QTime::currentTime().msecsTo(firstTaskTime);
    if(interval <= 0){
        executeTask();
    }
    else{
        timerFirstTask.setInterval(interval);
        timerFirstTask.start();
    }
    QString str = QString::fromStdString(std::to_string(interval));
    qDebug() << "Intervallo:" + str;
}

void Scheduler::addTaskToQueue(Task* task){
    QDateTime currentDate;
    int currentDay = currentDate.currentDateTime().date().dayOfWeek() - 1;
    if(task->getStartDate() <= currentDate.currentDateTime().date() && currentDate.currentDateTime().date() <= task->getEndDate()){
        if(headTaskQueue && tailTaskQueue){
            TaskScheduleQueue* firstPairWindow = headTaskQueue;
            TaskScheduleQueue* secondPairWindow = headTaskQueue->nextScheduleTask;
            // If the new task is the first to be executed today it needs to be appended at the beginning of the queue
            if(task->getScheduling()[currentDay]->getIterTime()->time <= firstPairWindow->task->getScheduling()[currentDay]->getIterTime()->time){
                TaskScheduleQueue* newTaskInQueue = new TaskScheduleQueue(task);
                newTaskInQueue->nextScheduleTask = headTaskQueue;
                headTaskQueue = newTaskInQueue;
                schedulerInstance->updateTimerFirstTask();
            }
            // Otherwise lets iterate to fine where to add the new task based on its start time and the others actual iter time
            while(firstPairWindow->nextScheduleTask){
                if(firstPairWindow->task->getScheduling()[currentDay]->getIterTime()->time <= task->getScheduling()[currentDay]->getIterTime()->time
                    && secondPairWindow->task->getScheduling()[currentDay]->getIterTime()->time >= task->getScheduling()[currentDay]->getIterTime()->time){
                    TaskScheduleQueue* newTaskInQueue = new TaskScheduleQueue(task);
                    firstPairWindow->nextScheduleTask = newTaskInQueue;
                    newTaskInQueue->nextScheduleTask = secondPairWindow;
                    break;
                }
                // If the end of the queue is reached, and the task has not been inserted yet, then verify if can be appended at the end of the queue
                if(!secondPairWindow->nextScheduleTask && secondPairWindow->task->getScheduling()[currentDay]->getIterTime()->time <= task->getScheduling()[currentDay]->getIterTime()->time){
                    TaskScheduleQueue* newTaskInQueue = new TaskScheduleQueue(task);
                    secondPairWindow->nextScheduleTask = newTaskInQueue;
                    tailTaskQueue = newTaskInQueue;
                }
                firstPairWindow = secondPairWindow;
                secondPairWindow = secondPairWindow->nextScheduleTask;
            }
        }

        else if(headTaskQueue){
            if(headTaskQueue->task->getScheduling()[currentDay]->getIterTime()->time <= task->getScheduling()[currentDay]->getIterTime()->time){
                headTaskQueue->nextScheduleTask = new TaskScheduleQueue(task);
                tailTaskQueue = headTaskQueue->nextScheduleTask;
            }
            else{
                tailTaskQueue = headTaskQueue;
                headTaskQueue = new TaskScheduleQueue(task);
                headTaskQueue->nextScheduleTask = tailTaskQueue;
                schedulerInstance->updateTimerFirstTask();
            }

        }
        else{
            headTaskQueue = new TaskScheduleQueue(task);
            schedulerInstance->updateTimerFirstTask();
        }
    }

    TaskScheduleQueue* iterTaskQueue = headTaskQueue;
    while(iterTaskQueue){
        qDebug() << iterTaskQueue->task->getTaskName();
        task->getScheduling()[currentDay]->getStartTime();
        iterTaskQueue = iterTaskQueue->nextScheduleTask;
    }
}

void Scheduler::populateTaskScheduleQueue(){
    schedulerInstance->msecsToMidnight += 24 * 60 * 60 * 1000;
    QTimer::singleShot(schedulerInstance->msecsToMidnight, schedulerInstance, &Scheduler::populateTaskScheduleQueue);
    Task* iterTask = AppController::getInstance()->taskHead;
    QDate currentDate = QDate::currentDate();
    while(iterTask){
        if(iterTask->getStartDate() <= currentDate && iterTask->getEndDate() >= currentDate){
            if(iterTask->getScheduling()[currentDate.dayOfWeek() - 1] && iterTask->getScheduling()[currentDate.dayOfWeek() - 1]->getDay() != -1){
                schedulerInstance->addTaskToQueue(iterTask);
            }
        }
        iterTask = iterTask->getTaskNext();
    }
}

void Scheduler::executeTask(){
    Task* task = headTaskQueue->task;
    qDebug() << task->getIsExternal();
    if(task->getIsExternal()){

    }
    else{
        task->execute();
        updateTaskSchedule(task);
        TaskScheduleQueue* tempTaskPtr = headTaskQueue->nextScheduleTask;
        delete headTaskQueue;
        headTaskQueue = tempTaskPtr;
        addTaskToQueue(task);
    }
}

void Scheduler::updateTaskSchedule(Task* task){
    int day = QDate::currentDate().dayOfWeek() - 1;
    int nextDay = 0;
    if(day == 6){
        nextDay = 0;
    }
    else{
        nextDay = day + 1;
    }
    if(task->getIsRepeatable()){
        int mscsFromStartDay;
        if(task->getScheduling()[day]->getRepeatableHours() != -1){
            if((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                           (task->getScheduling()[day]->getRepeatableHours() * 3600 * 1000) > 24 * 60 * 60 * 1000){
                mscsFromStartDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                        (task->getScheduling()[day]->getRepeatableHours() * 3600 * 1000) - (24 * 60 * 60 * 1000));
                task->getScheduling()[nextDay]->setDay(nextDay);
                task->getScheduling()[nextDay]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromStartDay)));
                task->getScheduling()[nextDay]->setIterTime(task->getScheduling()[nextDay]->getStartTime());
            }
            else{
                mscsFromStartDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableHours() * 3600 * 1000));
                task->getScheduling()[day]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromStartDay)));
                task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getStartTime());
            }
        }
        else if(task->getScheduling()[day]->getRepeatableSeconds() != -1){
            if((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                           (task->getScheduling()[day]->getRepeatableSeconds() * 1000) > 24 * 60 * 60 * 1000){
                mscsFromStartDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableSeconds() * 1000) - (24 * 60 * 60 * 1000));
                task->getScheduling()[nextDay]->setDay(nextDay);
                task->getScheduling()[nextDay]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromStartDay)));
                task->getScheduling()[nextDay]->setIterTime(task->getScheduling()[nextDay]->getStartTime());
            }
            else{
                mscsFromStartDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableSeconds() * 1000));
                task->getScheduling()[day]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromStartDay)));
                task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getStartTime());
            }
        }
    }
}

void Scheduler::removeTaskFromQueue(QString taskName){
    if(headTaskQueue){
        if(headTaskQueue->task->getTaskName() == taskName){
            TaskScheduleQueue* tempTaskPtr = headTaskQueue->nextScheduleTask;
            delete headTaskQueue;
            headTaskQueue = tempTaskPtr;
        }
        else{
            TaskScheduleQueue* iterFirstTask = headTaskQueue;
            TaskScheduleQueue* iterSecondTask = headTaskQueue->nextScheduleTask;
            while(iterSecondTask){
                if(iterSecondTask->task->getTaskName() == taskName){
                    iterFirstTask->nextScheduleTask = iterSecondTask->nextScheduleTask;
                    delete iterSecondTask;
                    break;
                }
                iterFirstTask = iterSecondTask;
                iterSecondTask = iterSecondTask->nextScheduleTask;
            }
        }
    }
}
