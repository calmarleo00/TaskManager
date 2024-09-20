#include <TaskCreationSchedulingUIBuilder.h>
#include <Task.h>
#ifndef TASKCREATIONSCHEDULINGUIDIRECTOR_H
#define TASKCREATIONSCHEDULINGUIDIRECTOR_H
/* La classe implementa il Director in un Builder Pattern.
 * Il suo scopo è dirigere il processo di costruzione dell'interfaccia utente per la pianificazione di un task,
 * coordinando i passaggi di costruzione definiti nel TaskCreationSchedulingUIBuilder. */
class TaskCreationSchedulingUIDirector{
    TaskCreationSchedulingUIBuilder* builder;

    public:
        // Il costruttore accetta un oggetto builder che sarà utilizzato per la costruzione della finestra di pianificazione.
        TaskCreationSchedulingUIDirector(TaskCreationSchedulingUIBuilder* builder){
            this->builder = builder;
        }

        // Questo metodo consente di cambiare dinamicamente il builder associato al director.
        // Utile se si desidera utilizzare un differente tipo di builder per personalizzare la costruzione dell'interfaccia utente.
        void setBuilder(TaskCreationSchedulingUIBuilder* builder){
            this->builder = builder;
        }

        /* Questo metodo rappresenta il flusso completo per la costruzione dell'interfaccia di pianificazione di un task.
         * Chiede al builder di eseguire i vari passi della costruzione, che sono:
         * initializeWindowStep(): inizializza la struttura base della finestra.
         * buildCalendarStep(): costruisce la parte dell'interfaccia relativa al calendario, dove si selezionano le date di inizio e fine.
         * buildDaysStep(): definisce i giorni della settimana interessati dalla pianificazione pianificazione.
         * buildTimeStep(): aggiunge la parte dell'interfaccia per la selezione dell'ora.
         * buttonStep(): aggiunge i bottoni necessari all'interfaccia. */
        void buildWindow(){
            builder->initializeWindowStep();
            builder->buildCalendarStep();
            builder->buildDaysStep();
            builder->buildTimeStep();
            builder->buttonStep();
        }
};

#endif // TASKCREATIONSCHEDULINGUIDIRECTOR_H
