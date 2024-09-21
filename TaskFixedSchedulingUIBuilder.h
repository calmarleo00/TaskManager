#include "qboxlayout.h"
#include "qspinbox.h"
#include <TaskCreationSchedulingUIBuilder.h>
#ifndef TASKFIXEDSCHEDULINGUIBUILDER_H
#define TASKFIXEDSCHEDULINGUIBUILDER_H
class TaskFixedSchedulingUIBuilder : public TaskCreationSchedulingUIBuilder{
    Q_OBJECT

protected:
    // Il layout principale (di tipo QVBoxLayout) per organizzare i widget nella finestra.
    QVBoxLayout* taskFixedSchedulingLayout;
    // Un layout secondario che contiene i componenti specifici per la pianificazione del task.
    QVBoxLayout* scheduleGroupLayout;
    // Array contenente QCheckBoxes uno per ogni giorno della settimana
    QCheckBox* daysCheckBoxes;
    // Array contenente una serie di widget in cui è possibile impostare le ore per ripetere il task in quelle giornate
    QWidget* daysTimeWidgets;
    // Array contenente i tempi fissi definiti per ogni singolo giorno
    QVBoxLayout* timeContainerLayout;

public:
    TaskFixedSchedulingUIBuilder(){
        taskFixedScheduling = new QWidget();
        daysCheckBoxes = new QCheckBox[7];
        daysTimeWidgets = new QWidget[7];
        timeContainerLayout = new QVBoxLayout[7];
    }
    // Inizializza la finestra di pianificazione, stabilendo dimensioni minime e massime e organizzando il layout.
    void initializeWindowStep() override;
    // Aggiunge un calendario e due QDateEdit (per le date di inizio e fine), insieme alla gestione delle date selezionate.
    void buildCalendarStep() override;
    // Essendo un task ripetuto è di solo interesse il giorno d'inizio
    void buildDaysStep() override;
    // Costruisce la parte dell'interfaccia per scegliere se la ripetizione sarà misurata in ore o secondi, permettendo di selezionare l'ora di inizio e la frequenza di ripetizione.
    void buildTimeStep() override;
    // Aggiunge i pulsanti per tornare alla pagina precedente o completare la pianificazione.
    void buttonStep() override;
    // Getters delle variabili
    QDateEdit* getStartDateEdit() override;
    QDateEdit* getEndDateEdit() override;
    QCheckBox* getDaysCheckBoxes();
    QVBoxLayout* getTimeContainerLayout();

    // Restituisce il widget principale della finestra, che può essere integrato in altre parti dell'applicazione.
    QWidget* getResult() override;

private slots:
    // Permette al widget di una giornata di apparire in base al flag del corrispondente QCheckBox
    void dayTimeAppear(int day);
    // Permette di aggiungere tempi in più per ogni giorno
    void addTime(int day);
    // Gestisce la logica per aggiornare le date di inizio e fine basandosi sulla selezione del calendario.
    void setDate(QDate date);
    // Emesso quando l'utente decide di tornare alla selezione della pagina di pianificazione.
    void emitBackSchedulingSelectionSignal();
    // Emesso quando la pianificazione è completata, passando i parametri selezionati come checkbox, date, orari e intervalli di ripetizione.
    void emitClosePageSignal();

signals:
    void closePageFixedSignal(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startDateEdit, QDateEdit* endDateEdit);

};

#endif // TASKFIXEDSCHEDULINGUIBUILDER_H
