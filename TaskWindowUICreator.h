#ifndef TASKWINDOWUICREATOR_H
#define TASKWINDOWUICREATOR_H
#include "qobject.h"
#include <TaskWindowUI.h>
/* La classe è una factory che gestisce la creazione delle finestre di interfaccia utente (UI) per la gestione dei task,
 * sia per la creazione di nuovi task che per l'aggiornamento di task esistenti.
 * A seconda della modalità (create o update), la fabbrica genera l'istanza appropriata di TaskWindowUI. */
class TaskWindowUICreator : public QObject{
    Q_OBJECT
    QString taskName;

    public:
        TaskWindowUICreator(){};
        TaskWindowUICreator(QString taskToUpdateName){
            taskName = taskToUpdateName;
        };
        /* Il metodo principale della classe, che decide quale tipo di finestra creare in base al valore della stringa mode */
        TaskWindowUI* createTaskWindow(QString mode);

};

#endif // TASKWINDOWUICREATOR_H
