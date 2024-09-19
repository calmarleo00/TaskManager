#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "qdatetime.h"
class Schedule{
    public:
        struct Time{
            QTime time;
            Time* nextTime;
            Time(QTime time){
                this->time = time;
                nextTime = nullptr;
            }
        };
    private:
        int idDatabase;
        int day;
        int repeatableHours;
        int repeatableSeconds;
        Time* startTime;
        Time* iterTime;

    public:
        Schedule(){
            day = -1;
            repeatableHours = -1;
            repeatableSeconds = -1;
        }

        int getDay();
        void setDay(int day);
        int getRepeatableHours();
        void setRepeatableHours(int repeatableHours);
        int getRepeatableSeconds();
        void setRepeatableSeconds(int repeatableSeconds);
        Time* getStartTime();
        void setStartTime(Time* startTime);
        Time* getIterTime();
        void setIterTime(Time* iterTime);
        int getIdDatabase();
        void setIdDatabase(int id);

};

#endif // SCHEDULE_H
