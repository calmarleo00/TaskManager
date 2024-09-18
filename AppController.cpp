#include "qcheckbox.h"
#include "qcombobox.h"
#include "qdatetimeedit.h"
#include "qjsondocument.h"
#include "qlineedit.h"
#include "qspinbox.h"
#include "qsqldatabase.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qtextedit.h"
#include <AppController.h>
#include <Scheduler.h>
#include <Task2.cpp>
AppController* AppController::instance = nullptr;


AppController* AppController::getInstance() {
    if(instance == nullptr) {
        instance = new AppController();
    }
    return instance;
}


Task* AppController::createNewTask(QString taskName, bool isExternal){
    Task* newTask = nullptr;
    if(isExternal){
        newTask = new Task();
    }
    else{
        if(taskName == "Task2"){
            newTask = new Task2();
        }
    }
    return newTask;
}

void AppController::setTaskAttributes(QGridLayout* attributesGrid, bool isExternal){
    Task* newTask = createNewTask(qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->toPlainText(), isExternal);

    // Update in-memory data structure, keeping references of all the existent tasks
    if(taskHead){
        taskTail->setTaskNext(newTask);
        taskTail = newTask;
    }
    else{
        taskHead = newTask;
        taskTail = newTask;
        taskIter = newTask;
    }

    // Set basic information
    newTask->setTaskName(qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->toPlainText());
    newTask->setTaskDescription(qobject_cast<QTextEdit*>(attributesGrid->itemAt(5)->widget())->toPlainText());
    newTask->setIsExternal(isExternal);
    // If it is external there is only one attribute to store, otherwise save the remaining attributes in a json object
    if(isExternal){
        newTask->setTaskCommand(qobject_cast<QTextEdit*>(attributesGrid->itemAt(8)->widget())->toPlainText());
    }
    else{
        newTask->setTaskCommand("");
        if (!jsonObject.isEmpty()){
            jsonObject = QJsonObject();
        }
        for(int index = 6; index < attributesGrid->rowCount() * attributesGrid->columnCount() - 1; index+=3){
            QLineEdit* name = qobject_cast<QLineEdit*>(attributesGrid->itemAt(index)->widget());
            QComboBox* type = qobject_cast<QComboBox*>(attributesGrid->itemAt(index + 1)->widget());
            if(type->currentText() == "string"){
                QTextEdit* textValue = qobject_cast<QTextEdit*>(attributesGrid->itemAt(index + 2)->widget());
                jsonObject[name->text()] = textValue->toPlainText();
            }
            else if(type->currentText() == "int"){
                QSpinBox* numValue = qobject_cast<QSpinBox*>(attributesGrid->itemAt(index + 2)->widget());
                jsonObject[name->text()] = numValue->value();
            }
            else{
                QDoubleSpinBox* numValue = qobject_cast<QDoubleSpinBox*>(attributesGrid->itemAt(index + 2)->widget());
                jsonObject[name->text()] = numValue->value();
            }
        }

        newTask->setTaskParameters(jsonObject);
    }
}


void AppController::setTaskRepeatableScheduleValues(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QVBoxLayout* checkBoxLayout, QHBoxLayout* startTimeContainerLayout, QHBoxLayout* repeatableContainerLayout){
    QDate startDate = qobject_cast<QDateEdit*>(dateContainerLayout->itemAt(1)->widget())->date();
    int day = startDate.dayOfWeek() - 1;
    taskTail->setStartDate(startDate);
    taskTail->setEndDate(qobject_cast<QDateEdit*>(dateContainerLayout->itemAt(3)->widget())->date());
    taskTail->getScheduling()[day]->setStartTime(new Schedule::Time(qobject_cast<QTimeEdit*>(startTimeContainerLayout->itemAt(1)->widget())->time()));
    taskTail->getScheduling()[day]->setIterTime(taskTail->getScheduling()[day]->getStartTime());
    taskTail->getScheduling()[day]->setDay(day);
    for(int i = 0; i < 7; i++){

        taskTail->getScheduling()[i]->setIsRepeatable(true);
        if(qobject_cast<QCheckBox*>(checkBoxLayout->itemAt(0)->widget())->isChecked()){
            taskTail->getScheduling()[i]->setRepeatableHours(qobject_cast<QSpinBox*>(repeatableContainerLayout->itemAt(1)->widget())->value());
        }
        else{
            taskTail->getScheduling()[i]->setRepeatableSeconds(qobject_cast<QSpinBox*>(repeatableContainerLayout->itemAt(1)->widget())->value());
        }
    }
    /*for(int i = 0; i < 7; i++){
        QCheckBox* dayCheckBox = qobject_cast<QCheckBox*>(daysLayout->itemAt(i)->widget());
        if(dayCheckBox->isChecked()){
            taskTail->getScheduling()[i]->setIsRepeatable(true);
            taskTail->getScheduling()[i]->setStartTime(new Schedule::Time(qobject_cast<QTimeEdit*>(startTimeContainerLayout->itemAt(1)->widget())->time()));
            taskTail->getScheduling()[i]->setIterTime(taskTail->getScheduling()[i]->getStartTime());
            qDebug() << taskTail->getScheduling()[i]->getIterTime();
            if(qobject_cast<QCheckBox*>(checkBoxLayout->itemAt(0)->widget())->isChecked()){
                taskTail->getScheduling()[i]->setRepeatableHours(qobject_cast<QSpinBox*>(repeatableContainerLayout->itemAt(1)->widget())->value());
            }
            else{
                taskTail->getScheduling()[i]->setRepeatableSeconds(qobject_cast<QSpinBox*>(repeatableContainerLayout->itemAt(1)->widget())->value());
            }
        }
        else{
            taskTail->getScheduling()[i] = nullptr;
        }
    }*/
}


void AppController::saveTaskToDatabase(){
    QSqlDatabase db = QSqlDatabase::database("ticare_connection");

    QJsonDocument jsonDocument(taskTail->getTaskParameters());

    if(db.open()){
        QSqlQuery query(db);
        query.prepare("INSERT INTO Task (id, description, parameters, isExternal, command, startDate, endDate) VALUES (:name, :description, :parameters, :isExternal, :command, :startDate, :endDate);");
        query.bindValue(":name", taskTail->getTaskName());
        query.bindValue(":description", taskTail->getTaskDescription());
        QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
        query.bindValue(":parameters", jsonString);
        query.bindValue(":isExternal", taskTail->getIsExternal());
        query.bindValue("command", taskTail->getTaskCommand());
        query.bindValue(":startDate", taskTail->getStartDate());
        query.bindValue(":endDate", taskTail->getEndDate());
        query.exec();
        query.clear();


        Schedule** schedule = taskTail->getScheduling();
        QVariant scheduleId = 0;

        for(int i = 0; i < 7; i++){
            if(schedule[i] && schedule[i]->getDay() != -1){
                query.prepare("INSERT INTO Schedule (taskId, day, isRepeatable, repeatableHours, repeatableSeconds) VALUE (:taskId, :day, :isRepeatable, :repeatableHours, :repeatableSeconds);");
                query.bindValue(":taskId", taskTail->getTaskName());
                query.bindValue(":day", i + 1);
                query.bindValue(":isRepeatable", true);
                query.bindValue(":repeatableHours", schedule[i]->getRepeatableHours());
                query.bindValue(":repeatableSeconds", schedule[i]->getRepeatableSeconds());
                query.exec();
                scheduleId = query.lastInsertId();
                query.clear();

                query.prepare("INSERT INTO Time (scheduleId, time) VALUE (:scheduleId, :time);");
                query.bindValue(":scheduleId", scheduleId.value<int>());
                query.bindValue(":time", schedule[i]->getStartTime()->time);
                query.exec();
            }
        }
    }
}

void AppController::deleteTask(QString taskName){
    Scheduler::getInstance()->removeTaskFromQueue(taskName);
    if(taskHead->getTaskName() == taskName){
        Task* taskHeadTmpPtr = taskHead->getTaskNext();
        delete taskHead;
        taskHead = taskHeadTmpPtr;
    }
    else{
        Task* iterFirstTask = taskHead;
        Task* iterSecondTask = taskHead->getTaskNext();
        while(iterSecondTask){
            if(iterSecondTask->getTaskName() == taskName){
                iterFirstTask->setTaskNext(iterSecondTask->getTaskNext());
                delete iterSecondTask;
                break;
            }
            iterFirstTask = iterSecondTask;
            iterSecondTask = iterSecondTask->getTaskNext();
        }
    }
    QSqlDatabase db = QSqlDatabase::database("ticare_connection");
    if(db.open()){
        QSqlQuery query(db);
        query.prepare("DELETE FROM Task WHERE id = :name");
        query.bindValue(":name", taskName);
        query.exec();
    }
}

void AppController::callAddTaskToSchedule(){
    Scheduler::getInstance()->addTaskToQueue(taskTail);
}

