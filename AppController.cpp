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
#include<AppController.h>

AppController* AppController::instance = nullptr;


AppController* AppController::getInstance() {
    if(instance == nullptr) {
        instance = new AppController();
    }
    return instance;
}

void AppController::createNewTask(QGridLayout* attributesGrid, bool isExternal){
    Task* newTask = new Task();

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


void AppController::setTaskRepeatableScheduleValues(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QHBoxLayout* hoursGroupLayout, QVBoxLayout* checkBoxLayout){
    taskTail->setStartDate(qobject_cast<QDateEdit*>(dateContainerLayout->itemAt(1)->widget())->date());
    taskTail->setEndDate(qobject_cast<QDateEdit*>(dateContainerLayout->itemAt(3)->widget())->date());
    for(int i = 0; i < 7; i++){
        QCheckBox* dayCheckBox = qobject_cast<QCheckBox*>(daysLayout->itemAt(i)->widget());
        if(dayCheckBox->isChecked()){
            taskTail->getScheduling()[i]->setIsRepeatable(true);
            if(qobject_cast<QCheckBox*>(checkBoxLayout->itemAt(0)->widget())->isChecked()){
                taskTail->getScheduling()[i]->setRepeatableHours(qobject_cast<QSpinBox*>(hoursGroupLayout->itemAt(1)->widget())->value());
            }
            else{
                taskTail->getScheduling()[i]->setRepeatableSeconds(qobject_cast<QSpinBox*>(hoursGroupLayout->itemAt(1)->widget())->value());
            }
        }
        else{
            taskTail->getScheduling()[i] = nullptr;
        }
    }
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
        query.clear();

        Schedule** schedule = taskTail->getScheduling();
        for(int i = 0; i < 7; i++){
            if(schedule[i]){
                query.prepare("INSERT INTO Schedule (taskId, day, isRepeatable, repeatableHours, repeatableSeconds) VALUE (:taskId, :day, :isRepeatable, :repeatableHours, :repeatableSeconds);");
                query.bindValue(":taskId", taskTail->getTaskName());
                query.bindValue(":day", i + 1);
                query.bindValue(":isRepeatable", true);
                query.bindValue(":repeatableHours", schedule[i]->getRepeatableHours());
                query.bindValue(":repeatableSeconds", schedule[i]->getRepeatableSeconds());
                query.clear();
            }
        }
    }

}

