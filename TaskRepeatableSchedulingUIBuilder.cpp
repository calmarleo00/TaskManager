#include "TaskRepeatableSchedulingUIBuilder.h"
#include "qspinbox.h"

TaskRepeatableSchedulingUIBuilder::TaskRepeatableSchedulingUIBuilder() {
    reset();
}

TaskRepeatableSchedulingUIBuilder::~TaskRepeatableSchedulingUIBuilder() {
    delete taskRepeatableSchedulingWidget;
    delete[] daysCheckBoxes;
}

void TaskRepeatableSchedulingUIBuilder::reset() {
    taskRepeatableSchedulingWidget = new QWidget();
    daysCheckBoxes = new QCheckBox[7];
}

void TaskRepeatableSchedulingUIBuilder::initializeWindowStep() {
    // Set window size and layout
    taskRepeatableSchedulingWidget->setMinimumSize(500, 600);
    taskRepeatableSchedulingWidget->setMaximumSize(600, 800);
    taskRepeatableSchedulingLayout = new QVBoxLayout();
    taskRepeatableSchedulingWidget->setLayout(taskRepeatableSchedulingLayout);
    taskRepeatableSchedulingLayout->setAlignment(Qt::AlignTop);

    QGroupBox* scheduleGroupBox = new QGroupBox("Task Timing Schedule");
    scheduleGroupLayout = new QVBoxLayout();
    scheduleGroupBox->setLayout(scheduleGroupLayout);
    taskRepeatableSchedulingLayout->addWidget(scheduleGroupBox, 1);
}

void TaskRepeatableSchedulingUIBuilder::buildCalendarStep() {
    // Create a group box for the calendar widget
    QGroupBox* calendarGroupBox = new QGroupBox("Calendar");
    QVBoxLayout* calendarLayout = new QVBoxLayout();
    calendarGroupBox->setLayout(calendarLayout);
    scheduleGroupLayout->addWidget(calendarGroupBox);
    calendarLayout->setAlignment(Qt::AlignTop);
    scheduleGroupLayout->setAlignment(Qt::AlignTop);

    // Set calendar on which is selected the period range when the process should be executed
    QCalendarWidget* calendar = new QCalendarWidget();
    calendarLayout->addWidget(calendar);
    connect(calendar, &QCalendarWidget::clicked, this, &TaskRepeatableSchedulingUIBuilder::setDate);

    // Create the container widget in which are positioned the DateEdits
    QWidget* dateContainer = new QWidget();
    calendarLayout->addWidget(dateContainer);
    dateContainerLayout = new QHBoxLayout();
    dateContainer->setLayout(dateContainerLayout);

    // Start date widget
    QLabel* startDate = new QLabel("Start Date:");
    startDateEdit = new QDateEdit();
    dateContainerLayout->addWidget(startDate, 0, Qt::AlignLeft);
    dateContainerLayout->addWidget(startDateEdit, 0, Qt::AlignLeft);

    // End date widget
    QLabel* endDate = new QLabel("End Date:");
    endDateEdit = new QDateEdit();
    QDate end(3000, 1, 1);
    endDateEdit->setDate(end);
    dateContainerLayout->addWidget(endDate, 0, Qt::AlignRight);
    dateContainerLayout->addWidget(endDateEdit, 0, Qt::AlignRight);
}

/*void TaskRepeatableSchedulingUIBuilder::buildDaysStep() {
    // Create a group box for days selection
    QGroupBox* daysGroupBox = new QGroupBox("Days Selection");
    daysLayout = new QHBoxLayout();
    daysGroupBox->setLayout(daysLayout);
    scheduleGroupLayout->addWidget(daysGroupBox);

    QStringList dayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < 7; ++i) {
        daysCheckBoxes[i].setText(dayNames[i]);
        daysCheckBoxes[i].setParent(daysGroupBox);
        daysLayout->addWidget(&daysCheckBoxes[i]);
    }
}*/

void TaskRepeatableSchedulingUIBuilder::buildTimeStep() {
    // Create a group box for time selection
    QGroupBox* hoursGroupBox = new QGroupBox("Time Selection");
    QHBoxLayout* hoursGroupLayout = new QHBoxLayout();
    hoursGroupBox->setLayout(hoursGroupLayout);
    scheduleGroupLayout->addWidget(hoursGroupBox);

    // Define a container for the checkboxes that indicates whether seconds or hours are considered
    QWidget* checkBoxContainer = new QWidget();
    checkBoxLayout = new QVBoxLayout();

    checkBoxContainer->setLayout(checkBoxLayout);

    hourCheckBox = new QCheckBox("Hours");
    secondCheckBox = new QCheckBox("Seconds");
    checkBoxLayout->addWidget(hourCheckBox);
    checkBoxLayout->addWidget(secondCheckBox);
    hoursGroupLayout->addWidget(checkBoxContainer, 0, Qt::AlignLeft | Qt::AlignHCenter);

    connect(hourCheckBox, &QCheckBox::stateChanged, this, &TaskRepeatableSchedulingUIBuilder::disableSecondCheckBox);
    connect(secondCheckBox, &QCheckBox::stateChanged, this, &TaskRepeatableSchedulingUIBuilder::disableHourCheckBox);

    QWidget* timeScheduleInformationContainer = new QWidget();
    QVBoxLayout* timeScheduleInformationContainerLayout = new QVBoxLayout();
    timeScheduleInformationContainer->setLayout(timeScheduleInformationContainerLayout);

    QWidget* startTimeContainer = new QWidget();
    startTimeContainerLayout = new QHBoxLayout();
    startTimeContainer->setLayout(startTimeContainerLayout);

    QLabel* selectStartTime = new QLabel("Select start time:");
    QTimeEdit* startTime = new QTimeEdit();
    startTimeContainerLayout->addWidget(selectStartTime, 0, Qt::AlignLeft);
    startTimeContainerLayout->addWidget(startTime, 0, Qt::AlignRight);
    timeScheduleInformationContainerLayout->addWidget(startTimeContainer);

    QWidget* repeatableContainer = new QWidget();
    repeatableContainerLayout = new QHBoxLayout();
    repeatableContainer->setLayout(repeatableContainerLayout);

    QLabel* selectRepeatable = new QLabel("How often to repeat:");
    QSpinBox* numTime = new QSpinBox();
    repeatableContainerLayout->addWidget(selectRepeatable, 0, Qt::AlignLeft);
    repeatableContainerLayout->addWidget(numTime, 0, Qt::AlignRight);
    timeScheduleInformationContainerLayout->addWidget(repeatableContainer);
    hoursGroupLayout->addWidget(timeScheduleInformationContainer, 0, Qt::AlignRight | Qt::AlignHCenter);
}

void TaskRepeatableSchedulingUIBuilder::buttonStep() {
    QWidget* buttonContainer = new QWidget();
    QHBoxLayout* buttonContainerLayout = new QHBoxLayout();
    buttonContainer->setLayout(buttonContainerLayout);

    QPushButton* backButton = new QPushButton("< Back");
    buttonContainerLayout->addWidget(backButton, 0, Qt::AlignLeft);
    connect(backButton, &QPushButton::released, this, &TaskRepeatableSchedulingUIBuilder::emitBackSchedulingSelectionSignal);

    QPushButton* finishButton = new QPushButton("Finish");
    buttonContainerLayout->addWidget(finishButton, 0, Qt::AlignRight);
    taskRepeatableSchedulingLayout->addWidget(buttonContainer);

    connect(finishButton, &QPushButton::released, this, &TaskRepeatableSchedulingUIBuilder::emitClosePageSignal);
}

QWidget* TaskRepeatableSchedulingUIBuilder::getResult() {
    return taskRepeatableSchedulingWidget;
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
        startDateEdit->setReadOnly(false);
        startDateEdit->setDate(date);
        toChangeStartDate = false;
        startDateEdit->setReadOnly(true);
    } else if (!toChangeStartDate && date > startDateEdit->date()) {
        endDateEdit->setDate(date);
        toChangeStartDate = true;
    }
}

void TaskRepeatableSchedulingUIBuilder::emitBackSchedulingSelectionSignal() {
    emit backPageSignal();
}

void TaskRepeatableSchedulingUIBuilder::emitClosePageSignal() {
    emit closePageSignal(dateContainerLayout, daysLayout, checkBoxLayout, startTimeContainerLayout, repeatableContainerLayout);
}
