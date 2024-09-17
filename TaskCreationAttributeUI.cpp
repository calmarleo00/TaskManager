#include "qcheckbox.h"
#include "qgroupbox.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qscrollarea.h"
#include "qspinbox.h"
#include "qtextedit.h"
#include <TaskCreationAttributeUI.h>

void TaskCreationAttributeUI::setupTaskCreationUI(){
    // Set minimum and maximum size of the window
    taskAttributeWindowWidget->setMinimumSize(400, 300);
    taskAttributeWindowWidget->setMaximumSize(550, 400);

    // Set the layout and let it align everything at the top
    QVBoxLayout* taskAttributeWindowLayout = new QVBoxLayout();
    taskAttributeWindowWidget->setLayout(taskAttributeWindowLayout);
    taskAttributeWindowLayout->setAlignment(Qt::AlignTop);

    // Create the group box that will contain the attributes list
    QGroupBox* attributesGroupBox = new QGroupBox("Task's Attributes");
    taskAttributeWindowLayout->addWidget(attributesGroupBox, 1);
    QVBoxLayout* attributesLayout = new QVBoxLayout();
    attributesGroupBox->setLayout(attributesLayout);

    // Create title labels for the values in the list
    QWidget* labelContainer = new QWidget();
    QGridLayout* labelContainerLayout = new QGridLayout();
    labelContainer->setLayout(labelContainerLayout);
    QLabel* nameLabel = new QLabel("Attribute Name");
    QLabel* typeLabel = new QLabel("Type");
    QLabel* valueLabel = new QLabel("Value");
    labelContainerLayout->addWidget(nameLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->addWidget(typeLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->addWidget(valueLabel, 0, 2, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->setContentsMargins(0, 0, 0, 0);
    labelContainer->setContentsMargins(0, 0, 0, 0);
    attributesLayout->addWidget(labelContainer);

    // Create the grid layout which organises the attributes
    attributesGrid = new QGridLayout();
    attributesGrid->setVerticalSpacing(0);
    QWidget* attributesGridContainer = new QWidget();
    attributesGridContainer->setLayout(attributesGrid);
    QScrollArea* attributesScroll = new QScrollArea();
    attributesScroll->setWidget(attributesGridContainer);
    attributesLayout->addWidget(attributesScroll);
    attributesScroll->setWidgetResizable(true);

    // Define a container for the bottom part of the window
    QWidget* buttonContainer = new QWidget();
    attributesLayout->addWidget(buttonContainer);
    QGridLayout* buttonGrid = new QGridLayout();
    buttonContainer->setLayout(buttonGrid);

    // Check box for defining if the task is to be considered an external or internal one
    isExternal = new QCheckBox("External Task");
    buttonGrid->addWidget(isExternal, 0, 0, Qt::AlignLeft);
    isExternal->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // If activated allow the user only to insert a command line instruction
    isExternal->connect(isExternal, &QCheckBox::stateChanged, this, &TaskCreationAttributeUI::disableAddition);

    // Go to the next page
    QPushButton* nextPageButton = new QPushButton("Next >");
    nextPageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nextPageButton->connect(nextPageButton, &QPushButton::released, this, &TaskCreationAttributeUI::nextPageSave);
    buttonGrid->addWidget(nextPageButton, 1, 4, Qt::AlignRight);

    addAttributeButton = new QPushButton("Add Attribute");
    buttonGrid->addWidget(addAttributeButton, 0, 4, Qt::AlignRight);
    addAttributeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addAttributeButton->connect(addAttributeButton, &QPushButton::released, this, &TaskCreationAttributeUI::addTaskAttribute);

    // Add the default attributes to the grid
    addDefaultTaskAttribute("Name", "string");
    addDefaultTaskAttribute("Description", "string");


}

/* Function that adds the default attributes (name - description) of the task */
void TaskCreationAttributeUI::addDefaultTaskAttribute(QString name, QString type){
    QLineEdit* attributeName = new QLineEdit(name);
    QComboBox* attributeType = new QComboBox();
    QTextEdit* valueTextEdit = new QTextEdit();

    attributeName->setReadOnly(true);
    attributeType->addItem(type);

    valueTextEdit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
    valueTextEdit->setMaximumHeight(100);
    attributesGrid->addWidget(attributeName, row, 0, Qt::AlignLeft | Qt::AlignTop);
    attributesGrid->addWidget(attributeType, row, 1, Qt::AlignLeft | Qt::AlignTop);
    attributesGrid->addWidget(valueTextEdit, row, 2, Qt::AlignLeft | Qt::AlignTop);
    attributesGrid->setRowStretch(row, 0);
    attributesGrid->setRowMinimumHeight(row, 100);
    row += 1;
}

void TaskCreationAttributeUI::nextPageSave(){
    emit nextPage(attributesGrid, isExternal->isChecked());
}


/* Function that allows the user to define its own attributes */
void TaskCreationAttributeUI::addTaskAttribute(){
    QLineEdit* attributeName = new QLineEdit();
    QComboBox* attributeType = new QComboBox();
    QSpinBox* valueSpinBox = new QSpinBox();

    attributeType->addItem("int");
    attributeType->addItem("string");
    attributeType->addItem("float");

    // Based on the type that are supported the value's box need to change
    attributeType->connect(attributeType, &QComboBox::currentIndexChanged, this, [this, attributeType]{this->changeValue(attributeType);});

    attributesGrid->addWidget(attributeName, row, 0, Qt::AlignLeft | Qt::AlignTop);
    attributesGrid->addWidget(attributeType, row, 1, Qt::AlignLeft | Qt::AlignTop);
    attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);

    attributesGrid->setRowStretch(row, 0);
    attributesGrid->setRowMinimumHeight(row, 50);
    row += 1;
}

/* Function which scope is to define which box needs to appear based on the selected type:
 * string -> QTextEdit
 * int -> QSpinBox
 * double -> QDoubleSpinBox
 * The comboBox argument is necessary to retrieve in which row the value's box needs to be changed */
void TaskCreationAttributeUI::changeValue(QComboBox* comboBox) {
    int index = attributesGrid->indexOf(comboBox);
    int row = int(index / 3);
    // Add the new widget based on the current index
    if (comboBox->currentText() == "string") {
        QTextEdit* valueTextEdit = new QTextEdit();
        // If there is another widget remove it before inserting the new one
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        attributesGrid->addWidget(valueTextEdit, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
    else if(comboBox->currentText() == "int"){
        QSpinBox* valueSpinBox = new QSpinBox();
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
    else{
        QDoubleSpinBox* valueSpinBox = new QDoubleSpinBox();
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
}

/* If the isExternal check box is flagged, then be sure to remove all the attributes inserted.
 * Then adds the new and only attribute needed: the text edit in which will be inserted the command line instruction */
void TaskCreationAttributeUI::disableAddition(int state){
    if(state){
        addAttributeButton->setDisabled(true);
        if(attributesGrid->rowCount() > 2){
            for(int i = 2; i < attributesGrid->rowCount(); i++){
                attributesGrid->itemAt(i * 3)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * 3)->widget());

                attributesGrid->itemAt(i * 3 + 1)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * 3 + 1)->widget());

                attributesGrid->itemAt(i * 3 + 2)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * 3 + 2)->widget());
            }
        }

        QLabel* name = new QLabel("Command");
        QLabel* type = new QLabel("string");
        QTextEdit* command = new QTextEdit();

        row = 2;

        attributesGrid->addWidget(name, row, 0, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->addWidget(type, row, 1, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->addWidget(command, row, 2, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->setRowStretch(row, 0);
        attributesGrid->setRowMinimumHeight(row, 50);
    }
    else{
        addAttributeButton->setDisabled(false);
    }
}

QWidget* TaskCreationAttributeUI::getTaskAttributeWindowWidget(){
    return this->taskAttributeWindowWidget;
}
