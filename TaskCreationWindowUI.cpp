#include <TaskCreationWindowUI.h>
#include <MainWindowUI.h>
#include <AppController.h>
#include <QSqlRecord>
#include <QSqlError>


void TaskCreationWindowUI::setupTaskWidgetsUI(){
    firstWindow = new TaskAttributeUI();
    firstWindow->setupTaskAttributeCreationUI();
    stackedWindows->addWidget(firstWindow->getTaskAttributeWindowWidget());
    stackedWindows->setMaximumSize(firstWindow->getTaskAttributeWindowWidget()->maximumWidth(), firstWindow->getTaskAttributeWindowWidget()->maximumHeight());
    connect(firstWindow, &TaskAttributeUI::nextPage, this, &TaskCreationWindowUI::nextSchedulingSelectionPage);

    schedulingSelectionWidget = new QWidget();
    schedulingSelectionWidget->setMaximumSize(300, 300);
    QVBoxLayout* schedulingSelectionLayout = new QVBoxLayout();
    schedulingSelectionLayout->setAlignment(Qt::AlignTop);
    schedulingSelectionWidget->setLayout(schedulingSelectionLayout);

    QLabel* message = new QLabel("Select the type of scheduling that you want:");
    schedulingSelectionLayout->addWidget(message);

    QWidget* bottomContainerWidget = new QWidget();
    QGridLayout* bottomContainerLayout = new QGridLayout();
    bottomContainerWidget->setLayout(bottomContainerLayout);
    schedulingSelectionLayout->addWidget(bottomContainerWidget);

    repeatableSelection = new QCheckBox("Repeatable");
    bottomContainerLayout->addWidget(repeatableSelection, 0, 1, Qt::AlignCenter);
    fixedSelection = new QCheckBox("Fixed");
    bottomContainerLayout->addWidget(fixedSelection, 0, 2, Qt::AlignCenter);

    QPushButton* previousPage = new QPushButton("< Back");
    previousPage->connect(previousPage, &QPushButton::released, this, &TaskCreationWindowUI::backAttributeSelectionPage);

    bottomContainerLayout->addWidget(previousPage, 1, 0, Qt::AlignLeft);

    QPushButton* nextPage = new QPushButton("Next >");
    bottomContainerLayout->addWidget(nextPage, 1, 3, Qt::AlignRight);
    nextPage->connect(nextPage, &QPushButton::released, this, &TaskWindowUI::nextSchedulePage);
    stackedWindows->addWidget(schedulingSelectionWidget);
    stackedWindows->show();
}

void TaskCreationWindowUI::backAttributeSelectionPage(){
    stackedWindows->setCurrentWidget(firstWindow->getTaskAttributeWindowWidget());
    stackedWindows->setMaximumSize(firstWindow->getTaskAttributeWindowWidget()->maximumWidth(), firstWindow->getTaskAttributeWindowWidget()->maximumHeight());
    stackedWindows->show();
}

void TaskCreationWindowUI::backSchedulingSelectionPage(){
    stackedWindows->setCurrentWidget(schedulingSelectionWidget);
    stackedWindows->setMaximumSize(schedulingSelectionWidget->maximumWidth(), schedulingSelectionWidget->maximumHeight());
    stackedWindows->show();
}

void TaskCreationWindowUI::nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal){
    taskName = qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->toPlainText();
    taskDescription = qobject_cast<QTextEdit*>(attributesGrid->itemAt(5)->widget())->toPlainText();
    AppController::getInstance()->createNewTask(attributesGrid, isExternal);
    stackedWindows->setMaximumSize(schedulingSelectionWidget->maximumWidth(), schedulingSelectionWidget->maximumHeight());
    stackedWindows->setCurrentWidget(schedulingSelectionWidget);
}

void TaskCreationWindowUI::TaskCreationWindowUI::nextSchedulePage(){
    if(fixedSelection->isChecked()){
        /*TaskFixedSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskFixedSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskFixedSchedulingUIBuilder::backPageSignal, this, &TaskCreationWindowUI::backSchedulingSelectionPage);
        secondWindowDirector = new TaskCreationSchedulingUIDirector(taskSchedulingWindowBuilder);
        secondWindowDirector->buildWindow();
        stackedWindows->setCurrentWidget(taskSchedulingWindowBuilder->getResult());*/
    }
    else if(repeatableSelection->isChecked()){
        TaskRepeatableSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskRepeatableSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::backPageSignal, this, &TaskCreationWindowUI::backSchedulingSelectionPage);
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::closePageSignal, this, &TaskCreationWindowUI::closeWindow);
        secondWindowDirector = new TaskCreationSchedulingUIDirector(taskSchedulingWindowBuilder);
        secondWindowDirector->buildWindow();
        stackedWindows->addWidget(taskSchedulingWindowBuilder->getResult());
        stackedWindows->setCurrentWidget(taskSchedulingWindowBuilder->getResult());
        stackedWindows->setMaximumSize(taskSchedulingWindowBuilder->getResult()->maximumWidth(), taskSchedulingWindowBuilder->getResult()->maximumHeight());
        stackedWindows->show();
    }

}

void TaskCreationWindowUI::closeWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount){
    AppController::getInstance()->setTaskRepeatableScheduleValues(hourCheckBox, secondCheckBox, startDateEdit, endDateEdit, startTime, repeatableAmount);
    AppController::getInstance()->saveTaskToDatabase();
    AppController::getInstance()->callAddTaskToSchedule();
    stackedWindows->close();
    emit signalAddNewTaskToUI(taskName, taskDescription);
}

TaskAttributeUI* TaskCreationWindowUI::getFirstWindow(){
    return this->firstWindow;
}

QWidget* TaskCreationWindowUI::getSchedulingSelectionWidget(){
    return this->schedulingSelectionWidget;
}

TaskCreationSchedulingUIDirector* TaskCreationWindowUI::getSecondWindowDirector(){
    return this->secondWindowDirector;
}


QStackedWidget* TaskCreationWindowUI::getStackedWindows(){
    return this->stackedWindows;
}

