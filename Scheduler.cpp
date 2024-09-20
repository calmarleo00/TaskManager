#include "qprocess.h"
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
        timerFirstTask.setSingleShot(true);
        timerFirstTask.start();
    }
    QString str = QString::fromStdString(std::to_string(interval));
    qDebug() << "Intervallo:" + str;
}

void Scheduler::addTaskToQueue(Task* task){
    QDateTime currentDate;
    int currentDay = currentDate.currentDateTime().date().dayOfWeek() - 1;
    if(task->getStartDate() <= currentDate.currentDateTime().date()
        && currentDate.currentDateTime().date() <= task->getEndDate()
        && task->getScheduling()[currentDay]->getIterTime()->time >= currentDate.currentDateTime().time()){
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
        QProcess* process = new QProcess();
        process->connect(process, &QProcess::finished, this, [this, task]{this->postExecuteTask(task);});
        process->start(task->getTaskCommand());
    }
    else{
        task->execute();
        postExecuteTask(task);
    }
}

void Scheduler::postExecuteTask(Task* task){
    updateTaskSchedule(task);
    TaskScheduleQueue* tempTaskPtr = headTaskQueue->nextScheduleTask;
    delete headTaskQueue;
    headTaskQueue = tempTaskPtr;
    addTaskToQueue(task);
}

void Scheduler::updateTaskSchedule(Task* task){
    int day = QDate::currentDate().dayOfWeek() - 1;
    // Controllare se il task è di tipo ripetibile
    if(task->getIsRepeatable()){
        int mscsFromCurrentDay;
        // Controllare se il task si ripete ogni ora o ogni secondo
        if(task->getScheduling()[day]->getRepeatableHours() != -1){
            // Controllare se il tempo in ms del task fino a questo punto con aggiunti i nuovi ms supera il totale di una giornata (i.e. deve essere seguito nei giorni seguenti e non più nel giorno corrente)
            if((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                           (task->getScheduling()[day]->getRepeatableHours() * 3600 * 1000) > 24 * 60 * 60 * 1000){
                // Richiamare il tempo dell'ultima esecuzione del task
                QDateTime currentDateTime(QDate::currentDate(), task->getScheduling()[day]->getIterTime()->time);
                // Calcolare quale sarebbe la nuova data considerato il tempo da aggiungere
                QDateTime futureDateTime = currentDateTime.addSecs(task->getScheduling()[day]->getRepeatableHours() * 3600);
                // Calcolare il nuovo giorno in cui il task dovrà essere eseguito
                int newDay = futureDateTime.date().dayOfWeek() - 1;
                task->getScheduling()[newDay]->setDay(newDay);
                // Impostare il tempo di quel giorno ai msecs dall'inizio di quel giorno
                task->getScheduling()[newDay]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(futureDateTime.time().msecsSinceStartOfDay())));
                task->getScheduling()[newDay]->setIterTime(task->getScheduling()[newDay]->getStartTime());
            }
            else{
                // Sommare il tempo trascorso oggi + il tempo da ripetere
                mscsFromCurrentDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableHours() * 3600 * 1000));
                // Impostare il tempo al nuovo risutato calcolato
                task->getScheduling()[day]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromCurrentDay)));
                task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getStartTime());
            }
        }
        // Ragionamento simile per quanto riguarda il caso in cui siano i secondi ad essere ripetibili
        else if(task->getScheduling()[day]->getRepeatableSeconds() != -1){
            if((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                           (task->getScheduling()[day]->getRepeatableSeconds() * 1000) > 24 * 60 * 60 * 1000){
                QDateTime currentDateTime(QDate::currentDate(), task->getScheduling()[day]->getIterTime()->time);
                mscsFromCurrentDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableSeconds() * 1000) - (24 * 60 * 60 * 1000));
                QDateTime futureDateTime = currentDateTime.addSecs(task->getScheduling()[day]->getRepeatableSeconds());
                int newDay = futureDateTime.date().dayOfWeek() - 1;
                task->getScheduling()[newDay]->setDay(newDay);
                task->getScheduling()[newDay]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromCurrentDay)));
                task->getScheduling()[newDay]->setIterTime(task->getScheduling()[newDay]->getStartTime());
            }
            else{
                mscsFromCurrentDay = ((task->getScheduling()[day]->getIterTime()->time.msecsSinceStartOfDay()) +
                                    (task->getScheduling()[day]->getRepeatableSeconds() * 1000));
                task->getScheduling()[day]->setStartTime(new Schedule::Time(QTime::fromMSecsSinceStartOfDay(mscsFromCurrentDay)));
                task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getStartTime());
            }
        }
    }
    else if(!task->getIsRepeatable()){
        if(task->getScheduling()[day]->getIterTime()->nextTime){
            task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getIterTime()->nextTime);
        }
        else{
            task->getScheduling()[day]->setIterTime(task->getScheduling()[day]->getStartTime());
        }
    }
}

void Scheduler::removeTaskFromQueue(QString taskName){
    if(headTaskQueue){
        if(headTaskQueue->task->getTaskName() == taskName){
            TaskScheduleQueue* tempTaskPtr = headTaskQueue->nextScheduleTask;
            delete headTaskQueue;
            headTaskQueue = tempTaskPtr;
            updateTimerFirstTask();
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
