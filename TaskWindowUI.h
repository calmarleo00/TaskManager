#include "qwidget.h"
#include "qcheckbox.h"
#include "qstackedwidget.h"
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <TaskAttributeUI.h>
#include <TaskCreationSchedulingUIBuilder.h>
#include <TaskFixedSchedulingUIBuilder.h>
#include <TaskCreationSchedulingUIDirector.h>
#include <TaskRepeatableSchedulingUIBuilder.h>
#include <AppController.h>

#ifndef TASKWINDOWUI_H
#define TASKWINDOWUI_H

/* Classe base astratta che definisce la struttura e il comportamento generale per le finestre di creazione dei task. */
class TaskWindowUI : public QWidget{
    Q_OBJECT
    protected:
        // Rappresenta la prima pagina dell'interfaccia per la creazione di un task, dove vengono definiti i suoi attributi.
        TaskAttributeUI* firstWindow;
        // Un widget utilizzato per selezionare il tipo di pianificazione del task.
        QWidget* schedulingSelectionWidget;
        /* La classe rappresentate il direttore nel design pattern "builder". Essa coordina la costruzione della seconda finestra per la configurazione della pianificazione del task,
         * basandosi sulla precedente selezione dell'utente. */
        TaskCreationSchedulingUIDirector* secondWindowDirector;
        // Viene utilizzato per contenere più pagine (widget) e consente di passare da una pagina all'altra.
        QStackedWidget* stackedWindows = nullptr;
        // I due checkbox vengono utilizzati per poter capire che tipo di UI per le impostazioni della schedulazione generare
        // Un checkbox che permettere all'utente di scegliere se il task deve essere ripetibile.
        QCheckBox* repeatableSelection;
        // Un checkbox che permettere all'utente di scegliere se il task è fisso.
        QCheckBox* fixedSelection;
        // Informazioni necessarie da mantenere per poterle poi utilizzare per richiamare i metodi corretti per la creazione del task, senza doverle riottenere successivamente dal layout
        // Nome del task
        QString taskName;
        // Descrizione del task
        QString taskDescription;

    public:
        // Metodo astratto per configurare e inizializzare i vari widget necessari per la creazione del task.
        virtual void setupTaskWidgetsUI() = 0;
        // Restituisce un puntatore al primo widget (finestra) che contiene l'interfaccia per l'inserimento degli attributi del task.
        virtual TaskAttributeUI* getFirstWindow() = 0;
        // Restituisce un puntatore al widget che rappresenta la selezione della schedulazione del task.
        virtual QWidget* getSchedulingSelectionWidget() = 0;
        // Restituisce un puntatore al direttore della costruzione della seconda finestra, che viene usato per creare la UI della schedulazione.
        virtual TaskCreationSchedulingUIDirector* getSecondWindowDirector() = 0;
        // Restituisce il QStackedWidget che contiene le diverse pagine della procedura.
        virtual QStackedWidget* getStackedWindows() = 0;

    public slots:
        // Metodo astratto che avanza alla prossima pagina nella procedura di creazione del task (tipicamente la pagina di configurazione della schedulazione).
        virtual void nextSchedulePage() = 0;
    private slots:
        // Metodo che consente di tornare alla prima pagina, dove vengono inseriti gli attributi del task.
        virtual void backAttributeSelectionPage() = 0;
        // Metodo che permette di tornare alla selezione del tipo di schedulazione.
        virtual void backSchedulingSelectionPage() = 0;
        /* Metodo chiamato per passare alla successiva pagina della UI, dove l'utente seleziona il tipo di schedulazione.
         * L'argomento attributesGrid contiene i widget per i dettagli del task, mentre isExternal indica se il task è da considerare esterno o meno. */
        virtual void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) = 0;
        /* Questi slot si occupano della chiusura della finestra e dei processi di salvataggio da avviare. Ricevono i widget/layout contenenti i dettagli per la schedulazione e li passano all'AppController per il salvataggio. */
        virtual void closeRepeatableWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) = 0;
        virtual void closeFixedWindow(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startEdit, QDateEdit* endDateEdit) = 0;
    signals:
        // Segnale emesso quando un nuovo task viene aggiunto, notificando MainWindowUI che un nuovo task è stato creato con un nome e una descrizione.
        void signalAddNewTaskToUI(QString taskName, QString taskDescription);
        // Segnale emesso quando un task esistente viene aggiornato, passando il task modificato all'interfaccia principale.
        void signalUpdateTaskToMainWindowUI(Task* task);

    };

#endif // TASKWINDOWUI_H
