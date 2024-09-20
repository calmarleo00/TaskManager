#include <TaskWindowUICreator.h>
#include <TaskUpdateWindowUIDecorator.h>
#include <TaskCreationWindowUI.h>
#include <MainWindowUI.h>


/* Esistono due modalità:
 *   - create: Crea una nuova finestra di creazione del task utilizzando TaskCreationWindowUI.
 *             Configura l'interfaccia utente chiamando setupTaskWidgetsUI().
 *             Collega il segnale signalAddNewTaskToUI al metodo addNewTaskToMainWindowUI della classe singleton MainWindowUI per aggiornare l'interfaccia principale con il nuovo task creato.
 *
 *   - update: Cerca il task da aggiornare partendo dalla testa della lista dei task, confrontando il nome del task con taskName.
 *             Una volta trovato il task, crea un decoratore TaskUpdateWindowUIDecorator, avvolgendo TaskCreationWindowUI, e passando il task da aggiornare.
 *             Configura l'interfaccia utente con i dati esistenti del task chiamando setupTaskWidgetsUI().
 *             Collega il segnale signalUpdateTaskToMainWindowUI al metodo updateTaskToMainWindowUI di MainWindowUI. */
TaskWindowUI* TaskWindowUICreator::createTaskWindow(QString mode){
    if(mode == "create"){
        TaskCreationWindowUI* taskCreationWindow = new TaskCreationWindowUI();
        taskCreationWindow->setupTaskWidgetsUI();
        connect(taskCreationWindow, &TaskCreationWindowUI::signalAddNewTaskToUI, MainWindowUI::getInstance(), &MainWindowUI::addNewTaskToMainWindowUI);
        return taskCreationWindow;
    }
    else if(mode == "update"){
        Task* task = AppController::getInstance()->taskHead;
        while(task->getTaskName() != taskName){
            task = task->getTaskNext();
        }
        TaskUpdateWindowUIDecorator* taskUpdateWindow = new TaskUpdateWindowUIDecorator(new TaskCreationWindowUI(), task);
        taskUpdateWindow->setupTaskWidgetsUI();
        connect(taskUpdateWindow, &TaskUpdateWindowUIDecorator::signalUpdateTaskToMainWindowUI, MainWindowUI::getInstance(), &MainWindowUI::updateTaskToMainWindowUI);
        return taskUpdateWindow;
    }
    else{
        return nullptr;
    }
}
