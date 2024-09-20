#include <TaskWindowUI.h>
#ifndef TASKUPDATEWINDOWUIDECORATOR_H
#define TASKUPDATEWINDOWUIDECORATOR_H
/* La classe TaskUpdateWindowUIDecorator estende la classe TaskWindowUI e rappresenta un decoratore che avvolge un'istanza di TaskWindowUI per gestire l'aggiornamento di un task esistente.
 * Utilizza il pattern Decorator per aggiungere o modificare funzionalità specifiche alla finestra di aggiornamento senza cambiare l'implementazione originale di TaskWindowUI.
 * In questo modo possono essere utilizzati gli stessi metodi di creazione della finestra proveniente da TaskCreationWindowUI e successivamente aggiungere tutti i dettagli del task selezionato
 * all'interno del layout */
class TaskUpdateWindowUIDecorator : public TaskWindowUI{
    Q_OBJECT
    // Rappresenta il componente di TaskWindowUI da decorare
    TaskWindowUI* component;
    // Rappresenta il task da aggiornare.
    Task* task;
    public:
        TaskUpdateWindowUIDecorator(TaskWindowUI* componentToWrap, Task* taskToUpdate){
            component = componentToWrap;
            stackedWindows = new QStackedWidget();
            task = taskToUpdate;
        }
    public:
        /* Configura l'interfaccia utente per l'aggiornamento di un task.
         * Disconnette il pulsante "Next >" dallo slot standard di creazione e lo collega al proprio (nextSchedulingSelectionPage).
         * Imposta i campi (nome del task, descrizione, eventuali parametri esterni/interni, ecc.) con i valori correnti del task da aggiornare. */
        void setupTaskWidgetsUI() override;
        TaskAttributeUI* getFirstWindow() override;
        QWidget* getSchedulingSelectionWidget() override;
        TaskCreationSchedulingUIDirector* getSecondWindowDirector() override;
        QStackedWidget* getStackedWindows() override;
    private slots:
        // Questi due slot non implementano nulla di aggiuntivo rispetto al componente
        void backAttributeSelectionPage() override;
        void backSchedulingSelectionPage() override;
        // Questo metodo aggiorna il nome e la descrizione del task, nonché gli attributi associati, e mostra la finestra di selezione della schedulazione.
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) override;
        /* Se il task è fisso o ripetibile, costruisce la finestra corrispondente e ne imposta i valori in base alle proprietà del task esistente. */
        void nextSchedulePage() override;
        /* Questo metodo chiude la finestra e salva i dettagli del task aggiornato.
         * Utilizza i valori correnti dell'interfaccia utente per aggiornare il task.
         * Dopo aver aggiornato il task nel database e nello scheduler, emette il segnale signalUpdateTaskToMainWindowUI per notificare la finestra principale dell'aggiornamento. */
        void closeRepeatableWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) override;
        void closeFixedWindow(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startEdit, QDateEdit* endDateEdit) override;
};
#endif // TASKUPDATEWINDOWUIDECORATOR_H
