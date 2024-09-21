#ifndef TASK_H
#define TASK_H
#include "qjsonobject.h"
#include <QString>
#include <QObject>
#include <QPushButton>
#include <QGridLayout>
#include <QDate>
#include <Schedule.h>
/* La classe Task rappresenta un'attività o processo che deve essere eseguito.
 * Organizza le attività in una lista concatenata, con task che possono essere pianificate su base settimanale, permettendo la ripetizione a intervalli di tempo definiti.*/
class Task: public QObject{
    Q_OBJECT
    protected:
        // Nome del task.
        QString name;
        // Descrizione della task.
        QString description;
        // Indica se il task è interno o esterno
        bool isExternal;
        // Indica se il task deve essere ripetuto.
        bool isRepeatable;
        // Il comando da eseguire nel caso il task sia esterno.
        QString command;
        // Parametri aggiuntivi associati alla task.
        QJsonObject parameters;
        // Data d'inizio del task
        QDate startDate;
        // Data di fine del task
        QDate endDate;
        // Array di puntatori di tipo Schedule per rappresentare la pianificazione settimanale.
        Schedule** scheduling = new Schedule*[7];
        // Puntatore al task successivo. Definisce così una lista di task che verrà
        // mantenuta e utilizzata dall'AppController
        Task* nextTask;

    public:
        // Costruttore default
        Task();
        // Costruttore con il nome da impostare del task
        Task(QString name);
        // Resituisce il nome del task
        QString getTaskName();
        // Imposta il nome del task
        void setTaskName(QString name);
        // Restituisce la descrizione del task
        QString getTaskDescription();
        // Imposta la descrizione del task
        void setTaskDescription(QString description);
        // Restituisce se il task è esterno o interno
        bool getIsExternal();
        // Imposta se il task è esterno o interno
        void setIsExternal(bool isExternal);
        // Restituisce se il task è ripetibile
        bool getIsRepeatable();
        // Imposta se il task è ripetibile
        void setIsRepeatable(bool isRepeatable);
        // Restituisce il comando del task, se esterno
        QString getTaskCommand();
        // Imposta il comando del task, se esterno
        void setTaskCommand(QString command);
        // Resituisce il task successivo
        Task* getTaskNext();
        // Imposta il task successivo
        void setTaskNext(Task* next);
        // Restituisce i parametri del task
        QJsonObject getTaskParameters();
        // Imposta i parametri del task
        void setTaskParameters(QJsonObject jsonObject);
        // Restituisce la data d'inizio
        QDate getStartDate();
        // Imposta la data d'inizio
        void setStartDate(QDate startDate);
        // Restituisce la data di conclusione
        QDate getEndDate();
        // Imposta la data di fine
        void setEndDate(QDate endDate);
        // Definisce un metodo che verrà implementato dai task derivanti dalla classe.
        // In questo modo si definisce uno Strategy Pattern utile allo Scheduler per poter eseguire senza problemi
        // a run-time qualsiasi task abbia bisogno di essere eseguito.
        virtual void execute() {};
        // Restituisce l'array di pianificazione dei giorni della settimana
        Schedule** getScheduling();
};

#endif // TASK_H
