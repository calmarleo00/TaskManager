#include <TrayIconMenu.h>

TrayIconMenu* TrayIconMenu::instance = nullptr;


TrayIconMenu* TrayIconMenu::getInstance(){
    if(!instance){
        instance = new TrayIconMenu();
    }
    return instance;
}
