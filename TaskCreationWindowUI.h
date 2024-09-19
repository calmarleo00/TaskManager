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
        TaskAttributeUI* getFirstWindow() override;
        QWidget* getSchedulingSelectionWidget() override;
        TaskCreationSchedulingUIDirector* getSecondWindowDirector() override;
        void nextSchedulePage() override;
        QStackedWidget* getStackedWindows() override;

    public slots:
    private slots:
        void backAttributeSelectionPage() override;
        void backSchedulingSelectionPage() override;
        void nextSchedulingSelectionPage(QGridLayout* attributesGrid, bool isExternal) override;
        void closeWindow(QCheckBox* hourCheckBox, QCheckBox* secondCheckBox, QDateEdit* startDateEdit, QDateEdit* endDateEdit, QTimeEdit* startTime, QSpinBox* repeatableAmount) override;
};

#endif // TASKCREATIONWINDOWUI_H
