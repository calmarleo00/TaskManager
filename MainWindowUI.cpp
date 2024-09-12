#include<MainWindowUI.h>

MainWindowUI *MainWindowUI::instance = nullptr;

MainWindowUI *MainWindowUI::getInstance()
{
    if(MainWindowUI::instance == nullptr){
        instance = new MainWindowUI();
    }
    return MainWindowUI::instance;
}
