#ifndef TASKREPEATABLESCHEDULINGUIBUILDER_H
#define TASKREPEATABLESCHEDULINGUIBUILDER_H

#include "TaskCreationSchedulingUIBuilder.h"
#include "qspinbox.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>

/* La classe un'implementazione concreta del pattern Builder per la creazione di un'interfaccia grafica (UI) volta a pianificare task ripetibili,
 * estendendo la classe astratta TaskCreationSchedulingUIBuilder.
 * Questa classe si concentra sulla gestione della UI per la pianificazione di task che si ripetono a intervalli regolari, con la possibilità di specificare l'intervallo in ore o secondi. */
class TaskRepeatableSchedulingUIBuilder : public TaskCreationSchedulingUIBuilder {
    Q_OBJECT

    protected:
        // Il layout principale (di tipo QVBoxLayout) per organizzare i widget nella finestra.
        QVBoxLayout* taskRepeatableSchedulingLayout;
        // Un layout secondario che contiene i componenti specifici per la pianificazione del task.
        QVBoxLayout* scheduleGroupLayout;
        // QTimeEdit che conterrà il valore di inizio d'esecuzione del task
        QTimeEdit* startTime;
        // Utilizzato per selezionare l'ammontare delle ore/secondi necessari prima di eseguire la ripetizione
        QSpinBox* numTime;

    public:
        // Inizializza la finestra di pianificazione, stabilendo dimensioni minime e massime e organizzando il layout.
        void initializeWindowStep() override;
        // Aggiunge un calendario e due QDateEdit (per le date di inizio e fine), insieme alla gestione delle date selezionate.
        void buildCalendarStep() override;
        // Essendo un task ripetuto è di solo interesse il giorno d'inizio
        void buildDaysStep() override {};
        // Costruisce la parte dell'interfaccia per scegliere se la ripetizione sarà misurata in ore o secondi, permettendo di selezionare l'ora di inizio e la frequenza di ripetizione.
        void buildTimeStep() override;
        // Aggiunge i pulsanti per tornare alla pagina precedente o completare la pianificazione.
        void buttonStep() override;
        // Getters delle variabili
        QCheckBox* getHourCheckBox();
        QCheckBox* getSecondCheckBox();
        QDateEdit* getStartDateEdit() override;
        QDateEdit* getEndDateEdit() override;
        QTimeEdit* getStartTime();
        QSpinBox* getRepeatableAmount();
        // Restituisce il widget principale della finestra, che può essere integrato in altre parti dell'applicazione.
        QWidget* getResult() override;

    private slots:
        // Disabilitano i checkbox per ore e secondi in base alla selezione dell'utente, garantendo che solo una delle due opzioni possa essere selezionata alla volta.
        void disableSecondCheckBox(int value);
        void disableHourCheckBox(int value);
        // Gestisce la logica per aggiornare le date di inizio e fine basandosi sulla selezione del calendario.
        void setDate(QDate date);
        // Emesso quando l'utente decide di tornare alla selezione della pagina di pianificazione.
        void emitBackSchedulingSelectionSignal();
        // Emesso quando la pianificazione è completata, passando i parametri selezionati come checkbox, date, orari e intervalli di ripetizione.
        void emitClosePageSignal();

    signals:
        void closePageRepeatableSignal(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount);

    };



#endif // TASKREPEATABLESCHEDULINGUIBUILDER_H
