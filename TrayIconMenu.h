#include "qboxlayout.h"
#include "qlabel.h"
#include "qscrollarea.h"
#include <QWidget>
#include <AppController.h>
#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H
class TrayIconMenu : public QWidget {
    Q_OBJECT
    QVBoxLayout* taskListContainerLayout;
public:
    static TrayIconMenu* instance;
private:
    TrayIconMenu(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QWidget* taskListContainer = new QWidget();
        taskListContainerLayout = new QVBoxLayout();
        taskListContainer->setLayout(taskListContainerLayout);
        // Create a scrollable area
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidget(taskListContainer);
        scrollArea->setWidgetResizable(true);

        layout->addWidget(scrollArea);
        setLayout(layout);
    }
public:
    void addTaskToTrayMenu(QString taskName){
        QWidget* taskContainer = new QWidget();
        QHBoxLayout* taskContainerLayout = new QHBoxLayout();
        taskContainer->setLayout(taskContainerLayout);

        QLabel* taskNameLabel = new QLabel(taskName);
        QPushButton* taskDetailsButton = new QPushButton("View Task's Details");

        taskContainerLayout->addWidget(taskNameLabel, 0 ,Qt::AlignLeft);
        taskContainerLayout->addWidget(taskDetailsButton, 0, Qt::AlignRight);
        taskContainer->setMaximumHeight(50);
        taskListContainerLayout->addWidget(taskContainer, 0, Qt::AlignTop);
    }

    static TrayIconMenu* getInstance();

};

#endif // TRAYICONMENU_H
