/*
 * Clock.cpp
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */
#include"Clock.h"
namespace prSunrise{

#include "Clock.h"
Clock::Clock(int hour,int minute,int seconds):seconds(seconds),minute(minute),hour(hour){}

bool Clock::operator==(const Clock& c){
		return c.hour==hour && c.minute==minute && c.seconds==seconds;
}
Clock Clock::operator+(const Clock& c){
	int auxSec=seconds+c.seconds;
	int auxMin=0,auxHour=0;
	if(auxSec>=MAX_SEC){
		auxSec%=MAX_SEC;
		auxMin++;
	}
	auxMin+=minute+c.minute;
	if(auxMin>=MAX_MIN){
		auxMin%=MAX_MIN;
		auxHour++;
	}
	auxHour+=hour+c.hour;
	auxHour%=MAX_HOUR;
	return Clock(auxHour,auxMin,auxSec);
}
void Clock::increaseTime(){
	*this=operator+(1);
}
void Clock::setTime(int8_t hour,int8_t minute,int8_t second){
	this->hour=hour;
	this->minute=minute;
	this->seconds=second;
}

std::string Clock::toString(){
	return std::to_string(hour)+":"+std::to_string(minute)+":"+std::to_string(seconds);
}

Clock Clock::operator++(){
	*this=operator+(1);
	return *this;
}

Clock Clock::operator+(int i){
	if(i<0)
		return operator-(-i);
	Clock res(*this);
	std::cout<<res<<std::endl;
	res.seconds=res.seconds+i;
	std::cout<<res<<std::endl;
	if(res.seconds>=MAX_SEC){
		res.minute+=res.seconds/MAX_SEC;
		res.seconds%=MAX_SEC;
	}
	std::cout<<res<<std::endl;
	if(res.minute>=MAX_MIN){
		res.hour+=res.minute/MAX_MIN;
		res.hour%=MAX_HOUR;
		res.minute%=MAX_MIN;
	}
	//std::cout<<res<<std::endl;
	return res;
}
Clock Clock::operator-(int i){
	if(i<0)
		return operator+(-i);
	Clock res (*this);
	res.seconds=res.seconds-i;
	if(res.seconds<0){
		res.minute+=res.seconds/MAX_SEC;
		res.seconds=-res.seconds%MAX_SEC;
	}
	if(res.minute<0){
		res.hour+=res.minute/MAX_MIN;
		res.minute=-res.minute%MAX_MIN;
	}
	if(res.hour<0)
		res.hour=-res.hour%MAX_HOUR;
	return res;
}

}



