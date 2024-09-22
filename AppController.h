#include "qcheckbox.h"
#include "qdatetimeedit.h"
#include "qjsonobject.h"
#include "qspinbox.h"
#include<QWidget>
#include<Task.h>
#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
/* La classe è una classe singleton che gestisce la creazione,
 * la gestione, e l'aggiornamento delle attività (Task) in un'applicazione.
 * Utilizza il pattern Singleton per assicurarsi che ci sia solo un'istanza della classe in esecuzione.
 * La classe gestisce l'interazione con un database, rappresentando inoltre il punto di incontro tra il livello applicativo e il livello di presentazione. */
class AppController : public QWidget{
    public:
        // È il puntatore statico che permette di implementare il pattern Singleton.
        static AppController* instance;
        // Puntatore al primo Task.
        Task* taskHead = nullptr;
        // Puntatore all'ultimo Task esistente.
        Task* taskTail = nullptr;
        // Puntatore temporaneo utilizzato per creare una nuova attività.
        Task* tmpTask;
        // Oggetto JSON usato per memorizzare gli attributi delle attività.
        // La scelta ricade sul JSON in modo tale da poter aggiungere ed eliminare dinamicamente gli
        // attributi di un task.
        QJsonObject jsonObject;
    private:
        AppController(){};
    public:
        // Crea una nuova attività.
        void createNewTask(QGridLayout* attributesGrid, bool isExternal);
        // Imposta gli attributi per una nuova attività. Aggiorna la struttura dati in memoria, salvando i parametri delle attività come un JSON se l'attività non è esterna.
        void setTaskAttributes(Task* newTask, QGridLayout* attributesGrid, bool isExternal);
        // Salva l'attività attualmente puntata da taskTail nel database.
        void saveTaskToDatabase();
        // Aggiorna le informazioni del task contenute nella memoria e nel database
        void updateTaskInDatabase(Task* task);
        // Imposta la pianificazione ripetibile dell'attività. Specifica i parametri come ore, secondi, data di inizio e fine. Le checkbox determinano se la ripetizione è basata su ore o secondi.
        void setTaskRepeatableScheduleValues(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount);
        // Imposta la pianificazione fissa dell'attività. L'attività è programmata per ripetersi su giorni specifici, usando l'interfaccia fornita da daysCheckBoxes e timeContainerLayout.
        void setTaskFixedScheduleValues(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startDateEdit, QDateEdit* endDateEdit);
        // Aggiunge l'attività corrente (taskTail) alla coda di pianificazione del Scheduler.
        void callAddTaskToSchedule();
        // Elimina un'attività dal database e dalla lista collegata delle attività in memoria. Rimuove anche l'attività dalla coda del pianificatore.
        void deleteTask(QString taskName);
        // Restituisce l'istanza della classe AppController.
        static AppController* getInstance();
};

#endif // APPCONTROLLER_H
