#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>
#include <QTextEdit>
#include <QSplitter>
#include <QSizeGrip>
#include <QRect>
#include <TaskAttributeUI.h>
#include <TaskCreationSchedulingUIBuilder.h>
#include <TaskFixedSchedulingUIBuilder.h>
#include <TaskCreationSchedulingUIDirector.h>
#include <TaskWindowUI.h>
#include <AppController.h>

#ifndef TASKCREATIONWINDOWUI_H
#define TASKCREATIONWINDOWUI_H
/* Questa classe è figlia della classe astratta TaskWindowUI e rappresenta la finestra per la creazione di un nuovo task all'interno di un'applicazione.
 * Questa finestra segue una struttura a pagine (i.e. un QStackedWidget), permettendo all'utente di passare attraverso diversi interfacce per configurare un nuovo task,
 * inclusa quella per la selezione degli attributi, la selezione della tipologia di scheduling, e l'impostazione dello scheduling stesso. */
class TaskCreationWindowUI : public TaskWindowUI{

    Q_OBJECT
    public:
        // Inizializza un QStackedWidget, che verrà utilizzato per ospitare le varie pagine della procedura di creazione del task.
        TaskCreationWindowUI(){
            stackedWindows = new QStackedWidget();
        }
        /* Questo metodo è responsabile di impostare la struttura delle finestre della UI di creazione del task.
         * Imposta il QStackedWidget come modale, affinché l'utente non possa interagire con la MainWindowUI fino a quando non interrompe o conclude l'operazione.
         * Sfruttando la classe responsabile, TaskAttributeUI, avvia la creazione della prima finestra (i.e. l'interfaccia per la definizione degli attributi del task) e ne ottiene il risultato.
         * Imposta, successivamente, la seconda pagina, che contiene la selezione del tipo di pianificazione (schedulazione fissa o ripetibile).
         * Aggiunge entrambe le pagine al stackedWindows e mostra il widget. */
        void setupTaskWidgetsUI() override;
        // Restituisce il puntatore alla prima finestra (necessario al Decorator al momento dell'Update delle informazioni).
        TaskAttributeUI* getFirstWindow() override;
        // Restituisce il widget della selezione della schedulazione (necessario al Decorator al momento dell'Update delle informazioni).
        QWidget* getSchedulingSelectionWidget() override;
        // Restituisce il director responsabile del coordinamento della costruzione della pagina della schedulazione (necessario al Decorator al momento dell'Update delle informazioni).
        TaskCreationSchedulingUIDirector* getSecondWindowDirector() override;
        // Restituisce il QStackedWidget, che contiene tutte le finestre della UI.
        QStackedWidget* getStackedWindows() override;

    public slots:
        /* Questo slot viene chiamato quando si procede alla pagina delle impostazioni dello scheduling.
         * Se l'utente ha selezionato una schedulazione fissa, la pagina viene costruita traime TaskFixedSchedulingUIBuilder e TaskCreationSchedulingUIDirector.
         * Se invece ha selezionato una schedulazione ripetibile, vengono utilizzate TaskRepeatableSchedulingUIBuilder e TaskCreationSchedulingUIDirector. */
        void nextSchedulePage() override;

    private slots:
        // Questo slot viene richiamato alla richiesta di ritornare alla pagina di configurazione degli attributi.
        void backAttributeSelectionPage() override;
        // Permette di tornare alla pagina di selezione del tipo di schedulazione.
        void backSchedulingSelectionPage() override;
        /* Quando l'utente decide di procedere alla selezione del tipo di pianificazione:
         * Recupera il nome e la descrizione del task dalla prima finestra (tramite il layout degli attributi).
         * Chiede all'AppController di creare un oggetto Task temporaneo con le informazioni parziali fornite fino a quel momento.
         * Passa alla pagina di selezione della schedulazione nel stackedWindows. */
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) override;
        /* Questo metodo viene utilizzato per chiudere la finestra di creazione del task dopo aver completato la configurazione della schedulazione ripetibile.
         * Salva i dettagli del task e invoca i metodi del controller per salvare il task nel database, aggiungendo il task al programma. */
        void closeRepeatableWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) override;
        void closeFixedWindow(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startEdit, QDateEdit* endDateEdit) override;
        void disableRepeatableCheckBox(int value);
        void disableFixedCheckBox(int value);
};

#endif // TASKCREATIONWINDOWUI_H
