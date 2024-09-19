#include "qcheckbox.h"
#include "qdatetimeedit.h"
#include <QWidget>
#include <QPushButton>

#ifndef TASKCREATIONSCHEDULINGUIBUILDER_H
#define TASKCREATIONSCHEDULINGUIBUILDER_H
class TaskCreationSchedulingUIBuilder : public QWidget{
    Q_OBJECT
    protected:
        QWidget* taskSchedulingWindow = nullptr;
        QCheckBox* hourCheckBox = nullptr;
        QCheckBox* secondCheckBox = nullptr;
        QDateEdit* startDateEdit = nullptr;
        QDateEdit* endDateEdit = nullptr;
        bool toChangeStartDate = true;

    public:
        virtual void reset() = 0;
        virtual void initializeWindowStep() = 0;
        virtual void buildCalendarStep() = 0;
        virtual void buildDaysStep() = 0;
        virtual void buildTimeStep() = 0;
        virtual void buttonStep() = 0;
        virtual QCheckBox* getHourCheckBox() = 0;
        virtual QCheckBox* getSecondCheckBox() = 0;
        virtual QDateEdit* getStartDateEdit() = 0;
        virtual QDateEdit* getEndDateEdit() = 0;

    signals:
        void backPageSignal();
};

#endif // TASKCREATIONSCHEDULINGUIBUILDER_H
