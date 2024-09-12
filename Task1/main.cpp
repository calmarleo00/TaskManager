#include <QApplication>
#include <QFileDialog>
#include <QFile>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    QString fileName = QFileDialog::getOpenFileName(nullptr, QFileDialog::tr("Open Text File"), "./", QFileDialog::tr("Text File (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QString text = "";
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        text = text + line + "\n";
    }
    qDebug().noquote() << text;
    return app.exec();
}


