#ifndef TASKATTRIBUTEUI_H
#define TASKATTRIBUTEUI_H
#include "Task.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qdialog.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include <QWidget>
#include <QString>
/* Questa classe gestisce l'interfaccia utente per la creazione e la modifica di attributi per un task. */
class TaskAttributeUI : public QWidget{
    Q_OBJECT
    // Rappresenta la finestra principale dell'interfaccia utente.
    QWidget* taskAttributeWindowWidget;
    // Un layout a griglia (QGridLayout) che contiene gli attributi dell'attività.
    QGridLayout* attributesGrid;
    // Un pulsante che permette all'utente di aggiungere nuovi attributi personalizzati.
    QPushButton* addAttributeButton;
    // Un QCheckBox che indica se l'attività è definito internamente o esternamente.
    QCheckBox* isExternal;
    // Un QLabel utilizzato per visualizzare eventuali errori di input
    QLabel* errorName;
    // Un pulsante per passare alla pagina successiva.
    QPushButton* nextPageButton;
    // Un contatore che tiene traccia della riga corrente nella griglia degli attributi.
    int row;

    public:
        TaskAttributeUI(QWidget* parent){
            taskAttributeWindowWidget = new QWidget(parent);
            row = 0;
        }
        /* Configura l'interfaccia utente per creare una nuova attività. */
        void setupTaskAttributeCreationUI();
        // Restituisce il layout a griglia degli attributi.
        QGridLayout* getAttributesGrid();
        // Restituisce il pulsante per passare alla pagina successiva.
        QPushButton* getNextPageButton();
        // Restituisce il numero di righe attualmente utilizzate nella griglia.
        int getAttributeRow();
        // Verifica se il checkbox "External Task" è selezionato.
        bool getIsExternal();
        // Imposta lo stato del checkbox "External Task".
        void setIsExternal(bool isExternal);
        // Controlla se il nome dell'attività interna corrisponde a un file C++ esistente. Se non esiste, mostra un errore.
        bool checkInternalTaskExistence();
        // Verifica se esiste già un'attività con lo stesso nome. Se sì, visualizza un messaggio di errore.
        bool checkTaskAlreadyCreated();
        // Restituisce il widget della finestra principale.
        QWidget* getTaskAttributeWindowWidget();

    private:
        // Aggiunge attributi predefiniti, come il nome e la descrizione dell'attività, alla griglia degli attributi.
        void addDefaultTaskAttribute(QString name, QString type);

    public slots:
        // Permette all'utente di aggiungere un attributo personalizzato.
        void addTaskAttribute();
        // Disabilita l'aggiunta di nuovi attributi
        void disableAddition(int state);
        // Quando si preme il pulsante "Next", controlla se l'attività è valida e non esiste già, emettendo un segnale per procedere.
        void nextPageCreate();
        // Emette il segnale per passare alla pagina successiva (aggiornamento di un'attività).
        void nextPageUpdate();

    private slots:
        /* Funzione che definisce quale box debba apparire basandosi sull'attributo selezioanto:
         * string -> QTextEdit
         * int -> QSpinBox
         * double -> QDoubleSpinBox */
        void changeValue(QComboBox* comboBox);

    signals:
        void nextPage(QGridLayout* attributesGrid, bool isExternal);

};

#endif // TASKATTRIBUTEUI_H
