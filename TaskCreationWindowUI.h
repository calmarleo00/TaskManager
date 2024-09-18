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
#include <TaskWindowUI.h>
#include <AppController.h>

#ifndef TASKCREATIONWINDOWUI_H
#define TASKCREATIONWINDOWUI_H
class TaskCreationWindowUI : public TaskWindowUI{
    Q_OBJECT
    public:
        TaskCreationWindowUI(){
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

#endif // TASKCREATIONWINDOWUI_H
