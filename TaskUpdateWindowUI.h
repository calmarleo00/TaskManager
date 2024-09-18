#include <TaskWindowUI.h>
#ifndef TASKUPDATEWINDOWUI_H
#define TASKUPDATEWINDOWUI_H
class TaskUpdateWindowUI : public TaskWindowUI{
    Q_OBJECT
    public:
        TaskUpdateWindowUI(){
            stackedWindows = new QStackedWidget();
        }
    public:
        void setupTaskWidgetsUI() override;

    private slots:
        void backAttributeSelectionPage() override;
        void backSchedulingSelectionPage() override;
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) override;
        void nextSchedulePage() override;
        void closeWindow(QHBoxLayout* dateContainerLayout, QHBoxLayout* daysLayout, QVBoxLayout* checkBoxLayout, QHBoxLayout* startTimeContainerLayout, QHBoxLayout* repeatableContainerLayout) override;

};
#endif // TASKUPDATEWINDOWUI_H
