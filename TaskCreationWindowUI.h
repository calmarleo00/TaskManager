#include "qcalendarwidget.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qdatetimeedit.h"
#include "qgroupbox.h"
#include "qspinbox.h"
#include "qstackedwidget.h"
#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>
#include <QTextEdit>
#include <QSplitter>
#include <QSizeGrip>
#include <QRect>
#ifndef TASKCREATIONWINDOWUI_H
#define TASKCREATIONWINDOWUI_H
class TaskCreationWindowUI : public QWidget{
    QWidget* taskCreationWindowWidget = nullptr;
    QVBoxLayout* taskCreationWindowLayout = nullptr;
    int row = 0;
    QGridLayout* attributesGrid = nullptr;
    QPushButton* addAttributeButton = nullptr;
    QStackedWidget* widgets = new QStackedWidget();
    QWidget* taskTimerWindowWidget = nullptr;
    QVBoxLayout* taskTimerWindowLayout = nullptr;
    QCheckBox* hourCheckBox = nullptr;
    QCheckBox* secondCheckBox = nullptr;
    bool toChangeStartDate = true;
    QDateEdit* startDateEdit = nullptr;
    QDateEdit* endDateEdit = nullptr;
    public:
        TaskCreationWindowUI(){
            taskCreationWindowWidget = new QWidget();
            taskCreationWindowLayout = new QVBoxLayout();

            taskTimerWindowWidget = new QWidget();
            taskTimerWindowLayout = new QVBoxLayout();
        }
    public:
        void setupTaskUI(){
            setupTaskCreationUI();
            setupTaskTimerUI();

            widgets->addWidget(taskCreationWindowWidget);
            widgets->addWidget(taskTimerWindowWidget);

            widgets->setCurrentWidget(taskCreationWindowWidget);
            widgets->show();
        }


        void setupTaskCreationUI(){
            taskCreationWindowWidget->setMinimumSize(300, 200);
            taskCreationWindowWidget->setMaximumSize(500, 600);
            taskCreationWindowWidget->setLayout(taskCreationWindowLayout);
            taskCreationWindowLayout->setAlignment(Qt::AlignTop);
            QGroupBox* attributesGroupBox = new QGroupBox("Task's Attributes");
            taskCreationWindowLayout->addWidget(attributesGroupBox, 1);

            QVBoxLayout* attributesLayout = new QVBoxLayout();
            attributesGroupBox->setLayout(attributesLayout);

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

            attributesGrid = new QGridLayout();
            QWidget* attributesGridContainer = new QWidget();

            attributesGridContainer->setLayout(attributesGrid);
            QScrollArea* attributesScroll = new QScrollArea();

            attributesScroll->setWidget(attributesGridContainer);

            attributesLayout->addWidget(attributesScroll);
            attributesScroll->setWidgetResizable(true);

            QWidget* buttonContainer = new QWidget();
            attributesLayout->addWidget(buttonContainer);

            QGridLayout* buttonGrid = new QGridLayout();
            buttonContainer->setLayout(buttonGrid);

            QCheckBox* isExternal = new QCheckBox("External Task");
            buttonGrid->addWidget(isExternal, 0, 0, Qt::AlignLeft);
            isExternal->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            isExternal->connect(isExternal, &QCheckBox::stateChanged, this, &TaskCreationWindowUI::disableAddition);

            QPushButton* nextPageButton = new QPushButton("Next >");
            addAttributeButton = new QPushButton("Add Attribute");

            nextPageButton->connect(nextPageButton, &QPushButton::released, this, &TaskCreationWindowUI::nextPage);
            buttonGrid->addWidget(addAttributeButton, 0, 4, Qt::AlignRight);
            buttonGrid->addWidget(nextPageButton, 1, 4, Qt::AlignRight);

            nextPageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            addAttributeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            addAttributeButton->connect(addAttributeButton, &QPushButton::released, this, &TaskCreationWindowUI::addTaskAttribute);

            attributesGrid->setVerticalSpacing(0);

            addDefaultTaskAttribute("Name", "string");
            addDefaultTaskAttribute("Description", "string");


        }

        void addDefaultTaskAttribute(QString name, QString type){
            QLabel* attributeName = new QLabel(name);
            QLabel* attributeType = new QLabel(type);
            QTextEdit* valueTextEdit = new QTextEdit();


            valueTextEdit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum);
            valueTextEdit->setMaximumHeight(100);
            attributesGrid->addWidget(attributeName, row, 0, Qt::AlignLeft | Qt::AlignTop);
            attributesGrid->addWidget(attributeType, row, 1, Qt::AlignLeft | Qt::AlignTop);
            attributesGrid->addWidget(valueTextEdit, row, 2, Qt::AlignLeft | Qt::AlignTop);
            attributesGrid->setRowStretch(row, 0);
            attributesGrid->setRowMinimumHeight(row, 100);
            row += 1;
        }

        void addTaskAttribute(){
            QLineEdit* attributeName = new QLineEdit();
            QComboBox* attributeType = new QComboBox();
            QSpinBox* valueSpinBox = new QSpinBox();

            attributeType->addItem("int");
            attributeType->addItem("string");
            attributeType->addItem("float");

            attributeType->connect(attributeType, &QComboBox::currentIndexChanged, this, [this, attributeType]{this->changeValue(this->attributesGrid, attributeType);});
            attributesGrid->addWidget(attributeName, row, 0, Qt::AlignLeft | Qt::AlignTop);
            attributesGrid->addWidget(attributeType, row, 1, Qt::AlignLeft | Qt::AlignTop);
            attributesGrid->addWidget(valueSpinBox, row, 2, Qt::AlignLeft | Qt::AlignTop);

            attributesGrid->setRowStretch(row, 0);
            attributesGrid->setRowMinimumHeight(row, 50);
            row += 1;
        }

        void changeValue(QGridLayout* attributesGrid, QComboBox* comboBox) {
            int index = attributesGrid->indexOf(comboBox);
            int row = int(index / 3);
            qDebug() << row;
            // Add the new widget based on the current index
            if (comboBox->currentText() == "string") {
                QTextEdit* valueTextEdit = new QTextEdit();
                if(attributesGrid->itemAt(index + 1)){
                    attributesGrid->itemAt(index + 1)->widget()->deleteLater();
                    attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
                }
                attributesGrid->addWidget(valueTextEdit, row + 1, 2, Qt::AlignLeft | Qt::AlignTop);
            } else {
                QSpinBox* valueSpinBox = new QSpinBox();
                if(attributesGrid->itemAt(index + 1)){
                    attributesGrid->itemAt(index + 1)->widget()->deleteLater();
                    attributesGrid->removeWidget(attributesGrid->itemAt(index + 1)->widget());
                }
                attributesGrid->addWidget(valueSpinBox, row + 1, 2, Qt::AlignLeft | Qt::AlignTop);
            }
        }

        void disableAddition(int state){
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

        void setupTaskTimerUI(){
            taskTimerWindowWidget->setMinimumSize(200, 300);
            taskTimerWindowWidget->setLayout(taskTimerWindowLayout);
            taskTimerWindowLayout->setAlignment(Qt::AlignTop);
            QGroupBox* timerGroupBox = new QGroupBox("Task Timing Schedule");
            QVBoxLayout* timerGroupLayout = new QVBoxLayout();
            timerGroupBox->setLayout(timerGroupLayout);
            taskTimerWindowLayout->addWidget(timerGroupBox, 1);

            QGroupBox* calendarGroupBox = new QGroupBox("Calendar");
            QVBoxLayout* calendarLayout = new QVBoxLayout();
            calendarGroupBox->setLayout(calendarLayout);
            timerGroupLayout->addWidget(calendarGroupBox);

            calendarLayout->setAlignment(Qt::AlignTop);
            timerGroupLayout->setAlignment(Qt::AlignTop);

            QCalendarWidget* calendar = new QCalendarWidget();
            calendarLayout->addWidget(calendar);
            calendar->connect(calendar, &QCalendarWidget::clicked, this, &TaskCreationWindowUI::setDate);
            QWidget* dateContainer = new QWidget();
            QHBoxLayout* dateContainerLayout = new QHBoxLayout();
            dateContainer->setLayout(dateContainerLayout);

            QLabel* startDate = new QLabel("Start Date:");
            startDateEdit = new QDateEdit();

            dateContainerLayout->addWidget(startDate, 0, Qt::AlignLeft);
            dateContainerLayout->addWidget(startDateEdit, 0, Qt::AlignLeft);


            QLabel* endDate = new QLabel("End Date:");
            endDateEdit = new QDateEdit();
            QDate end(3000, 1, 1);

            endDateEdit->setDate(end);

            dateContainerLayout->addWidget(endDate, 0, Qt::AlignRight);
            dateContainerLayout->addWidget(endDateEdit, 0, Qt::AlignRight);

            calendarLayout->addWidget(dateContainer);

            QGroupBox* daysGroupBox = new QGroupBox("Days Selection");
            QHBoxLayout* daysLayout = new QHBoxLayout();
            daysGroupBox->setLayout(daysLayout);

            timerGroupLayout->addWidget(daysGroupBox);

            QCheckBox* monday = new QCheckBox("Monday");
            QCheckBox* tuesday = new QCheckBox("Tuesday");
            QCheckBox* wednesday = new QCheckBox("Wednesday");
            QCheckBox* thursday = new QCheckBox("Thursday");
            QCheckBox* friday = new QCheckBox("Friday");
            QCheckBox* saturday = new QCheckBox("Saturday");
            QCheckBox* sunday = new QCheckBox("Sunday");

            daysLayout->addWidget(monday);
            daysLayout->addWidget(tuesday);
            daysLayout->addWidget(wednesday);
            daysLayout->addWidget(thursday);
            daysLayout->addWidget(friday);
            daysLayout->addWidget(saturday);
            daysLayout->addWidget(sunday);

            QGroupBox* hoursGroupBox = new QGroupBox("Time Selection");
            QHBoxLayout* hoursGroupLayout = new QHBoxLayout();
            hoursGroupBox->setLayout(hoursGroupLayout);

            timerGroupLayout->addWidget(hoursGroupBox);

            QWidget* checkBoxContainer = new QWidget();
            QVBoxLayout* checkBoxLayout = new QVBoxLayout();
            checkBoxContainer->setLayout(checkBoxLayout);

            hourCheckBox = new QCheckBox("Hours");
            secondCheckBox = new QCheckBox("Seconds");

            checkBoxLayout->addWidget(hourCheckBox);
            checkBoxLayout->addWidget(secondCheckBox);

            hoursGroupLayout->addWidget(checkBoxContainer, 0, Qt::AlignLeft);

            QSpinBox* numTime = new QSpinBox();

            hoursGroupLayout->addWidget(numTime, 0, Qt::AlignRight);

            QWidget* buttonContainer = new QWidget();
            QHBoxLayout* buttonContainerLayout = new QHBoxLayout();
            buttonContainer->setLayout(buttonContainerLayout);


            QPushButton* backButton = new QPushButton("< Back");
            buttonContainerLayout->addWidget(backButton, 0, Qt::AlignLeft);
            backButton->connect(backButton, &QPushButton::released, this, &TaskCreationWindowUI::backPage);

            hourCheckBox->connect(hourCheckBox, &QCheckBox::stateChanged, this, &TaskCreationWindowUI::disableSecondCheckBox);
            secondCheckBox->connect(secondCheckBox, &QCheckBox::stateChanged, this, &TaskCreationWindowUI::disableHourCheckBox);

            QPushButton* finishButton = new QPushButton("Finish");
            buttonContainerLayout->addWidget(finishButton, 0, Qt::AlignRight);
            timerGroupLayout->addWidget(buttonContainer);
        }

        void nextPage(){
            widgets->setCurrentWidget(taskTimerWindowWidget);
            widgets->show();
        }

        void backPage(){
            widgets->setCurrentWidget(taskCreationWindowWidget);
            widgets->show();
        }

        void disableSecondCheckBox(int value){
            if(value && secondCheckBox->isChecked()){
                secondCheckBox->setChecked(false);
            }
        }

        void disableHourCheckBox(int value){
            if(value && hourCheckBox->isChecked()){
                hourCheckBox->setChecked(false);
            }
        }

        void setDate(QDate date){
            if(toChangeStartDate && date < endDateEdit->date()){
                startDateEdit->setReadOnly(false);
                startDateEdit->setDate(date);
                toChangeStartDate = false;
                startDateEdit->setReadOnly(true);
            }
            else if(!toChangeStartDate && date > startDateEdit->date()){
                endDateEdit->setDate(date);
                toChangeStartDate = true;
            }
        }
};

#endif // TASKCREATIONWINDOWUI_H
