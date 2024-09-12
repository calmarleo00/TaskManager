#include<MainWindowUI.h>
#include<MainWindow.h>

MainWindow* MainWindow::instance = nullptr;

void MainWindow::addNewTask(){
    Task* newTask = new Task(QString::fromStdString("Task " + std::to_string(*colPointer)));
    if(MainWindow::taskHead){
        taskTail->setTaskNext(newTask);
        taskTail = newTask;
    }
    else{
        taskHead = newTask;
        taskTail = newTask;
        taskIter = newTask;
    }
    MainWindowUI* ui = MainWindowUI::getInstance();
    ui->addNewTaskUI(newTask, colPointer);
    *colPointer += 1;
}

MainWindow* MainWindow::getInstance() {
    if(instance == nullptr) {
        instance = new MainWindow();
    }
    return instance;
}
// Connect the buttons to the corresponding function when released
