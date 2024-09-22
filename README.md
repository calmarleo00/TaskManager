## Guida all'installazione ed esecuzione
- Clonare il repository all'interno di una cartella del proprio computer.
- Aprire Qt 6.2.4.
- Selezionare "Open Project"
- Nella finestra che si apre, selezionare il file con estensione ".pro"
- Per eseguire l'applicazione premere il pulsante run

### (Opzionale) Impostazione per l'utilizzo di MySQL
- Collegarsi al seguente link (https://github.com/thecodemonkey86/qt_mysql_driver) ed installare la release corrispondente alla versione di Qt scaricata (n.b. 6.2.4)
- Estrarre i file in una cartella del computer
- Copiare il contenuto della cartella sqldrivers, all'interno della cartella debug, all'interno della cartella con all'interno la versione di Qt (eg. QT/6.2.4/mingw_64/plugins/sqldrivers)
- I restanti 3 file all'interno della cartella debug, copiarli nella cartella debug del progetto
- Fare la rebuild del progetto

## Documentazione
All'interno della cartella "Documentazione" nel repository, sono presenti 3 file PDF. Due sono diagrammi delle classe in UML. Il file rimanente contiene una breve descrizione del progetto e della sua struttura, 
nonché come possa essere utilizzato tramite l'inserimento di due task di base supportati.
