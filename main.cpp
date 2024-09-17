#include <QApplication>
#include <AppController.h>
#include <MainWindowUI.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

void databaseInitialization(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "ticare_connection");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("ticare");
    db.setUserName("root");

    if(db.open()){
        QSqlQuery query(db);
        query.exec("CREATE TABLE IF NOT EXISTS Task "
                    "(id VARCHAR(255) PRIMARY KEY, "
                    "description TEXT, "
                    "isExternal BOOLEAN NOT NULL DEFAULT FALSE, "
                    "command VARCHAR(255), "
                    "parameters JSON, "
                    "startDate DATE, "
                    "endDate DATE);");
        query.exec("CREATE TABLE IF NOT EXISTS Schedule "
                    "(id INT AUTO_INCREMENT PRIMARY KEY, "
                    "taskId VARCHAR(255), "
                    "day ENUM('0', '1', '2', '3', '4', '5', '6') NOT NULL, "
                    "isRepeatable BOOLEAN DEFAULT FALSE, "
                    "repeatableHours INT DEFAULT -1, "
                    "repeatableSeconds INT DEFAULT -1, "
                   "FOREIGN KEY (taskId) REFERENCES Task(id) ON DELETE CASCADE);");
        query.exec("CREATE TABLE IF NOT EXISTS Time "
                   "(id INT AUTO_INCREMENT PRIMARY KEY, "
                   "scheduleId INT, "
                   "time TIME, "
                   "FOREIGN KEY (scheduleId) REFERENCES Schedule(id) ON DELETE CASCADE);");
    }
}
int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    // Initialize database connection
    databaseInitialization();
    // Setup the main window UI
    MainWindowUI* mainWindowUI = MainWindowUI::getInstance();
    mainWindowUI->setupMainWindowUI();
    return app.exec();
}


