#include "qboxlayout.h"
#include "qscrollarea.h"
#include <QWidget>
#include <AppController.h>
#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H
/* Questa classe ha lo scopo di gestire un'interfaccia utente per visualizzare e interagire con i task attraverso un menu associato all'icona di sistema (tray icon) */
class TrayIconMenu : public QWidget {
    Q_OBJECT
protected:
    // Layout verticale che conterrà i task visualizzabili. In questo layout saranno aggiunti i vari widget (come etichette o bottoni) relativi ai tasks.
    QVBoxLayout* taskListContainerLayout;
public:
    // È un puntatore statico alla singola istanza della classe TrayIconMenu, rendendo la classe un Singleton.
    static TrayIconMenu* instance;
private:
    /* Il costruttore privato imposta il layout e l'interfaccia.
     * Crea un layout verticale per il menu.
     * Aggiunge un widget contenitore, taskListContainer, che ospita un layout verticale taskListContainerLayout per visualizzare i task.
     * Utilizza una QScrollArea per rendere il contenuto scorribile, nel caso il numero di task superi lo spazio visibile. */
    TrayIconMenu(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QWidget* taskListContainer = new QWidget(this);
        taskListContainerLayout = new QVBoxLayout(taskListContainer);
        taskListContainer->setLayout(taskListContainerLayout);
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidget(taskListContainer);
        scrollArea->setWidgetResizable(true);
        layout->addWidget(scrollArea);
        setLayout(layout);
    }
public:
    ~TrayIconMenu() {
        delete instance;
        instance = nullptr;
    }
    /* Questo metodo aggiunge un nuovo task al menu aggiungendolo a taskListContainerLayout. */
    void addTaskToTrayMenu(QString taskName);
    /* Questo metodo elimina un task dal menu. */
    void deleteTaskFromTrayMenu(QString taskName);
    /* Metodo statico che implementa il pattern Singleton.
     * Se l'istanza della classe (instance) è nullptr, crea una nuova istanza di TrayIconMenu e la restituisce.
     * Se l'istanza esiste già, restituisce quella esistente. */
    static TrayIconMenu* getInstance();

};

#endif // TRAYICONMENU_H
