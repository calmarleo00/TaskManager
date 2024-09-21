#include <TaskUpdateWindowUIDecorator.h>
#include <MainWindowUI.h>
#include <AppController.h>
#include <Scheduler.h>

void TaskUpdateWindowUIDecorator::setupTaskWidgetsUI(){
    component->setupTaskWidgetsUI();
    firstWindow = component->getFirstWindow();
    QPushButton* nextFirstWindowButton = firstWindow->getNextPageButton();
    nextFirstWindowButton->disconnect(nextFirstWindowButton, &QPushButton::released, firstWindow, &TaskAttributeUI::nextPageCreate);
    nextFirstWindowButton->connect(nextFirstWindowButton, &QPushButton::released, firstWindow, &TaskAttributeUI::nextPageUpdate);

    QGridLayout* attributesGrid = firstWindow->getAttributesGrid();
    // Aggiunta del nome del task inserito precedentemente, reso read-only
    qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->setPlainText(task->getTaskName());
    qobject_cast<QTextEdit*>(attributesGrid->itemAt(2)->widget())->setReadOnly(true);
    // Aggiunta della descrizione
    qobject_cast<QTextEdit*>(attributesGrid->itemAt(5)->widget())->setPlainText(task->getTaskDescription());
    // Se il task è esterno, nella finestra bisogna aggiungere una sola riga aggiuntiva (i.e. quella relativa al command)
    if(task->getIsExternal()){
        firstWindow->setIsExternal(true);
        qobject_cast<QTextEdit*>(attributesGrid->itemAt(8)->widget())->setPlainText(task->getTaskCommand());
    }
    // Se il task è interno, invece, nel layout devono essere aggiunti tutti gli attributi inseriti in precedenza dall'utente (e.g. nome, type, value)
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

    // Imposta la pagina di selezione del tipo di schedulazione con il flag attivo in base a ciò che era stato inserito in precedenza
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
    qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(0)->widget())->setEnabled(false);
    qobject_cast<QCheckBox*>(bottomContainerLayout->itemAt(1)->widget())->setEnabled(false);
    // Il bottone "Next >" deve essere ora collegato al metodo implementato all'interno del decorator
    // Questo per permettere al Decorator di interagire con il Director per costruire la pagina e successivamente inserirvi i dati corretti
    QPushButton* nextButton = qobject_cast<QPushButton*>(bottomContainerLayout->itemAt(3)->widget());
    disconnect(nextButton, &QPushButton::released, component, &TaskWindowUI::nextSchedulePage);
    nextButton->connect(nextButton, &QPushButton::released, this, &TaskUpdateWindowUIDecorator::nextSchedulePage);

}


void TaskUpdateWindowUIDecorator::backAttributeSelectionPage(){
    component->getStackedWindows()->setCurrentWidget(firstWindow->getTaskAttributeWindowWidget());
    component->getStackedWindows()->setMaximumSize(firstWindow->getTaskAttributeWindowWidget()->minimumWidth(), firstWindow->getTaskAttributeWindowWidget()->minimumHeight());
    component->getStackedWindows()->setMaximumSize(firstWindow->getTaskAttributeWindowWidget()->maximumWidth(), firstWindow->getTaskAttributeWindowWidget()->maximumHeight());
    component->getStackedWindows()->show();
}

void TaskUpdateWindowUIDecorator::backSchedulingSelectionPage(){
    component->getStackedWindows()->setFixedSize(schedulingSelectionWidget->maximumWidth(), schedulingSelectionWidget->maximumHeight());
    component->getStackedWindows()->setCurrentWidget(schedulingSelectionWidget);
    component->getStackedWindows()->show();
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
        TaskFixedSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskFixedSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskFixedSchedulingUIBuilder::backPageSignal, this, &TaskUpdateWindowUIDecorator::backSchedulingSelectionPage);
        connect(taskSchedulingWindowBuilder, &TaskFixedSchedulingUIBuilder::closePageFixedSignal, this, &TaskUpdateWindowUIDecorator::closeFixedWindow);
        secondWindowDirector = new TaskCreationSchedulingUIDirector(taskSchedulingWindowBuilder);
        secondWindowDirector->buildWindow();

        taskSchedulingWindowBuilder->getStartDateEdit()->setDate(task->getStartDate());
        taskSchedulingWindowBuilder->getEndDateEdit()->setDate(task->getEndDate());

        QCheckBox* daysCheckBoxes = taskSchedulingWindowBuilder->getDaysCheckBoxes();
        QVBoxLayout* timeContainerLayout = taskSchedulingWindowBuilder->getTimeContainerLayout();
        for(int i = 0; i < 7; i++){
            if(task->getScheduling()[i]->getDay() != -1){
                qDebug() << i;
                daysCheckBoxes[i].setChecked(true);
                Schedule::Time* iterTime = task->getScheduling()[i]->getStartTime();
                qobject_cast<QTimeEdit*>(timeContainerLayout[i].itemAt(0)->widget())->setTime(iterTime->time);
                iterTime = iterTime->nextTime;
                while(iterTime){
                    QTimeEdit* timeEdit = new QTimeEdit(taskSchedulingWindowBuilder->getResult());
                    timeEdit->setTime(iterTime->time);
                    qDebug() << timeEdit->time();
                    timeContainerLayout[i].addWidget(timeEdit);
                    timeEdit->update();
                    iterTime = iterTime->nextTime;
                }
            }
            else{
                daysCheckBoxes[i].setChecked(false);
            }
        }
        component->getStackedWindows()->addWidget(taskSchedulingWindowBuilder->getResult());
        component->getStackedWindows()->setCurrentWidget(taskSchedulingWindowBuilder->getResult());
        component->getStackedWindows()->setMinimumSize(taskSchedulingWindowBuilder->getResult()->minimumWidth(), taskSchedulingWindowBuilder->getResult()->minimumHeight());
        component->getStackedWindows()->setMaximumSize(taskSchedulingWindowBuilder->getResult()->maximumWidth(), taskSchedulingWindowBuilder->getResult()->maximumHeight());
        component->getStackedWindows()->show();

    }
    else if(task->getIsRepeatable()){
        TaskRepeatableSchedulingUIBuilder* taskSchedulingWindowBuilder = new TaskRepeatableSchedulingUIBuilder();
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::backPageSignal, this, &TaskUpdateWindowUIDecorator::backSchedulingSelectionPage);
        connect(taskSchedulingWindowBuilder, &TaskRepeatableSchedulingUIBuilder::closePageRepeatableSignal, this, &TaskUpdateWindowUIDecorator::closeRepeatableWindow);
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
        component->getStackedWindows()->setMinimumSize(taskSchedulingWindowBuilder->getResult()->minimumWidth(), taskSchedulingWindowBuilder->getResult()->minimumHeight());
        component->getStackedWindows()->setMaximumSize(taskSchedulingWindowBuilder->getResult()->maximumWidth(), taskSchedulingWindowBuilder->getResult()->maximumHeight());
        component->getStackedWindows()->show();
    }

}

// Se il task era ripetibile, una volta chiusa la finestra viene richiesto all'AppController di aggiornare i valori del task all'interno dello heap e del database. Infine invia un segnale alla MainWinowUI affinchè aggiorni la grafica di conseguenza
void TaskUpdateWindowUIDecorator::closeRepeatableWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount){
    AppController::getInstance()->setTaskAttributes(task, firstWindow->getAttributesGrid(), firstWindow->getIsExternal());
    AppController::getInstance()->setTaskRepeatableScheduleValues(hourCheckBox, secondCheckBox, startDateEdit, endDateEdit, startTime, repeatableAmount);
    AppController::getInstance()->updateTaskInDatabase(task);
    Scheduler::getInstance()->removeTaskFromQueue(task->getTaskName());
    AppController::getInstance()->callAddTaskToSchedule();
    component->getStackedWindows()->close();
    emit signalUpdateTaskToMainWindowUI(task);
}

void TaskUpdateWindowUIDecorator::closeFixedWindow(QCheckBox* daysCheckBoxes, QVBoxLayout* timeContainerLayout, QDateEdit* startDateEdit, QDateEdit* endDateEdit){
    AppController::getInstance()->setTaskAttributes(task, firstWindow->getAttributesGrid(), firstWindow->getIsExternal());
    AppController::getInstance()->setTaskFixedScheduleValues(daysCheckBoxes, timeContainerLayout, startDateEdit, endDateEdit);
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




