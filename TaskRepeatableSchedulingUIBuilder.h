#ifndef TASKREPEATABLESCHEDULINGUIBUILDER_H
#define TASKREPEATABLESCHEDULINGUIBUILDER_H

#include "TaskCreationSchedulingUIBuilder.h"
#include "qspinbox.h"
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
        QHBoxLayout* daysLayout;
        QTimeEdit* startTime;
        QSpinBox* numTime;

    public:
        TaskRepeatableSchedulingUIBuilder();
        virtual ~TaskRepeatableSchedulingUIBuilder();
        void reset() override;
        void initializeWindowStep() override;
        void buildCalendarStep() override;
        void buildDaysStep() override {};
        void buildTimeStep() override;
        void buttonStep() override;
        QCheckBox* getHourCheckBox() override;
        QCheckBox* getSecondCheckBox() override;
        QDateEdit* getStartDateEdit() override;
        QDateEdit* getEndDateEdit() override;
        QTimeEdit* getStartTime();
        QSpinBox* getRepeatableAmount();
        QWidget* getResult();

    private slots:
        void disableSecondCheckBox(int value);
        void disableHourCheckBox(int value);
        void setDate(QDate date);
        void emitBackSchedulingSelectionSignal();
        void emitClosePageSignal();

    signals:
        void closePageSignal(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount);

    };



#endif // TASKREPEATABLESCHEDULINGUIBUILDER_H
