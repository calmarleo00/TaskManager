#include <QApplication>
#include <MainWindow.h>
#include <MainWindowUI.h>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    MainWindowUI* mainWindowUI = MainWindowUI::getInstance();
    MainWindow* mainWindow = MainWindow::getInstance();
    mainWindowUI->setupMainWindowUI(mainWindow);
    // Make appear the window
    return app.exec();
}


