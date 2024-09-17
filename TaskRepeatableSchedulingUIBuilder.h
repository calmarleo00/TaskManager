#ifndef TASKREPEATABLESCHEDULINGUIBUILDER_H
#define TASKREPEATABLESCHEDULINGUIBUILDER_H

#include "TaskCreationSchedulingUIBuilder.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>

class TaskRepeatableSchedulingUIBuilder : public TaskCreationSchedulingUIBuilder {
    Q_OBJECT

protected:
    QWidget* taskRepeatableSchedulingWidget;
    QVBoxLayout* taskRepeatableSchedulingLayout;
    QVBoxLayout* scheduleGroupLayout;
    QCheckBox* daysCheckBoxes;
    QVBoxLayout* checkBoxLayout;
    QHBoxLayout* hoursGroupLayout;
    QHBoxLayout* daysLayout;
    QHBoxLayout* dateContainerLayout;

public:
    TaskRepeatableSchedulingUIBuilder();
    virtual ~TaskRepeatableSchedulingUIBuilder();
    void reset() override;
    void initializeWindowStep() override;
    void buildCalendarStep() override;
    void buildDaysStep() override;
    void buildTimeStep() override;
    void buttonStep() override;

    QWidget* getResult();

private slots:
    void disableSecondCheckBox(int value);
    void disableHourCheckBox(int value);
    void setDate(QDate date);
    void emitBackSchedulingSelectionSignal();
    void emitClosePageSignal();
};

#endif // TASKREPEATABLESCHEDULINGUIBUILDER_H
