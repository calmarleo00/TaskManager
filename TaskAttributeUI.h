#ifndef TASKATTRIBUTEUI_H
#define TASKATTRIBUTEUI_H
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qgridlayout.h"
#include "qpushbutton.h"
#include <QWidget>
#include <QString>

class TaskCreationAttributeUI : public QWidget{
    Q_OBJECT
    QWidget* taskAttributeWindowWidget;
    QGridLayout* attributesGrid;
    QPushButton* addAttributeButton;
    QCheckBox* isExternal;
    int row = 0;

    public:
        TaskCreationAttributeUI(){
            taskAttributeWindowWidget = new QWidget();
        }
        void setupTaskAttributeCreationUI();
        void setupTaskAttributeUpdateUI();

        QWidget* getTaskAttributeWindowWidget();
    private:
    /* Function that adds the default attributes (name - description) of the task */
        void addDefaultTaskAttribute(QString name, QString type);

    private slots:
        void nextPageSave();
        /* Function that allows the user to define its own attributes */
        void addTaskAttribute();

        /* Function which scope is to define which box needs to appear based on the selected type:
         * string -> QTextEdit
         * int -> QSpinBox
         * double -> QDoubleSpinBox
         * The comboBox argument is necessary to retrieve in which row the value's box needs to be changed */
        void changeValue(QComboBox* comboBox);
        /* If the isExternal check box is flagged, then be sure to remove all the attributes inserted.
         * Then adds the new and only attribute needed: the text edit in which will be inserted the command line instruction */
        void disableAddition(int state);

    signals:
        void nextPage(QGridLayout* attributesGrid, bool isExternal);

};

#endif // TASKATTRIBUTEUI_H
