#include <TaskCreationSchedulingUIBuilder.h>
#ifndef TASKFIXEDSCHEDULINGUIBUILDER_H
#define TASKFIXEDSCHEDULINGUIBUILDER_H
class TaskFixedSchedulingUIBuilder : public TaskCreationSchedulingUIBuilder{
    QWidget* taskFixedSchedulingWidget;

    public:
        QWidget* getResult(){
            return taskFixedSchedulingWidget;
        }
    signals:
        void emitBackSchedulingSelectionSignal(){
            emit backPageSignal();
        }

        /*void emitClosePageSignal(){
            emit closePageSignal(NULL, NULL, NULL, NULL, NULL);
        }*/
};

#endif // TASKFIXEDSCHEDULINGUIBUILDER_H
