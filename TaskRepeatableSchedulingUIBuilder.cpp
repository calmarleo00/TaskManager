#include "TaskRepeatableSchedulingUIBuilder.h"
#include "qspinbox.h"


void TaskRepeatableSchedulingUIBuilder::initializeWindowStep() {
    // Imposta il layout e la dimensione della finestra
    taskFixedScheduling->setMinimumSize(500, 600);
    taskFixedScheduling->setMaximumSize(600, 800);
    taskRepeatableSchedulingLayout = new QVBoxLayout();
    taskFixedScheduling->setLayout(taskRepeatableSchedulingLayout);
    taskRepeatableSchedulingLayout->setAlignment(Qt::AlignTop);

    QGroupBox* scheduleGroupBox = new QGroupBox("Task Timing Schedule", taskFixedScheduling);
    scheduleGroupLayout = new QVBoxLayout();
    scheduleGroupBox->setLayout(scheduleGroupLayout);
    taskRepeatableSchedulingLayout->addWidget(scheduleGroupBox, 1);
}

void TaskRepeatableSchedulingUIBuilder::buildCalendarStep() {
    // Crea un group box per il calendario
    QGroupBox* calendarGroupBox = new QGroupBox("Calendar", taskFixedScheduling);
    QVBoxLayout* calendarLayout = new QVBoxLayout();
    calendarGroupBox->setLayout(calendarLayout);
    scheduleGroupLayout->addWidget(calendarGroupBox);
    calendarLayout->setAlignment(Qt::AlignTop);
    scheduleGroupLayout->setAlignment(Qt::AlignTop);

    // Imposta il calendario
    QCalendarWidget* calendar = new QCalendarWidget(calendarGroupBox);
    calendar->setMinimumDate(QDate::currentDate());
    calendarLayout->addWidget(calendar);
    connect(calendar, &QCalendarWidget::clicked, this, &TaskRepeatableSchedulingUIBuilder::setDate);

    // Crea un contenitore per i DateEdits
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

    // Widget della data di fine
    QLabel* endDate = new QLabel("End Date:", calendarGroupBox);
    endDateEdit = new QDateEdit();
    QDate end(3000, 1, 1);
    endDateEdit->setDate(end);
    dateContainerLayout->addWidget(endDate, 0, Qt::AlignRight);
    dateContainerLayout->addWidget(endDateEdit, 0, Qt::AlignRight);
}

void TaskRepeatableSchedulingUIBuilder::buildTimeStep() {
    // Crea un group box per la selezione dei tempi
    QGroupBox* hoursGroupBox = new QGroupBox("Time Selection", taskFixedScheduling);
    QHBoxLayout* hoursGroupLayout = new QHBoxLayout();
    hoursGroupBox->setLayout(hoursGroupLayout);
    scheduleGroupLayout->addWidget(hoursGroupBox);

    // Due checkbox per definire se sono da ripetere le ore o i secondi
    QWidget* checkBoxContainer = new QWidget(hoursGroupBox);
    QVBoxLayout* checkBoxLayout = new QVBoxLayout();

    checkBoxContainer->setLayout(checkBoxLayout);

    hourCheckBox = new QCheckBox("Hours", hoursGroupBox);
    secondCheckBox = new QCheckBox("Seconds", hoursGroupBox);
    hourCheckBox->setChecked(true);
    checkBoxLayout->addWidget(hourCheckBox);
    checkBoxLayout->addWidget(secondCheckBox);
    hoursGroupLayout->addWidget(checkBoxContainer, 0, Qt::AlignLeft | Qt::AlignHCenter);

    connect(hourCheckBox, &QCheckBox::stateChanged, this, &TaskRepeatableSchedulingUIBuilder::disableSecondCheckBox);
    connect(secondCheckBox, &QCheckBox::stateChanged, this, &TaskRepeatableSchedulingUIBuilder::disableHourCheckBox);

    // Definisce un container per le tempistiche da inserire
    QWidget* timeScheduleInformationContainer = new QWidget(hoursGroupBox);
    QVBoxLayout* timeScheduleInformationContainerLayout = new QVBoxLayout();
    timeScheduleInformationContainer->setLayout(timeScheduleInformationContainerLayout);
    // Container per il tempo d'inizio
    QWidget* startTimeContainer = new QWidget(timeScheduleInformationContainer);
    QHBoxLayout* startTimeContainerLayout = new QHBoxLayout();
    startTimeContainer->setLayout(startTimeContainerLayout);

    QLabel* selectStartTime = new QLabel("Select start time:", timeScheduleInformationContainer);
    startTime = new QTimeEdit();
    startTime->setMinimumTime(QTime(0, 1, 0));
    startTime->setMaximumTime(QTime(23, 59, 0));
    startTimeContainerLayout->addWidget(selectStartTime, 0, Qt::AlignLeft);
    startTimeContainerLayout->addWidget(startTime, 0, Qt::AlignRight);
    timeScheduleInformationContainerLayout->addWidget(startTimeContainer);
    // Container per il valore corrispondente alla ripetizione
    QWidget* repeatableContainer = new QWidget(hoursGroupBox);
    QHBoxLayout* repeatableContainerLayout = new QHBoxLayout();
    repeatableContainer->setLayout(repeatableContainerLayout);

    QLabel* selectRepeatable = new QLabel("How often to repeat:", repeatableContainer);
    numTime = new QSpinBox(repeatableContainer);
    numTime->setMinimum(1);
    numTime->setMaximum(100);
    repeatableContainerLayout->addWidget(selectRepeatable, 0, Qt::AlignLeft);
    repeatableContainerLayout->addWidget(numTime, 0, Qt::AlignRight);
    timeScheduleInformationContainerLayout->addWidget(repeatableContainer);
    hoursGroupLayout->addWidget(timeScheduleInformationContainer, 0, Qt::AlignRight | Qt::AlignHCenter);
}

void TaskRepeatableSchedulingUIBuilder::buttonStep() {
    QWidget* buttonContainer = new QWidget(taskFixedScheduling);
    QHBoxLayout* buttonContainerLayout = new QHBoxLayout();
    buttonContainer->setLayout(buttonContainerLayout);

    QPushButton* backButton = new QPushButton("< Back", buttonContainer);
    buttonContainerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    connect(backButton, &QPushButton::released, this, &TaskRepeatableSchedulingUIBuilder::emitBackSchedulingSelectionSignal);

    QPushButton* finishButton = new QPushButton("Finish", buttonContainer);
    buttonContainerLayout->addWidget(finishButton, 0, Qt::AlignRight);
    taskRepeatableSchedulingLayout->addWidget(buttonContainer);

    connect(finishButton, &QPushButton::released, this, &TaskRepeatableSchedulingUIBuilder::emitClosePageSignal);
}

QWidget* TaskRepeatableSchedulingUIBuilder::getResult() {
    return taskFixedScheduling;
}

void TaskRepeatableSchedulingUIBuilder::disableSecondCheckBox(int value) {
    if (value && secondCheckBox->isChecked()) {
        secondCheckBox->setChecked(false);
    }
}

void TaskRepeatableSchedulingUIBuilder::disableHourCheckBox(int value) {
    if (value && hourCheckBox->isChecked()) {
        hourCheckBox->setChecked(false);
    }
}

void TaskRepeatableSchedulingUIBuilder::setDate(QDate date) {
    if (toChangeStartDate && date < endDateEdit->date()) {
        startDateEdit->setDate(date);
        toChangeStartDate = false;
    } else if (!toChangeStartDate && date > startDateEdit->date()) {
        endDateEdit->setDate(date);
        toChangeStartDate = true;
    }
}

void TaskRepeatableSchedulingUIBuilder::emitBackSchedulingSelectionSignal() {
    emit backPageSignal();
}
void TaskRepeatableSchedulingUIBuilder::emitClosePageSignal() {
    emit closePageRepeatableSignal(hourCheckBox, secondCheckBox, startDateEdit, endDateEdit, startTime, numTime);
}


QCheckBox* TaskRepeatableSchedulingUIBuilder::getHourCheckBox(){
    return this->hourCheckBox;
}
QCheckBox* TaskRepeatableSchedulingUIBuilder::getSecondCheckBox(){
    return this->secondCheckBox;
}
QDateEdit* TaskRepeatableSchedulingUIBuilder::getStartDateEdit(){
    return this->startDateEdit;
}
QDateEdit* TaskRepeatableSchedulingUIBuilder::getEndDateEdit(){
    return this->endDateEdit;
}
QTimeEdit* TaskRepeatableSchedulingUIBuilder::getStartTime(){
    return this->startTime;
}
QSpinBox* TaskRepeatableSchedulingUIBuilder::getRepeatableAmount(){
    return this->numTime;
}
