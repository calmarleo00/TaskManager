#include <TrayIconMenu.h>
#include <MainWindowUI.h>
TrayIconMenu* TrayIconMenu::instance = nullptr;

/* Metodo statico che implementa il pattern Singleton.
 * Se l'istanza della classe (instance) è nullptr, crea una nuova istanza di TrayIconMenu e la restituisce.
 * Se l'istanza esiste già, restituisce quella esistente. */
TrayIconMenu* TrayIconMenu::getInstance(){
    if(!instance){
        instance = new TrayIconMenu();
    }
    return instance;
}

/* Questo metodo aggiunge un nuovo task al menu aggiungendolo a taskListContainerLayout. */
void TrayIconMenu::addTaskToTrayMenu(QString taskName){
    QWidget* taskContainer = new QWidget(this);
    QHBoxLayout* taskContainerLayout = new QHBoxLayout(taskContainer);
    taskContainer->setLayout(taskContainerLayout);

    QLabel* taskNameLabel = new QLabel(taskName, taskContainer);
    QPushButton* taskDetailsButton = new QPushButton("View Task's Details", taskContainer);
    taskContainerLayout->addWidget(taskNameLabel, 0 ,Qt::AlignLeft);
    taskContainerLayout->addWidget(taskDetailsButton, 0, Qt::AlignRight);
    taskContainer->setMaximumHeight(50);
    taskDetailsButton->connect(taskDetailsButton, &QPushButton::released, MainWindowUI::getInstance(), [taskName]{MainWindowUI::getInstance()->openNewTaskUpdateWindowUI(taskName);});
    taskListContainerLayout->addWidget(taskContainer, 0, Qt::AlignTop);
}

/* Questo metodo elimina un task dal menu. */
void TrayIconMenu::deleteTaskFromTrayMenu(QString taskName){
    for(int i = 0; i < taskListContainerLayout->count(); i++){
        QWidget* taskContainer = taskListContainerLayout->itemAt(i)->widget();
        if(qobject_cast<QLabel*>(taskContainer->layout()->itemAt(0)->widget())->text() == taskName){
            taskContainer->deleteLater();
            taskListContainerLayout->removeWidget(taskContainer);
        }
    }
}
