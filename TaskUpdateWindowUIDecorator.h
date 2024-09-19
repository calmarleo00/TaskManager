#include <TaskWindowUI.h>
#ifndef TASKUPDATEWINDOWUIDECORATOR_H
#define TASKUPDATEWINDOWUIDECORATOR_H
class TaskUpdateWindowUIDecorator : public TaskWindowUI{
    Q_OBJECT
    TaskWindowUI* component;
    Task* task;
    public:
        TaskUpdateWindowUIDecorator(TaskWindowUI* componentToWrap, Task* taskToUpdate){
            component = componentToWrap;
            stackedWindows = new QStackedWidget();
            task = taskToUpdate;
        }
    public:
        void setupTaskWidgetsUI() override;
        TaskAttributeUI* getFirstWindow() override;
        QWidget* getSchedulingSelectionWidget() override;
        TaskCreationSchedulingUIDirector* getSecondWindowDirector() override;
        QStackedWidget* getStackedWindows() override;
    private slots:
        void backAttributeSelectionPage() override;
        void backSchedulingSelectionPage() override;
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) override;
        void nextSchedulePage() override;
        void closeWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) override;

};
#endif // TASKUPDATEWINDOWUIDECORATOR_H
