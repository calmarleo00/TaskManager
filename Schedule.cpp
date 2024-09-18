#include <Schedule.h>

int Schedule::getDay(){
    return this->day;
}

void Schedule::setDay(int day){
    this->day = day;
}

bool Schedule::getIsRepeatable(){
    return this->isRepeatable;
}
void Schedule::setIsRepeatable(bool isRepeatable){
    this->isRepeatable = isRepeatable;
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
