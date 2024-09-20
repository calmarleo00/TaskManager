#include "qcheckbox.h"
#include "qgroupbox.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qscrollarea.h"
#include "qspinbox.h"
#include "qtextedit.h"
#include <TaskAttributeUI.h>
#include <AppController.h>
#include <Constants.h>
#include <QFile>
#include<QDir>
#include <QCoreApplication>

/* Aggiunge i campi predefiniti (nome e descrizione) e permette di aggiungere nuovi attributi. */
void TaskAttributeUI::setupTaskAttributeCreationUI(){
    // Set minimum and maximum size of the window
    taskAttributeWindowWidget->setMinimumSize(500, 300);
    taskAttributeWindowWidget->setMaximumSize(600, 400);

    // Set the layout and let it align everything at the top
    QVBoxLayout* taskAttributeWindowLayout = new QVBoxLayout();
    taskAttributeWindowWidget->setLayout(taskAttributeWindowLayout);
    taskAttributeWindowLayout->setAlignment(Qt::AlignTop);
    errorName = new QLabel(taskAttributeWindowWidget);
    errorName->setStyleSheet("QLabel { color : red; }");
    taskAttributeWindowLayout->addWidget(errorName, 0, Qt::AlignCenter);
    // Create the group box that will contain the attributes list
    QGroupBox* attributesGroupBox = new QGroupBox("Task's Attributes", taskAttributeWindowWidget);
    taskAttributeWindowLayout->addWidget(attributesGroupBox, 1);
    QVBoxLayout* attributesLayout = new QVBoxLayout();
    attributesGroupBox->setLayout(attributesLayout);

    // Create title labels for the values in the list
    QWidget* labelContainer = new QWidget(attributesGroupBox);
    QGridLayout* labelContainerLayout = new QGridLayout();
    labelContainer->setLayout(labelContainerLayout);
    QLabel* nameLabel = new QLabel("Attribute Name", labelContainer);
    QLabel* typeLabel = new QLabel("Type", labelContainer);
    QLabel* valueLabel = new QLabel("Value", labelContainer);
    labelContainerLayout->addWidget(nameLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->addWidget(typeLabel, 0, 1, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->addWidget(valueLabel, 0, 2, Qt::AlignLeft | Qt::AlignTop);
    labelContainerLayout->setContentsMargins(0, 0, 0, 0);
    labelContainer->setContentsMargins(0, 0, 0, 0);
    attributesLayout->addWidget(labelContainer);

    // Create the grid layout which organises the attributes
    attributesGrid = new QGridLayout();
    attributesGrid->setVerticalSpacing(0);
    QWidget* attributesGridContainer = new QWidget(taskAttributeWindowWidget);
    attributesGridContainer->setLayout(attributesGrid);
    QScrollArea* attributesScroll = new QScrollArea();
    attributesScroll->setWidget(attributesGridContainer);
    attributesLayout->addWidget(attributesScroll);
    attributesScroll->setWidgetResizable(true);

    // Define a container for the bottom part of the window
    QWidget* buttonContainer = new QWidget(taskAttributeWindowWidget);
    attributesLayout->addWidget(buttonContainer);
    QGridLayout* buttonGrid = new QGridLayout();
    buttonContainer->setLayout(buttonGrid);

    // Check box for defining if the task is to be considered an external or internal one
    isExternal = new QCheckBox("External Task", buttonContainer);
    buttonGrid->addWidget(isExternal, 0, 0, Qt::AlignLeft);
    isExternal->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // If activated allow the user only to insert a command line instruction
    isExternal->connect(isExternal, &QCheckBox::stateChanged, this, &TaskAttributeUI::disableAddition);

    // Go to the next page
    nextPageButton = new QPushButton("Next >", buttonContainer);
    nextPageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    nextPageButton->connect(nextPageButton, &QPushButton::released, this, &TaskAttributeUI::nextPageCreate);
    buttonGrid->addWidget(nextPageButton, 1, 4, Qt::AlignRight);

    addAttributeButton = new QPushButton("Add Attribute", buttonContainer);
    buttonGrid->addWidget(addAttributeButton, 0, 4, Qt::AlignRight);
    addAttributeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addAttributeButton->connect(addAttributeButton, &QPushButton::released, this, &TaskAttributeUI::addTaskAttribute);
    // Add the default attributes to the grid
    addDefaultTaskAttribute("Name", "string");
    addDefaultTaskAttribute("Description", "string");
}

/* Function that adds the default attributes (name - description) of the task */
void TaskAttributeUI::addDefaultTaskAttribute(QString name, QString type){
    QLineEdit* attributeName = new QLineEdit(name, taskAttributeWindowWidget);
    QComboBox* attributeType = new QComboBox(taskAttributeWindowWidget);
    QTextEdit* valueTextEdit = new QTextEdit(taskAttributeWindowWidget);
    if("Name" == name)
        valueTextEdit->setText("Default");
    valueTextEdit->setMaximumSize(300, 200);
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

void TaskAttributeUI::nextPageCreate(){
    if(checkTaskAlreadyCreated() && (isExternal->isChecked() || checkInternalTaskExistence()))
        emit nextPage(attributesGrid, isExternal->isChecked());
}

void TaskAttributeUI::nextPageUpdate(){
    emit nextPage(attributesGrid, isExternal->isChecked());
}

/* Function that allows the user to define its own attributes */
void TaskAttributeUI::addTaskAttribute(){
    QLineEdit* attributeName = new QLineEdit(taskAttributeWindowWidget);
    QComboBox* attributeType = new QComboBox(taskAttributeWindowWidget);
    QSpinBox* valueSpinBox = new QSpinBox(taskAttributeWindowWidget);
    valueSpinBox->setMinimum(-MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
    valueSpinBox->setMaximum(MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
    attributeType->addItem("int");
    attributeType->addItem("string");
    attributeType->addItem("double");

    // Based on the type that are supported the value's box need to change
    attributeType->connect(attributeType, &QComboBox::currentTextChanged, this, [this, attributeType]{this->changeValue(attributeType);});

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
void TaskAttributeUI::changeValue(QComboBox* comboBox) {
    int index = attributesGrid->indexOf(comboBox);
    int row = int(index / GRID_COLUMNS);
    // Add the new widget based on the current index
    if (comboBox->currentText() == "string") {
        QTextEdit* valueTextEdit = new QTextEdit(taskAttributeWindowWidget);
        // If there is another widget remove it before inserting the new one
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        valueTextEdit->setMaximumSize(300, 200);
        attributesGrid->addWidget(valueTextEdit, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
    else if(comboBox->currentText() == "int"){
        QSpinBox* valueSpinBox = new QSpinBox(taskAttributeWindowWidget);
        valueSpinBox->setMinimum(-MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
        valueSpinBox->setMaximum(MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
    else{
        QDoubleSpinBox* valueSpinBox = new QDoubleSpinBox(taskAttributeWindowWidget);
        valueSpinBox->setMinimum(-MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
        valueSpinBox->setMaximum(MAXIMUM_ATTRIBUTE_SPINBOX_VALUE);
        if(attributesGrid->itemAt(index + 1)){
            attributesGrid->itemAt(index + 1)->widget()->deleteLater();
            attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
        }
        attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);
    }
}

/* If the isExternal check box is flagged, then be sure to remove all the attributes inserted.
 * Then adds the new and only attribute needed: the text edit in which will be inserted the command line instruction */
void TaskAttributeUI::disableAddition(int state){
    if(state){
        addAttributeButton->setDisabled(true);
        if(row > 2){
            for(int i = 2; i < attributesGrid->rowCount(); i++){
                attributesGrid->itemAt(i * GRID_COLUMNS)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * GRID_COLUMNS)->widget());

                attributesGrid->itemAt(i * GRID_COLUMNS)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * GRID_COLUMNS)->widget());

                attributesGrid->itemAt(i * GRID_COLUMNS)->widget()->deleteLater();
                attributesGrid->removeWidget(attributesGrid->itemAt(i * GRID_COLUMNS)->widget());
            }
        }

        QLabel* name = new QLabel("Command", taskAttributeWindowWidget);
        QLabel* type = new QLabel("string", taskAttributeWindowWidget);
        QTextEdit* command = new QTextEdit(taskAttributeWindowWidget);

        row = 2;

        attributesGrid->addWidget(name, row, 0, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->addWidget(type, row, 1, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->addWidget(command, row, 2, Qt::AlignLeft | Qt::AlignTop);
        attributesGrid->setRowStretch(row, 0);
        attributesGrid->setRowMinimumHeight(row, 50);
    }
    else{
        attributesGrid->itemAt(row * GRID_COLUMNS)->widget()->deleteLater();
        attributesGrid->removeWidget(attributesGrid->itemAt(row * GRID_COLUMNS)->widget());

        attributesGrid->itemAt(row * GRID_COLUMNS)->widget()->deleteLater();
        attributesGrid->removeWidget(attributesGrid->itemAt(row * GRID_COLUMNS)->widget());

        attributesGrid->itemAt(row * 3)->widget()->deleteLater();
        attributesGrid->removeWidget(attributesGrid->itemAt(row * GRID_COLUMNS)->widget());

        addAttributeButton->setDisabled(false);
    }
}

QWidget* TaskAttributeUI::getTaskAttributeWindowWidget(){
    return this->taskAttributeWindowWidget;
}

QGridLayout* TaskAttributeUI::getAttributesGrid(){
    return this->attributesGrid;
}

int TaskAttributeUI::getAttributeRow(){
    return this->row;
}

bool TaskAttributeUI::getIsExternal(){
    return this->isExternal->isChecked();
}

void TaskAttributeUI::setIsExternal(bool isExternal){
    this->isExternal->setChecked(isExternal);
}


bool TaskAttributeUI::checkInternalTaskExistence(){
    QTextEdit* taskNameEdit = qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget());
    QString taskNameInserted = taskNameEdit->toPlainText();
    QString projectPath = QDir::currentPath() + "/../..";
    QString filePath = QDir(projectPath).filePath(taskNameInserted + ".cpp");
    if(!QFile::exists(filePath)){
        taskNameEdit->setStyleSheet("background-color: red;");
        errorName->setText("The class file for this task does not exist! \nPlease be sure that the chosen name of the internal task corresponds to an existent file");
        return false;
    }
    else{
        taskNameEdit->setStyleSheet("background-color: white;");
        errorName->setText("");
        return true;
    }
}

bool TaskAttributeUI::checkTaskAlreadyCreated(){
    QTextEdit* taskNameEdit = qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget());
    Task* iterTask = AppController::getInstance()->taskHead;
    while(iterTask){
        if(iterTask->getTaskName() == taskNameEdit->toPlainText()){
            errorName->setText("A task with the chosen name already exist!");
            return false;
        }
        iterTask = iterTask->getTaskNext();
    }
    errorName->setText("");
    return true;
}

QPushButton* TaskAttributeUI::getNextPageButton(){
    return this->nextPageButton;
}




