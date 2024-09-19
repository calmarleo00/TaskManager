#ifndef TASKWINDOWUICREATOR_H
#define TASKWINDOWUICREATOR_H
#include "qobject.h"
#include <TaskWindowUI.h>
class TaskWindowUICreator : public QObject{
    Q_OBJECT
    QString taskName;

    public:
        TaskWindowUICreator(){};
        TaskWindowUICreator(QString taskToUpdateName){
            taskName = taskToUpdateName;
        };
        TaskWindowUI* createTaskWindow(QString mode);

};

#endif // TASKWINDOWUICREATOR_H
