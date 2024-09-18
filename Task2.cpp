#include <Task.h>
#include <QFile>
#include <QDebug>
class Task2 : public Task {
public:
    Task2(){}

    void execute() override{
        QString filePath = parameters["filePath"].toString();
        if (!QFile::exists(filePath)) {
            qDebug().noquote() << "File not found:" << filePath;
        }
    }


};
