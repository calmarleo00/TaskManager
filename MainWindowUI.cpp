/* La classe MainWindowUI è la classe principale per la gestione dell'interfaccia grafica principale,
 * con metodi per la creazione, gestione e aggiornamento dei task */
#include "TaskWindowUICreator.h"
#include<MainWindowUI.h>
#include <TrayIconMenu.h>

MainWindowUI *MainWindowUI::mainWindowUI = nullptr;

/* L'oggetto MainWindow è un singleton, ciò permette di assicurarsi che esista una sola istanza nel tempo di MainWindowUI.
 * Nel caso in cui non esistesse ancora, ne crea una nuova e la restituisce. */
MainWindowUI *MainWindowUI::getInstance()
{
    if(MainWindowUI::mainWindowUI == nullptr){
        mainWindowUI = new MainWindowUI();
    }
    return MainWindowUI::mainWindowUI;
}

/* Configura l'interfaccia principale della finestra. */
void MainWindowUI::setupMainWindowUI(){
    // Massimizza la finestra e ne fissa le dimensioni.
    mainWindowUI->setWindowState(Qt::WindowMaximized);
    mainWindowUI->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gridTaskMainWindowLayout = new QGridLayout();
    // Crea un layout a griglia per la finestra principale e un'area di scorrimento per visualizzare i tasks.
    QGridLayout* gridMainWindowLayout = new QGridLayout();
    mainWindowUI->setLayout(gridMainWindowLayout);

    // Un container utile per il layout a griglia in cui verranno inseriti i diversi tasks
    QWidget* taskGridContainer = new QWidget(mainWindowUI);
    taskGridContainer->setLayout(gridTaskMainWindowLayout);

    // Una scroll area che permetterà di scorrere tra i diversi tasks inseriti
    QScrollArea* taskScrollArea = new QScrollArea(mainWindowUI);

    // La scroll area deve puntare al widget contenente i tasks
    taskScrollArea->setWidget(taskGridContainer);
    taskScrollArea->setWidgetResizable(true);
    // Insert inside the first row of the main grid the scroll area
    gridMainWindowLayout->addWidget(taskScrollArea, 1, 0, 1, 3);

    /* Aggiunge due pulsanti in basso per creare e cancellare task e connette i pulsanti alle relative funzioni di creazione
    * (openNewTaskCreationWindowUI) e cancellazione (deleteTaskFromMainWindowUI). */
    QPushButton* newTaskButton = new QPushButton("Create a new task", mainWindowUI);
    QPushButton* deleteTaskButton = new QPushButton("Delete selected tasks", mainWindowUI);
    newTaskButton->connect(newTaskButton, &QPushButton::released, this, &MainWindowUI::openNewTaskCreationWindowUI);
    deleteTaskButton->connect(deleteTaskButton, &QPushButton::released, this, &MainWindowUI::deleteTaskFromMainWindowUI);
    gridMainWindowLayout->addWidget(newTaskButton, 0, 0, 1, 1);
    gridMainWindowLayout->addWidget(deleteTaskButton, 0, 2, 1, 1);

    // Mostra la finestra principale una volta completata la configurazione.
    mainWindowUI->show();
}

/* Crea una finestra per l'aggiunta di un nuovo task utilizzando un oggetto TaskWindowUICreator.
Il parametro passato a createTaskWindow ("create") indica che la funzione della nuova finestra è quella di creare un nuovo task. */
void MainWindowUI::openNewTaskCreationWindowUI() {
    TaskWindowUICreator *taskWindowCreator = new TaskWindowUICreator();
    taskWindowCreator->createTaskWindow("create");
}

/* Apre una finestra per aggiornare un task esistente, passando il nome del task alla funzione.
Il parametro "update" indica che la funzione della nuova finestra è quella di aggiornare un task esistente. */
void MainWindowUI::openNewTaskUpdateWindowUI(QString taskName) {
    TaskWindowUICreator *taskWindowCreator = new TaskWindowUICreator(taskName);
    taskWindowCreator->createTaskWindow("update");
}

/* Aggiunge un nuovo task all'interfaccia principale. */
void MainWindowUI::addNewTaskToMainWindowUI(QString taskName, QString taskDescription){
    // Definisce un gruppo contenente informazioni base su ogni singolo task (nome e descrizione)
    QGroupBox* taskInformationGroup = new QGroupBox(mainWindowUI);
    taskInformationGroup->setMaximumSize(300, 700);
    QVBoxLayout* taskInformationGroupLayout = new QVBoxLayout(taskInformationGroup);
    taskInformationGroup->setLayout(taskInformationGroupLayout);
    // Il gruppo è selezionabile così che l'utente possa scegliere dei task ben precisi prima di eliminarli
    taskInformationGroup->setCheckable(true);
    taskInformationGroup->setChecked(false);

    QLabel* nameLabel = new QLabel(taskName, taskInformationGroup);

    // Impostare la size del font della label
    QFont fontLabel;
    fontLabel.setPointSize(16);
    nameLabel->setFont(fontLabel);

    QLabel* descriptionLabel = new QLabel(taskDescription, taskInformationGroup);
    // Impostare la size del font della description
    QFont fontDescription;
    fontDescription.setPointSize(12);
    nameLabel->setFont(fontDescription);

    taskInformationGroupLayout->addWidget(nameLabel, 0, Qt::AlignCenter);
    taskInformationGroupLayout->addWidget(descriptionLabel, 0, Qt::AlignCenter);

    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    descriptionLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Per ogni task viene creato un pulsante che ne permette la visualizzazione dei dati
    QPushButton* detailButton = new QPushButton("View Task Details", taskInformationGroup);
    detailButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    taskInformationGroupLayout->addWidget(detailButton, 0, Qt::AlignBottom);
    detailButton->connect(detailButton, &QPushButton::released, this, [this, taskName]{this->openNewTaskUpdateWindowUI(taskName);});

    // Posiziona il task nel layout a griglia della finestra principale
    gridTaskMainWindowLayout->addWidget(taskInformationGroup, 0, col, 1, 1);
    gridTaskMainWindowLayout->setColumnMinimumWidth(col, 220);
    col += 1;

    // Aggiorna il TrayIconMenu con il nuovo task.
    TrayIconMenu::getInstance()->addTaskToTrayMenu(taskName);
}


/* Cancella i task selezionati (quelli con la casella di controllo selezionata)
 * Scorre il layout della finestra principale, controllando quali task sono selezionati, inserendoli in una lista.
 * La lista verrà poi scorsa per eliminare in sicurezza, sia dall'interfaccia che dal database, ogni task selezionato. */
void MainWindowUI::deleteTaskFromMainWindowUI(){
    struct ListGroupBoxToDelete{
        QGroupBox* taskGroupBox;
        ListGroupBoxToDelete* nextTaskGroupBox;
        ListGroupBoxToDelete(QGroupBox* taskGroupBox){
            this->taskGroupBox = taskGroupBox;
            this->nextTaskGroupBox = nullptr;
        }
    };
    ListGroupBoxToDelete* headList = nullptr;
    ListGroupBoxToDelete* tailList = nullptr;
    for(int i = 0; i < gridTaskMainWindowLayout->count(); i++){
        QGroupBox* groupBox = qobject_cast<QGroupBox*>(gridTaskMainWindowLayout->itemAt(i)->widget());
        if(groupBox->isChecked()){
            AppController::getInstance()->deleteTask(qobject_cast<QLabel*>(groupBox->layout()->itemAt(0)->widget())->text());
            TrayIconMenu::getInstance()->deleteTaskFromTrayMenu(qobject_cast<QLabel*>(groupBox->layout()->itemAt(0)->widget())->text());
            if(headList){
                tailList->nextTaskGroupBox = new ListGroupBoxToDelete(groupBox);
                tailList = tailList->nextTaskGroupBox;
            }
            else{
                headList = new ListGroupBoxToDelete(groupBox);
                tailList = headList;
            }
        }
    }
    tailList = headList;
    while(tailList){
        tailList->taskGroupBox->deleteLater();
        gridTaskMainWindowLayout->removeWidget(tailList->taskGroupBox);
        ListGroupBoxToDelete* tempVal = tailList->nextTaskGroupBox;
        delete tailList;
        tailList = tempVal;
        col -= 1;
    }
    delete tailList;
}

/* Aggiorna le informazioni di un task esistente nell'interfaccia principale.
 * Scorre i task presenti nella finestra principale e aggiorna il nome e la descrizione del task corrispondente, se trova una corrispondenza con il nome del task passato come parametro */
void MainWindowUI::updateTaskToMainWindowUI(Task* task){
    for(int i = 0; i < gridTaskMainWindowLayout->count(); i++){
        if(qobject_cast<QLabel*>(gridTaskMainWindowLayout->itemAt(i)->widget()->layout()->itemAt(0)->widget())->text() == task->getTaskName()){
            qobject_cast<QLabel*>(gridTaskMainWindowLayout->itemAt(i)->widget()->layout()->itemAt(0)->widget())->setText(task->getTaskName());
            qobject_cast<QLabel*>(gridTaskMainWindowLayout->itemAt(i)->widget()->layout()->itemAt(1)->widget())->setText(task->getTaskDescription());
        }
    }
}

/* Gestisce la chiusura della finestra principale.
 * Nasconde la finestra invece di chiuderla completamente e ignora l'evento di chiusura, in modo che possa continuare a funzionare in background. */
void MainWindowUI::closeEvent(QCloseEvent *event) {
        getInstance()->hide();
        event->ignore();
}
