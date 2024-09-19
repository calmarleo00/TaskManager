#include "qwidget.h"
#include "qcheckbox.h"
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
#include <TaskAttributeUI.h>
#include <TaskCreationSchedulingUIBuilder.h>
#include <TaskFixedSchedulingUIBuilder.h>
#include <TaskCreationSchedulingUIDirector.h>
#include <TaskRepeatableSchedulingUIBuilder.h>
#include <AppController.h>

#ifndef TASKWINDOWUI_H
#define TASKWINDOWUI_H
class TaskWindowUI : public QWidget{
    Q_OBJECT
    protected:
        TaskAttributeUI* firstWindow;
        QWidget* schedulingSelectionWidget;
        TaskCreationSchedulingUIDirector* secondWindowDirector;
        QStackedWidget* stackedWindows = nullptr;
        QCheckBox* repeatableSelection;
        QCheckBox* fixedSelection;
        QString taskName;
        QString taskDescription;

    public:
        virtual void setupTaskWidgetsUI() = 0;
        virtual TaskAttributeUI* getFirstWindow() = 0;
        virtual QWidget* getSchedulingSelectionWidget() = 0;
        virtual TaskCreationSchedulingUIDirector* getSecondWindowDirector() = 0;
        virtual void nextSchedulePage() = 0;
        virtual QStackedWidget* getStackedWindows() = 0;

    private slots:
        virtual void backAttributeSelectionPage() = 0;
        virtual void backSchedulingSelectionPage() = 0;
        virtual void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) = 0;
        virtual void closeWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) = 0;

    signals:
        void signalAddNewTaskToUI(QString taskName, QString taskDescription);
        void signalUpdateTaskToMainWindowUI(Task* task);

    };

#endif // TASKWINDOWUI_H
