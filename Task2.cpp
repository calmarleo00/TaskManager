#include <Task.h>
#include <QFile>
#include <QDebug>
class Task2 : public Task {
    Q_OBJECT
    QString filePath;
public:
    Task2(QString filePath){
        this->filePath = filePath;
    }

private slots:
    void execute() {
        if (!QFile::exists(filePath)) {
            qDebug().noquote() << "File not found:" << filePath;
        }
    }


};
