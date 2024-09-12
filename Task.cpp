#include <Task.h>

Task::Task(){
    name = "default";
    this->next = nullptr;
}
Task::Task(QString name)
{
    this->name = name;
    this->next = nullptr;
}
QString Task::getTaskName(){
    return name;
}

void Task::setTaskName(QString name){
    this->name = name;
}

Task* Task::getTaskNext(){
    return next;
}

void Task::setTaskNext(Task* next){
    this->next = next;
}

