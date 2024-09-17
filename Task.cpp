#include <Task.h>

Task::Task(){
    name = "default";
    this->nextTask = nullptr;
    for(int i = 0; i < 7; i++){
        scheduling[i] = new Schedule();
    }
}

Task::Task(QString name)
{
    this->name = name;
    this->nextTask = nullptr;
}

QString Task::getTaskName(){
    return this->name;
}

void Task::setTaskName(QString name){
    this->name = name;
}

QString Task::getTaskDescription(){
    return this->description;
}

void Task::setTaskDescription(QString description){
    this->description = description;
}

bool Task::getIsExternal(){
    return this->isExternal;
}

void Task::setIsExternal(bool isExternal){
    this->isExternal = isExternal;
}

QString Task::getTaskCommand(){
    return this->command;
}

void Task::setTaskCommand(QString command){
    this->command = command;
}

QJsonObject Task::getTaskParameters(){
    return this->parameters;
}

void Task::setTaskParameters(QJsonObject jsonObject){
    this->parameters = jsonObject;
}

Task* Task::getTaskNext(){
    return nextTask;
}

void Task::setTaskNext(Task* nextTask){
    this->nextTask = nextTask;
}


QDate Task::getStartDate(){
    return this->startDate;
}

void Task::setStartDate(QDate startDate){
    this->startDate = startDate;
}

QDate Task::getEndDate(){
    return this->endDate;
}

void Task::setEndDate(QDate endDate){
    this->endDate = endDate;
}

Schedule** Task::getScheduling(){
    return this->scheduling;
}
