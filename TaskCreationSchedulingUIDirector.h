#include <TaskCreationSchedulingUIBuilder.h>
#include <Task.h>
#ifndef TASKCREATIONSCHEDULINGUIDIRECTOR_H
#define TASKCREATIONSCHEDULINGUIDIRECTOR_H
class TaskCreationSchedulingUIDirector{
    TaskCreationSchedulingUIBuilder* builder;

    public:
        TaskCreationSchedulingUIDirector(TaskCreationSchedulingUIBuilder* builder){
            this->builder = builder;
        }

        void setBuilder(TaskCreationSchedulingUIBuilder* builder){
            this->builder = builder;
        }

        void buildWindow(){
            builder->initializeWindowStep();
            builder->buildCalendarStep();
            builder->buildDaysStep();
            builder->buildTimeStep();
            builder->buttonStep();
        }
};

#endif // TASKCREATIONSCHEDULINGUIDIRECTOR_H
