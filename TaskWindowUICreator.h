#ifndef TASKWINDOWUICREATOR_H
#define TASKWINDOWUICREATOR_H
#include "qobject.h"
#include <TaskWindowUI.h>
class TaskWindowUICreator : public QObject{
    Q_OBJECT

    TaskWindowUI* createTaskWindow();

};

#endif // TASKWINDOWUICREATOR_H
