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

#ifndef TASKCREATIONWINDOWUI_H
#define TASKCREATIONWINDOWUI_H
class TaskCreationWindowUI : public QWidget{
    Q_OBJECT
    TaskCreationAttributeUI* firstWindow;
    QWidget* schedulingSelectionWidget;
    TaskCreationSchedulingUIDirector* secondWindowDirector;
    QStackedWidget* stackedWindows = nullptr;
    QCheckBox* repeatableSelection;
    QCheckBox* fixedSelection;
    QString taskName;
    QString taskDescription;

    public:
        TaskCreationWindowUI(){
            stackedWindows = new QStackedWidget();
        }
    public:
        void setupTaskCreationWidgetsUI();

    private slots:
        void backAttributeSelectionPage();
        void backSchedulingSelectionPage();
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal);
        void nextSchedulePage();
        void closeWindow(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QHBoxLayout* hoursGroupLayout, QVBoxLayout* checkBoxLayout);

    signals:
        void signalAddNewTaskToMainWindowUI(QString taskName, QString taskDescription);
};

#endif // TASKCREATIONWINDOWUI_H
