/*
 * Alarm.cpp
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */
#include "Alarm.h"
namespace prSunrise{

Alarm::Alarm(int hour,int minute,int second):Clock(hour,minute,second),_refClock(0),_isActive(false),_isRinging(false){}

Alarm::Alarm(Clock* & rel):Clock(0,0,0),_refClock(rel),_isActive(false),_isRinging(false){}

Alarm::~Alarm(){
    _refClock=0;
}

bool Alarm::isTime()const {
    if(_refClock==0)
        _isRinging=false;
    else
        _isRinging=((*_refClock)==(*this));
    return _isRinging;
}
void Alarm::turnOff(){
    _isRinging=false;
}
void Alarm::activate(bool des){
    _isActive=des;
}
void Alarm::modifyAlarm(Alarm&& alarm){
    hour=alarm.hour;
    minute=alarm.minute;
    seconds=alarm.seconds;
}
void Alarm::modifyAlarm(int h,int m,int s){
    hour=h;
    minute=m;
    seconds=s;
}
void Alarm::changeClock(Clock* clk){
    _refClock=clk;
}

}
