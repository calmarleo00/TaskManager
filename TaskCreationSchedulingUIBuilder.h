#include "qcheckbox.h"
#include "qdatetimeedit.h"
#include <QWidget>
#include <QPushButton>

#ifndef TASKCREATIONSCHEDULINGUIBUILDER_H
#define TASKCREATIONSCHEDULINGUIBUILDER_H
/* La classe definisce l'interfaccia base per i builder della UI di pianificazione di un task. */
class TaskCreationSchedulingUIBuilder : public QObject{
    Q_OBJECT
    protected:
        // Il prodotto creato dal builder
        QWidget* taskFixedScheduling;
        // Definiti tutti elementi di layout che serviranno poi per passare i dati all'AppController per ricostruire che dati sono stati inseriti e salvarli nel database
        QCheckBox* hourCheckBox = nullptr;
        QCheckBox* secondCheckBox = nullptr;
        QDateEdit* startDateEdit = nullptr;
        QDateEdit* endDateEdit = nullptr;
        // Flag per determinare se la data di inizio deve essere cambiata.
        bool toChangeStartDate = true;

    public:
        // Inizializza la finestra o il layout principale dove verranno posizionati gli altri elementi.
        virtual void initializeWindowStep() = 0;
        // Aggiunge alla UI il controllo per selezionare le date di inizio e fine (calendario).
        virtual void buildCalendarStep() = 0;
        // Costruisce la parte dell'interfaccia per selezionare i giorni specifici della pianificazione
        virtual void buildDaysStep() = 0;
        // Aggiunge la parte per selezionare l'ora di inizio e l'ora di fine della pianificazione.
        virtual void buildTimeStep() = 0;
        // Aggiunge i bottoni necessari per navigare o confermare le azioni nella finestra
        virtual void buttonStep() = 0;
        // Serie di getter methods
        virtual QDateEdit* getStartDateEdit() = 0;
        virtual QDateEdit* getEndDateEdit() = 0;
        virtual QWidget* getResult() = 0;

    signals:
        // Un segnale che verrà emesso quando l'utente preme un pulsante per tornare alla pagina precedente.
        void backPageSignal();

};

#endif // TASKCREATIONSCHEDULINGUIBUILDER_H
