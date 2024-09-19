#include <Schedule.h>

int Schedule::getDay(){
    return this->day;
}

void Schedule::setDay(int day){
    this->day = day;
}

int Schedule::getRepeatableHours(){
    return this->repeatableHours;
}
void Schedule::setRepeatableHours(int repeatableHours){
    this->repeatableHours = repeatableHours;
}
int Schedule::getRepeatableSeconds(){
    return this->repeatableSeconds;
}
void Schedule::setRepeatableSeconds(int repeatableSeconds){
    this->repeatableSeconds = repeatableSeconds;
}

Schedule::Time* Schedule::getStartTime(){
    return this->startTime;
}
void Schedule::setStartTime(Time* startTime){
    this->startTime = startTime;
}
Schedule::Time* Schedule::getIterTime(){
    return this->iterTime;
}
void Schedule::setIterTime(Time* iterTime){
    this->iterTime = iterTime;
}

int Schedule::getIdDatabase(){
    return this->idDatabase;
}

void Schedule::setIdDatabase(int id){
    this->idDatabase = id;
}
