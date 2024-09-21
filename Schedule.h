#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "qdatetime.h"
/* La classe Schedule rappresenta la pianificazione di una task (attività)
 * che può essere eseguita in un giorno specifico e a determinati orari.
 * La classe contiene anche informazioni sulla ripetibilità della task in termini di ore e secondi. */
class Schedule{
    public:
        // Struttura che per ogni giorno tiene conto di una lista di orari in cui il task dev'essere eseguito.
        // I valori della lista sono impostati a seconda di ciò che l'utente inserisce
        struct Time{
            // Rappresenta un singolo orario in cui la task può essere eseguita.
            QTime time;
            // Puntatore al prossimo orario, utile per avere una lista di orari.
            Time* nextTime;
            Time(QTime time){
                this->time = time;
                nextTime = nullptr;
            }
        };
    private:
        // Identificatore univoco nel database per la pianificazione.
        int idDatabase;
        // Giorno della settimana (0-6, dove 0 rappresenta lunedì e 6 domenica).
        int day;
        // Se ripetibile quante ore sono state inserite per la ripetizione
        int repeatableHours;
        // Se ripetibile l'utente può selezionare invece se il task deve essere ripetuto ogni tot secondi.
        int repeatableSeconds;
        // Puntatore alla struttura Time che rappresenta il primo orario di esecuzione della task.
        Time* startTime;
        // Puntatore che può essere usato per iterare su una lista di orari collegati.
        // Sarà il puntatore che viene aggiornato e monitorato per capire quando il task dev'essere eseguito.
        // Quando raggiunge il limite viene reimpostato all'inizio della lista, in modo tale che quando ritorna al giorno
        // corrispondente, può ripartire dall'inizio
        Time* iterTime;

    public:
        Schedule(){
            day = -1;
            repeatableHours = -1;
            repeatableSeconds = -1;
        }
        // Restituisce il giorno della settimana in cui la task è programmata.
        int getDay();
        // Imposta il giorno della settimana.
        void setDay(int day);
        // Restituisce il numero di ore tra ripetizioni della task.
        int getRepeatableHours();
        // Imposta il numero di ore tra ripetizioni
        void setRepeatableHours(int repeatableHours);
        // Restituisce il numero di secondi tra ripetizioni.
        int getRepeatableSeconds();
        // Imposta il numero di secondi tra ripetizioni.
        void setRepeatableSeconds(int repeatableSeconds);
        // Restituisce il puntatore al primo orario in cui la task è eseguita.
        Time* getStartTime();
        // Imposta l'orario di inizio della task del giorno
        void setStartTime(Time* startTime);
        // Restituisce il puntatore all'iteratore corrente sugli orari.
        Time* getIterTime();
        // Imposta l'iteratore corrente sugli orari.
        void setIterTime(Time* iterTime);
        // Restituisce l'ID della pianificazione nel database.
        int getIdDatabase();
        // Imposta l'ID della pianificazione nel database.
        void setIdDatabase(int id);

};

#endif // SCHEDULE_H
