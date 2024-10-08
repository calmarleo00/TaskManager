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
#include <TaskWindowUICreator.h>
AppController* AppController::instance = nullptr;


AppController* AppController::getInstance() {
    if(instance == nullptr) {
        instance = new AppController();
    }
    return instance;
}

/* Crea una nuova attività.
 * Se esiste già un'attività con lo stesso nome, non viene creato un nuovo oggetto.
 * Utilizza la griglia attributesGrid per ottenere i parametri dell'attività, come nome e descrizione.
 * Se l'attività è "esterna", crea una nuova istanza di Task, altrimenti istanzia un oggetto di tipo specifico in base alle classi inserite. */
void AppController::createNewTask(QGridLayout* attributesGrid, bool isExternal){
    QString taskName = qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->toPlainText();
    Task* newTask = taskHead;
    bool taskAlreadyExist = false;
    while(newTask && !taskAlreadyExist){
        if(newTask->getTaskName() == taskName){
            taskAlreadyExist = true;
            break;
        }
        newTask = newTask->getTaskNext();
    }
    if(!taskAlreadyExist){
        if(isExternal){
            tmpTask = new Task();
        }
        else{
            if(taskName == "Task2"){
                tmpTask = new Task2();
            }
        }
        setTaskAttributes(tmpTask, attributesGrid, isExternal);
    }
}

void AppController::setTaskAttributes(Task* newTask, QGridLayout* attributesGrid, bool isExternal){
    // Update in-memory data structure, keeping references of all the existent tasks
    if(taskHead){
        taskTail->setTaskNext(newTask);
        taskTail = newTask;
    }
    else{
        taskHead = newTask;
        taskTail = newTask;
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
            QJsonObject attributeDetail;
            if(type->currentText() == "string"){
                attributeDetail["type"] = "string";
                QTextEdit* textValue = qobject_cast<QTextEdit*>(attributesGrid->itemAt(index + 2)->widget());
                attributeDetail["value"] = textValue->toPlainText();
                jsonObject[name->text()] = attributeDetail;
            }
            else if(type->currentText() == "int"){
                attributeDetail["type"] = "int";
                QSpinBox* numValue = qobject_cast<QSpinBox*>(attributesGrid->itemAt(index + 2)->widget());
                attributeDetail["value"] = numValue->value();
                jsonObject[name->text()] = attributeDetail;
            }
            else{
                attributeDetail["type"] = "float";
                QDoubleSpinBox* numValue = qobject_cast<QDoubleSpinBox*>(attributesGrid->itemAt(index + 2)->widget());
                attributeDetail["value"] = numValue->value();
                jsonObject[name->text()] = attributeDetail;
            }
        }
        newTask->setTaskParameters(jsonObject);
    }
}


void AppController::setTaskFixedScheduleValues(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startEdit, QDateEdit* endDateEdit){
    taskTail->setIsRepeatable(false);
    taskTail->setStartDate(startEdit->date());
    taskTail->setEndDate(endDateEdit->date());
    for(int day = 0; day < 7; day++){
        if(daysCheckBoxes[day].isChecked()){
            taskTail->getScheduling()[day]->setDay(day);
            for(int time = 0; time < timeContainerLayout[day].count(); time++){
                if(time == 0){
                    taskTail->getScheduling()[day]->setStartTime(new Schedule::Time(qobject_cast<QTimeEdit*>(timeContainerLayout[day].itemAt(time)->widget())->time()));
                    taskTail->getScheduling()[day]->setIterTime(taskTail->getScheduling()[day]->getStartTime());
                }
                else{
                    taskTail->getScheduling()[day]->getIterTime()->nextTime = new Schedule::Time(qobject_cast<QTimeEdit*>(timeContainerLayout[day].itemAt(time)->widget())->time());
                    taskTail->getScheduling()[day]->setIterTime(taskTail->getScheduling()[day]->getIterTime()->nextTime);
                }
            }
            taskTail->getScheduling()[day]->setIterTime(taskTail->getScheduling()[day]->getStartTime());
        }
        else{
            delete taskTail->getScheduling()[day];
            taskTail->getScheduling()[day] = new Schedule();
        }

    }
}

void AppController::setTaskRepeatableScheduleValues(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount){
    QDate startDate = startDateEdit->date();
    int day = startDate.dayOfWeek() - 1;
    taskTail->setIsRepeatable(true);
    taskTail->setStartDate(startDate);
    taskTail->setEndDate(endDateEdit->date());
    taskTail->getScheduling()[day]->setStartTime(new Schedule::Time(startTime->time()));
    taskTail->getScheduling()[day]->setIterTime(taskTail->getScheduling()[day]->getStartTime());
    taskTail->getScheduling()[day]->setDay(day);
    for(int i = 0; i < 7; i++){
        if(hourCheckBox->isChecked()){
            taskTail->getScheduling()[i]->setRepeatableHours(repeatableAmount->value());
            taskTail->getScheduling()[i]->setRepeatableSeconds(-1);
        }
        else{
            taskTail->getScheduling()[i]->setRepeatableHours(-1);
            taskTail->getScheduling()[i]->setRepeatableSeconds(repeatableAmount->value());
        }
    }
}

/* Salva l'attività attualmente puntata da taskTail nel database.
 * Crea una voce nella tabella Task e inserisce anche le informazioni di pianificazione nella tabella Schedule. */
void AppController::saveTaskToDatabase(){
    QSqlDatabase db = QSqlDatabase::database("ticare_connection");

    QJsonDocument jsonDocument(taskTail->getTaskParameters());

    if(db.open()){
        QSqlQuery query(db);
        query.prepare("INSERT INTO Task (id, description, parameters, isExternal, isRepeatable, command, startDate, endDate) VALUES (:name, :description, :parameters, :isExternal, :isRepeatable, :command, :startDate, :endDate);");
        query.bindValue(":name", taskTail->getTaskName());
        query.bindValue(":description", taskTail->getTaskDescription());
        QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
        query.bindValue(":parameters", jsonString);
        query.bindValue(":isExternal", taskTail->getIsExternal());
        query.bindValue(":isRepeatable", taskTail->getIsRepeatable());
        query.bindValue("command", taskTail->getTaskCommand());
        query.bindValue(":startDate", taskTail->getStartDate());
        query.bindValue(":endDate", taskTail->getEndDate());
        query.exec();
        query.clear();


        Schedule** schedule = taskTail->getScheduling();
        QVariant scheduleId = 0;

        for(int i = 0; i < 7; i++){
            if(schedule[i] && schedule[i]->getDay() != -1){
                query.prepare("INSERT INTO Schedule (taskId, day, repeatableHours, repeatableSeconds) VALUE (:taskId, :day, :repeatableHours, :repeatableSeconds);");
                query.bindValue(":taskId", taskTail->getTaskName());
                query.bindValue(":day", i + 1);
                query.bindValue(":repeatableHours", schedule[i]->getRepeatableHours());
                query.bindValue(":repeatableSeconds", schedule[i]->getRepeatableSeconds());
                query.exec();
                scheduleId = query.lastInsertId();
                schedule[i]->setIdDatabase(scheduleId.toInt());
                query.clear();

                Schedule::Time* iterTime = schedule[i]->getStartTime();
                while(iterTime){
                    query.prepare("INSERT INTO Time (scheduleId, time) VALUE (:scheduleId, :time);");
                    query.bindValue(":scheduleId", scheduleId.value<int>());
                    query.bindValue(":time", iterTime->time);
                    query.exec();
                    iterTime = iterTime->nextTime;
                }

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

/* Aggiorna una determinata attività nel database.
 * Viene aggiornata la tabella Task con i nuovi parametri dell'attività, così come la tabella Schedule con le nuove informazioni di pianificazione. */
void AppController::updateTaskInDatabase(Task* task){
    QSqlDatabase db = QSqlDatabase::database("ticare_connection");

    QJsonDocument jsonDocument(task->getTaskParameters());

    if(db.open()){
        QSqlQuery query(db);
        query.prepare("UPDATE Task SET description = :description, parameters = :parameters, isExternal = :isExternal, isRepeatable = :isRepeatable, command = :command, startDate = :startDate, endDate = :endDate WHERE id = :name;");
        query.bindValue(":name", task->getTaskName());
        query.bindValue(":description", task->getTaskDescription());
        QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
        query.bindValue(":parameters", jsonString);
        query.bindValue(":isExternal", task->getIsExternal());
        query.bindValue(":isRepeatable", task->getIsRepeatable());
        query.bindValue("command", task->getTaskCommand());
        query.bindValue(":startDate", task->getStartDate());
        query.bindValue(":endDate", task->getEndDate());
        query.exec();
        query.clear();


        Schedule** schedule = task->getScheduling();

        for(int i = 0; i < 7; i++){
            if(schedule[i] && schedule[i]->getDay() != -1){
                query.prepare("UPDATE Schedule SET taskId = :taskId, day = :day, repeatableHours = :repeatableHours, repeatableSeconds = :repeatableSeconds WHERE id = :id;");
                query.bindValue(":id", schedule[i]->getIdDatabase());
                query.bindValue(":taskId", task->getTaskName());
                query.bindValue(":day", i + 1);
                query.bindValue(":repeatableHours", schedule[i]->getRepeatableHours());
                query.bindValue(":repeatableSeconds", schedule[i]->getRepeatableSeconds());
                query.exec();
                query.clear();
                Schedule::Time* iterTime = schedule[i]->getStartTime();
                while(iterTime){
                    query.prepare("UPDATE Time SET scheduleId = :scheduleId, time = :time WHERE scheduleId = :scheduleId;");
                    query.bindValue(":scheduleId", schedule[i]->getIdDatabase());
                    query.bindValue(":time", schedule[i]->getStartTime()->time);
                    query.exec();
                    iterTime = iterTime->nextTime;
                }
            }
            // Se il task era stato impostato per essere eseguito in un certo giorno e l'esecuzione viene annullata nella modifica, bisogna essere certi che l'entry corrispondente sia eliminata
            else{
                query.prepare("DELETE FROM Schedule WHERE taskId = :taskId AND day = :day;");
                query.bindValue(":taskId", task->getTaskName());
                query.bindValue(":day", i + 1);
                query.exec();
                query.clear();
            }
        }
    }
}




