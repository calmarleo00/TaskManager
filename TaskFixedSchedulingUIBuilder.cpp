#include "TaskFixedSchedulingUIBuilder.h"
#include "qcalendarwidget.h"
#include "qgroupbox.h"
#include "qlabel.h"
#include "qscrollarea.h"


void TaskFixedSchedulingUIBuilder::initializeWindowStep() {
    // Set window size and layout
    taskFixedScheduling->setMinimumSize(500, 600);
    taskFixedScheduling->setMaximumSize(600, 800);
    taskFixedSchedulingLayout = new QVBoxLayout();
    taskFixedScheduling->setLayout(taskFixedSchedulingLayout);
    taskFixedSchedulingLayout->setAlignment(Qt::AlignTop);

    QGroupBox* scheduleGroupBox = new QGroupBox("Task's Timing Schedule", taskFixedScheduling);
    scheduleGroupLayout = new QVBoxLayout();
    scheduleGroupBox->setLayout(scheduleGroupLayout);
    taskFixedSchedulingLayout->addWidget(scheduleGroupBox, 1);
}

void TaskFixedSchedulingUIBuilder::buildCalendarStep() {
    // Crea un Group Box per il calendar
    QGroupBox* calendarGroupBox = new QGroupBox("Calendar", taskFixedScheduling);
    QVBoxLayout* calendarLayout = new QVBoxLayout();
    calendarGroupBox->setLayout(calendarLayout);
    scheduleGroupLayout->addWidget(calendarGroupBox);
    calendarLayout->setAlignment(Qt::AlignTop);
    scheduleGroupLayout->setAlignment(Qt::AlignTop);

    // Imposta il calendario affinché l'utente possa selezionare la data d'inizio e di fine per ogni task
    QCalendarWidget* calendar = new QCalendarWidget(calendarGroupBox);
    calendar->setMinimumDate(QDate::currentDate());
    calendarLayout->addWidget(calendar);
    connect(calendar, &QCalendarWidget::clicked, this, &TaskFixedSchedulingUIBuilder::setDate);

    // Crea il container in cui sono posizionati i QDateEdit per la data d'inizio e per la data di fine
    QWidget* dateContainer = new QWidget(calendarGroupBox);
    calendarLayout->addWidget(dateContainer);
    QHBoxLayout* dateContainerLayout = new QHBoxLayout();
    dateContainer->setLayout(dateContainerLayout);

    // Widget della data d'inizio
    QLabel* startDate = new QLabel("Start Date:", calendarGroupBox);
    startDateEdit = new QDateEdit();
    startDateEdit->setDate(QDate::currentDate());
    dateContainerLayout->addWidget(startDate, 0, Qt::AlignLeft);
    dateContainerLayout->addWidget(startDateEdit, 0, Qt::AlignLeft);

    // Widget per la data di fine
    QLabel* endDate = new QLabel("End Date:", calendarGroupBox);
    endDateEdit = new QDateEdit();
    QDate end(3000, 1, 1);
    endDateEdit->setDate(end);
    dateContainerLayout->addWidget(endDate, 0, Qt::AlignRight);
    dateContainerLayout->addWidget(endDateEdit, 0, Qt::AlignRight);
}

void TaskFixedSchedulingUIBuilder::buildDaysStep() {
    // Crea un group box per la selezione dei giorni
    QGroupBox* daysGroupBox = new QGroupBox("Days Selection", taskFixedScheduling);
    QHBoxLayout* daysLayout = new QHBoxLayout();
    daysGroupBox->setLayout(daysLayout);
    scheduleGroupLayout->addWidget(daysGroupBox);

    QStringList dayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < 7; ++i) {
        daysCheckBoxes[i].setText(dayNames[i]);
        daysCheckBoxes[i].setParent(daysGroupBox);
        daysLayout->addWidget(&daysCheckBoxes[i]);
        daysCheckBoxes[i].connect(&daysCheckBoxes[i], &QCheckBox::stateChanged, this, [this, i]{this->dayTimeAppear(i);});
    }
}

void TaskFixedSchedulingUIBuilder::buildTimeStep() {
    // Crea un gruppo per la selezione dei tempi
    QGroupBox* hoursGroupBox = new QGroupBox("Time Selection", taskFixedScheduling);
    QVBoxLayout* hoursGroupLayout = new QVBoxLayout();
    hoursGroupBox->setLayout(hoursGroupLayout);
    QStringList dayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    QScrollArea* mainScroll = new QScrollArea();
    scheduleGroupLayout->addWidget(mainScroll);

    mainScroll->setWidget(hoursGroupBox);
    mainScroll->setWidgetResizable(true);
    for(int i = 0; i < 7; i++){
        QHBoxLayout* dayTimeContainerLayout = new QHBoxLayout();
        daysTimeWidgets[i].setLayout(dayTimeContainerLayout);

        QLabel* dayName = new QLabel(dayNames[i], &daysTimeWidgets[i]);
        dayTimeContainerLayout->addWidget(dayName, 0, Qt::AlignLeft | Qt::AlignHCenter);

        QWidget* dayTimesContainer = new QWidget(&daysTimeWidgets[i]);
        QVBoxLayout* dayTimesLayout = new QVBoxLayout();
        dayTimesContainer->setLayout(dayTimesLayout);
        dayTimeContainerLayout->addWidget(dayTimesContainer, 1);


        QWidget* timeContainer = new QWidget(&daysTimeWidgets[i]);
        timeContainer->setLayout(&timeContainerLayout[i]);
        QScrollArea* scrollArea = new QScrollArea();
        scrollArea->setWidget(timeContainer);
        scrollArea->setWidgetResizable(true);

        QTimeEdit* timeEdit = new QTimeEdit(timeContainer);
        QPushButton* addTimeButton = new QPushButton("Add new time", dayTimesContainer);

        timeContainerLayout[i].addWidget(timeEdit, 0, Qt::AlignTop);
        dayTimesLayout->addWidget(scrollArea, 1);
        dayTimesLayout->addWidget(addTimeButton, 0, Qt::AlignTop);
        addTimeButton->connect(addTimeButton, &QPushButton::released, this, [this, i]{this->addTime(i);});

        hoursGroupLayout->addWidget(&daysTimeWidgets[i], 1);
        daysTimeWidgets[i].hide();
    }

}

void TaskFixedSchedulingUIBuilder::buttonStep() {
    QWidget* buttonContainer = new QWidget(taskFixedScheduling);
    QHBoxLayout* buttonContainerLayout = new QHBoxLayout();
    buttonContainer->setLayout(buttonContainerLayout);

    QPushButton* backButton = new QPushButton("< Back", buttonContainer);
    buttonContainerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    connect(backButton, &QPushButton::released, this, &TaskFixedSchedulingUIBuilder::emitBackSchedulingSelectionSignal);

    QPushButton* finishButton = new QPushButton("Finish", buttonContainer);
    buttonContainerLayout->addWidget(finishButton, 0, Qt::AlignRight);
    taskFixedSchedulingLayout->addWidget(buttonContainer);

    connect(finishButton, &QPushButton::released, this, &TaskFixedSchedulingUIBuilder::emitClosePageSignal);
}

QWidget* TaskFixedSchedulingUIBuilder::getResult() {
    return taskFixedScheduling;
}


void TaskFixedSchedulingUIBuilder::dayTimeAppear(int day){
    if(daysCheckBoxes[day].isChecked()){
        daysTimeWidgets[day].show();
    }
    else{
        daysTimeWidgets[day].hide();
    }
}

void TaskFixedSchedulingUIBuilder::addTime(int day){
    QTimeEdit* timeEdit = new QTimeEdit(taskFixedScheduling);
    timeContainerLayout[day].addWidget(timeEdit);

}

void TaskFixedSchedulingUIBuilder::setDate(QDate date) {
    if (toChangeStartDate && date < endDateEdit->date()) {
        startDateEdit->setDate(date);
        toChangeStartDate = false;
    } else if (!toChangeStartDate && date > startDateEdit->date()) {
        endDateEdit->setDate(date);
        toChangeStartDate = true;
    }
}

void TaskFixedSchedulingUIBuilder::emitBackSchedulingSelectionSignal() {
    emit backPageSignal();
}
void TaskFixedSchedulingUIBuilder::emitClosePageSignal() {
    emit closePageFixedSignal(daysCheckBoxes, timeContainerLayout, startDateEdit, endDateEdit);
}

QDateEdit* TaskFixedSchedulingUIBuilder::getStartDateEdit(){
    return this->startDateEdit;
}

QDateEdit* TaskFixedSchedulingUIBuilder::getEndDateEdit(){
    return this->endDateEdit;
}

QCheckBox* TaskFixedSchedulingUIBuilder::getDaysCheckBoxes(){
    return this->daysCheckBoxes;
}

QVBoxLayout* TaskFixedSchedulingUIBuilder::getTimeContainerLayout(){
    return this->timeContainerLayout;
}
