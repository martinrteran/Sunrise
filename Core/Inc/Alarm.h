/*
 * Alarm.h
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */

#ifndef INC_ALARM_H_
#define INC_ALARM_H_
#include "Clock.h"
namespace prSunrise{

class Alarm:private Clock{
    Clock* _refClock;
    bool _isActive;
    mutable bool _isRinging;
public:
    Alarm(int hour,int minute,int second=0);
    Alarm(Clock* &);
    ~Alarm();
    bool isTime()const;
    void turnOff();
    void activate(bool des=true);
    void modifyAlarm(Alarm&& alarm);
    void modifyAlarm(int h,int m,int s=0);
    void changeClock(Clock* clk);
};


}

#endif /* INC_ALARM_H_ */
