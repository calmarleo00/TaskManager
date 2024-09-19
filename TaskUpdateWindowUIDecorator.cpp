#include <TaskUpdateWindowUIDecorator.h>
#include <MainWindowUI.h>
#include <AppController.h>
#include <Scheduler.h>
void TaskUpdateWindowUIDecorator::setupTaskWidgetsUI(){
    component->setupTaskWidgetsUI();
    firstWindow = component->getFirstWindow();
    QGridLayout* attributesGrid = firstWindow->getAttributesGrid();
    // Add task name
    qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->setPlainText(task->getTaskName());
    // Add task description
    qobject_cast<QTextEdit*>(attributesGrid->itemAt(5)->widget())->setPlainText(task->getTaskDescription());
    if(task->getIsExternal()){
        firstWindow->setIsExternal(true);
        qobject_cast<QTextEdit*>(attributesGrid->itemAt(8)->widget())->setPlainText(task->getTaskCommand());
    }
    else{
        firstWindow->setIsExternal(false);
        for(int i = 0; i < task->getTaskParameters().count(); i++){
            firstWindow->addTaskAttribute();
            int lastRow = firstWindow->getAttributeRow() - 1;
            QString attributeName = task->getTaskParameters().keys()[i];
            QJsonObject attributeInformation = task->getTaskParameters().value(attributeName).toObject();
            QString attributeType = attributeInformation["type"].toString();
            qobject_cast<QLineEdit*>(attributesGrid->itemAt(lastRow * 3)->widget())->setText(attributeName);
            qobject_cast<QComboBox*>(attributesGrid->itemAt(lastRow * 3 + 1)->widget())->setCurrentText(attributeType);
            if(attributeType == "string"){
                qobject_cast<QTextEdit*>(attributesGrid->itemAt(lastRow * 3 + 2)->widget())->setText(attributeInformation["value"].toString());
            }
            else if(attributeType == "int"){
                qobject_cast<QSpinBox*>(attributesGrid->itemAt(lastRow * 3 + 2)->widget())->setValue(attributeInformation["value"].toInt());
            }
            else{
                qobject_cast<QDoubleSpinBox*>(attributesGrid->itemAt(lastRow * 3 + 2)->widget())->setValue(attributeInformation["value"].toDouble());
            }
        }
    }


    schedulingSelectionWidget = component->getSchedulingSelectionWidget();
    QLayout* schedulingSelectionLayout = schedulingSelectionWidget->layout();

    QWidget* bottomContainerWidget = schedulingSelectionLayout->itemAt(1)->widget();
    QLayout* bottomContainerLayout = bottomContainerWidget->layout();
    if(task->getIsRepeatable()){
        qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(0)->widget())->setChecked(true);
        qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(1)->widget())->setChecked(false);
    }
    else{
        qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(0)->widget())->setChecked(false);
        qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(1)->widget())->setChecked(true);
    }
    QPushButton* nextButton = qobject_cast<QPushButton*>(bottomContainerLayout->itemAt(3)->widget());
    disconnect(nextButton, &QPushButton::released, component, &TaskWindowUI::nextSchedulePage);
    nextButton->connect(nextButton, &QPushButton::released, this, &TaskUpdateWindowUIDecorator::nextSchedulePage);

}

void TaskUpdateWindowUIDecorator::backAttributeSelectionPage(){
    stackedWindows->setCurrentWidget(firstWindow->getTaskAttributeWindowWidget());
    stackedWindows->setMaximumSize(firstWindow->getTaskAttributeWindowWidget()->maximumWidth(), firstWindow->getTaskAttributeWindowWidget()->maximumHeight());
    stackedWindows->show();
}

void TaskUpdateWindowUIDecorator::backSchedulingSelectionPage(){
    stackedWindows->setCurrentWidget(schedulingSelectionWidget);
    stackedWindows->setMaximumSize(schedulingSelectionWidget->maximumWidth(), schedulingSelectionWidget->maximumHeight());
    stackedWindows->show();
}

void TaskUpdateWindowUIDecorator::nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal){
    taskName = qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->toPlainText();
    taskDescription = qobject_cast<QTextEdit*>(attributesGrid->itemAt(5)->widget())->toPlainText();
    AppController::getInstance()->setTaskAttributes(task, attributesGrid, isExternal);
    component->getStackedWindows()->setMaximumSize(component->getSchedulingSelectionWidget()->maximumWidth(), component->getSchedulingSelectionWidget()->maximumHeight());
    component->getStackedWindows()->setCurrentWidget(component->getSchedulingSelectionWidget());
}

void TaskUpdateWindowUIDecorator::TaskUpdateWindowUIDecorator::nextSchedulePage(){
    if(!task->getIsRepeatable()){
        /*TaskFixedSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskFixedSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskFixedSchedulingUIBuilder::backPageSignal, this, &TaskUpdateWindowUIDecorator::backSchedulingSelectionPage);
        secondWindowDirector = new TaskCreationSchedulingUIDirector(taskSchedulingWindowBuilder);
        secondWindowDirector->buildWindow();
        stackedWindows->setCurrentWidget(taskSchedulingWindowBuilder->getResult());*/
    }
    else if(task->getIsRepeatable()){
        TaskRepeatableSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskRepeatableSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::backPageSignal, this, &TaskUpdateWindowUIDecorator::backSchedulingSelectionPage);
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::closePageSignal, this, &TaskUpdateWindowUIDecorator::closeWindow);
        secondWindowDirector = new TaskCreationSchedulingUIDirector(taskSchedulingWindowBuilder);
        secondWindowDirector->buildWindow();

        int startDay = task->getStartDate().dayOfWeek() - 1;
        taskSchedulingWindowBuilder->getStartDateEdit()->setDate(task->getStartDate());
        taskSchedulingWindowBuilder->getEndDateEdit()->setDate(task->getEndDate());
        taskSchedulingWindowBuilder->getStartTime()->setTime(task->getScheduling()[startDay]->getStartTime()->time);
        if(task->getScheduling()[startDay]->getRepeatableHours() != -1){
            taskSchedulingWindowBuilder->getHourCheckBox()->setChecked(true);
            taskSchedulingWindowBuilder->getRepeatableAmount()->setValue(task->getScheduling()[startDay]->getRepeatableHours());
        }
        else{
            taskSchedulingWindowBuilder->getSecondCheckBox()->setChecked(true);
            taskSchedulingWindowBuilder->getRepeatableAmount()->setValue(task->getScheduling()[startDay]->getRepeatableSeconds());
        }
        component->getStackedWindows()->addWidget(taskSchedulingWindowBuilder->getResult());
        component->getStackedWindows()->setCurrentWidget(taskSchedulingWindowBuilder->getResult());
        component->getStackedWindows()->setMaximumSize(taskSchedulingWindowBuilder->getResult()->maximumWidth(), taskSchedulingWindowBuilder->getResult()->maximumHeight());
        component->getStackedWindows()->show();
    }

}

void TaskUpdateWindowUIDecorator::closeWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount){
    AppController::getInstance()->setTaskAttributes(task, firstWindow->getAttributesGrid(), firstWindow->getIsExternal());
    AppController::getInstance()->setTaskRepeatableScheduleValues(hourCheckBox, secondCheckBox, startDateEdit, endDateEdit, startTime, repeatableAmount);
    AppController::getInstance()->updateTaskInDatabase(task);
    Scheduler::getInstance()->removeTaskFromQueue(task->getTaskName());
    AppController::getInstance()->callAddTaskToSchedule();
    component->getStackedWindows()->close();
    emit signalUpdateTaskToMainWindowUI(task);
}


TaskAttributeUI* TaskUpdateWindowUIDecorator::getFirstWindow(){
    return this->firstWindow;
}

QWidget* TaskUpdateWindowUIDecorator::getSchedulingSelectionWidget(){
    return this->schedulingSelectionWidget;
}

TaskCreationSchedulingUIDirector* TaskUpdateWindowUIDecorator::getSecondWindowDirector(){
    return this->secondWindowDirector;
}


QStackedWidget* TaskUpdateWindowUIDecorator::getStackedWindows(){
    return this->stackedWindows;
}


