#include <QtWidgets>
#include <Task.h>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <AppController.h>
#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

/* La classe MainWindowUI è la classe principale per la gestione dell'interfaccia grafica principale,
 * con metodi per la creazione, gestione e aggiornamento dei task */
class MainWindowUI: public QWidget{
    public:
        // Istanza dell'oggetto Singleton
        static MainWindowUI* mainWindowUI;
    protected:
        // Griglia in cui verranno aggiunti o eliminati i task presenti nella finestra principale
        QGridLayout* gridTaskMainWindowLayout;
        int col = 0;


    /* L'oggetto MainWindow è un singleton, ciò permette di assicurarsi che esista una sola istanza nel tempo di MainWindowUI.
     * Nel caso in cui non esistesse ancora, ne crea una nuova e la restituisce. */
    private:
        MainWindowUI(){
            AppController::getInstance();
        };

    public:
        ~MainWindowUI() {
            delete mainWindowUI;
            mainWindowUI = nullptr;
        }
        static MainWindowUI* getInstance();
        /* Configura l'interfaccia principale della finestra. */
        void setupMainWindowUI();
        /* Aggiunge un nuovo task all'interfaccia principale. */
        void addNewTaskToMainWindowUI(QString taskName, QString taskDescrption);
        /* Aggiorna le informazioni di un task esistente nell'interfaccia principale.
         * Scorre i task presenti nella finestra principale e aggiorna il nome e la descrizione del task corrispondente, se trova una corrispondenza con il nome del task passato come parametro */
        void updateTaskToMainWindowUI(Task* task);
        /* Cancella i task selezionati (quelli con la casella di controllo selezionata) */
        void deleteTaskFromMainWindowUI();
        /* Gestisce la chiusura della finestra principale.
         * Nasconde la finestra invece di chiuderla completamente e ignora l'evento di chiusura, in modo che possa continuare a funzionare in background. */
        void closeEvent(QCloseEvent *event) override;

    public slots:
        /* Crea una finestra per l'aggiunta di un nuovo task delegando all'TaskWindowUICreator il compito di richiamare il metodo corretto. */
        void openNewTaskCreationWindowUI();
        /* Crea una finestra per l'aggiornamento di un nuovo task delegando all'TaskWindowUICreator il compito di richiamare il metodo corretto. */
        void openNewTaskUpdateWindowUI(QString taskName);
        // Permette al TrayIconMenu di poter riprendere la visualizzazione della finestra principale
        void showMainWindow();

};

#endif // MAINWINDOWUI_H
