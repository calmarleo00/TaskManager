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
#include <TaskCreationAttributeUI.h>
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
        TaskCreationAttributeUI* firstWindow;
        QWidget* schedulingSelectionWidget;
        TaskCreationSchedulingUIDirector* secondWindowDirector;
        QStackedWidget* stackedWindows = nullptr;
        QCheckBox* repeatableSelection;
        QCheckBox* fixedSelection;
        QString taskName;
        QString taskDescription;


    virtual void setupTaskCreationWidgetsUI() = 0;

    private slots:
        virtual void backAttributeSelectionPage() = 0;
        virtual void backSchedulingSelectionPage() = 0;
        virtual void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) = 0;
        virtual void nextSchedulePage() = 0;
        virtual void closeWindow(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QVBoxLayout* checkBoxLayout, QHBoxLayout* startTimeContainerLayout, QHBoxLayout* repeatableContainerLayout) = 0;

    signals:
        void signalAddNewTaskToMainWindowUI(QString taskName, QString taskDescription);
    };

#endif // TASKWINDOWUI_H
