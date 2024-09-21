#include <QApplication>
#include <AppController.h>
#include <MainWindowUI.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <TrayIconMenu.h>

/* Questa funzione si occupa di configurare e aprire una connessione a un database MySQL,
 * oltre a creare le tabelle necessarie per gestire le task e la loro pianificazione, se non esistono già. */
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
                    "isRepeatable BOOLEAN DEFAULT FALSE, "
                    "command VARCHAR(255), "
                    "parameters JSON, "
                    "startDate DATE, "
                    "endDate DATE);");
        query.exec("CREATE TABLE IF NOT EXISTS Schedule "
                    "(id INT AUTO_INCREMENT PRIMARY KEY, "
                    "taskId VARCHAR(255), "
                    "day ENUM('0', '1', '2', '3', '4', '5', '6') NOT NULL, "
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

    // Inizializza la connesione con il database
    databaseInitialization();
    // Inizializza il TrayIconMenu
    /* Questa funzione si occupa di configurare e mostrare l'icona nella tray di sistema con un menu contestuale, offrendo opzioni per visualizzare le task e chiudere l'applicazione. */
    QString projectPath = QDir::currentPath() + "/../..";
    QString filePath = QDir(projectPath).filePath("trayiconmenu.png");
    QSystemTrayIcon trayIcon((QIcon(filePath)));
    trayIcon.setToolTip("Task Scheduler");

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    // Calcola la posizione per l'angono in basso a destra
    TrayIconMenu *trayMenu = TrayIconMenu::getInstance();
    trayMenu->setFixedSize(250, 400);

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Calcola la dimensione del widget
    int widgetWidth = trayMenu->geometry().width();
    int widgetHeight = trayMenu->geometry().height();

    // Calcola x e y per l'angono in basso a destra
    int x = screenWidth - widgetWidth;
    int y = screenHeight - widgetHeight;

    // Mostra il menu nella posizione calcolata
    trayMenu->move(x, y);

    QMenu* trayMenuContext = new QMenu();

    QAction* showAction = trayMenuContext->addAction("Show Tray Menu");
    QAction* showMainWindowUI = trayMenuContext->addAction("Show Main Window");
    QAction* quitAction = trayMenuContext->addAction("Quit");

    QObject::connect(showMainWindowUI, &QAction::triggered, MainWindowUI::getInstance(), &MainWindowUI::showMainWindow);
    QObject::connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    QObject::connect(showAction, &QAction::triggered, [trayMenu] {
        trayMenu->show();
    });

    trayIcon.setContextMenu(trayMenuContext);
    trayIcon.show();
    // Imposta la finestra principale
    MainWindowUI* mainWindowUI = MainWindowUI::getInstance();
    mainWindowUI->setupMainWindowUI();


    return app.exec();
}


