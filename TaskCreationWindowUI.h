#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>
#ifndef TASKCREATIONWINDOWUI_H
#define TASKCREATIONWINDOWUI_H
class TaskCreationWindowUI : public QObject{
    public:
        void setupTaskCreationUI(){
            QWidget* taskCreationWindow = new QWidget();
            taskCreationWindow->setMinimumSize(300, 550);
            QLabel* nameAttribute = new QLabel("Attribute");
            QLabel* typeAttribute = new QLabel("Type");
            QLabel* valueAttribute = new QLabel("Value");
            QGridLayout* mainGrid = new QGridLayout();
            mainGrid->addWidget(nameAttribute, 0, 0, 1, 1);
            mainGrid->addWidget(typeAttribute, 0, 3, 1, 1);
            mainGrid->addWidget(valueAttribute, 0, 4, 1, 1);
            taskCreationWindow->setLayout(mainGrid);
            QGridLayout* attributeGrid = new QGridLayout();
            QWidget* containerWidget = new QWidget(taskCreationWindow);
            QScrollArea* scrollArea = new QScrollArea();
            containerWidget->setLayout(attributeGrid);
            scrollArea->setWidget(containerWidget);
            scrollArea->setWidgetResizable(true);
            mainGrid->addWidget(scrollArea, 1, 0, 1, 5);
            QPushButton* addAttributeButton = new QPushButton("Add Attribute");
            mainGrid->addWidget(addAttributeButton, 2, 4, 1, 1);
            //addAttributeButton->connect(addAttributeButton, &QPushButton::released, this, )
            QLabel* nameName = new QLabel("Name");
            QLabel* nameType = new QLabel("String");
            QLineEdit* nameValue = new QLineEdit();
            nameValue->setMinimumSize(100, 20);
            nameValue->setMaximumSize(100, 30);
            attributeGrid->addWidget(nameName, 0, 0, 1, 4);
            attributeGrid->addWidget(nameType, 0, 4, 1, 1);
            attributeGrid->addWidget(nameValue, 0, 5, 1, 1);

            QLabel* descritionName = new QLabel("Description");
            QLabel* descriptionType = new QLabel("String");
            QLineEdit* descriptionValue = new QLineEdit();
            descriptionValue->setMinimumSize(100, 20);
            descriptionValue->setMaximumSize(100, 30);
            attributeGrid->addWidget(descritionName, 1, 0, 1, 4);
            attributeGrid->addWidget(descriptionType, 1, 4, 1, 1);
            attributeGrid->addWidget(descriptionValue, 1, 5, 1, 1);

            attributeGrid->setAlignment(Qt::AlignTop);

            nameName->show();
            addAttributeButton->show();
            taskCreationWindow->show();
        }

        void addTaskAttributeUI(){

        }
};

#endif // TASKCREATIONWINDOWUI_H
