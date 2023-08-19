/*
 * TimeHandler.c
 *
 *  Created on: 6 jul. 2023
 *      Author: marti
 */
#include "TimeHandler.h"
#include "time.h"
#include<assert.h>
#include <math.h>
#include "ssd1306.h"
#include "fonts.h"

#define CLOCK_OFFSET 100
#define GET_HOUR(x) (x)/CLOCK_OFFSET
#define GET_MINUTE(x) (x)- ((x)/CLOCK_OFFSET)*CLOCK_OFFSET;

#define CLOCK_HOUR_TOP 24
#define CLOCK_MINUTE_TOP 60



time_t GetMinutes(clock_t clock){
	return GET_MINUTE(clock);
}
time_t GetHour(clock_t clock){
	return GET_HOUR(clock);
}
void SetMinutes(clock_t* clock, time_t minute){
	assert(minute<59 && minute>=0);
	*clock -= GetMinutes(*clock);
	*clock += minute;
}
void SetHour(clock_t* clock,time_t hour){
	assert(hour>=0 && hour<24);
	*clock -= GetHour(*clock)*CLOCK_OFFSET;
	*clock += hour*CLOCK_OFFSET;
}

void AddMinute(clock_t* clock){
	(*clock) += 1;
	if(GetMinutes(*clock)==60){
		(*clock) -= 60;
		(*clock) += CLOCK_OFFSET;
	}
	if(GetHour(*clock)==24){
		(*clock) -= (24*CLOCK_OFFSET);
	}
}

uint8_t Equals(clock_t lhs, clock_t rhs){
	return (GetMinutes(lhs)==GetMinutes(rhs) && GetHour(lhs)==GetHour(rhs));
}


clock_t Clock(time_t hour, time_t minute){
	assert(hour>=0 && hour<24 && minute>=0 && minute<59);
	clock_t res=0;
	res += hour*CLOCK_OFFSET;
	res += minute;
	return res;
}

void ChangeClockDigit(clock_t* clock,uint16_t digit,int8_t increase){
	assert(digit<4 && increase>-2 && increase<2);
	uint16_t digitPosition = pow(10,digit);
	(*clock) += increase*digitPosition;
}
void PrintClockLED(clock_t clock, int8_t cursor, uint8_t x, uint8_t y){
	SSD1306_GotoXY(x, y);
	char chour[2];
	char cminute[2];
	char ctime[6];

	int hour = GetHour(clock);
	int minute = GetMinutes(clock);

	chour[0]='0'+(int) hour/10;
	chour[1]='0'+(hour-(hour/10)*10);

	cminute[0]='0'+ (int) minute/10;
	cminute[1]='0'+(minute-(minute/10)*10);

	ctime[0]=chour[0];
	ctime[1]=chour[1];
	ctime[2]=':';
	ctime[3]=cminute[0];
	ctime[4]=cminute[1];
	ctime[5]='\0';

	if(cursor>-1 && cursor<4){
		if(cursor>1) cursor++;

		for(int i=0;i<5;++i)
			SSD1306_Putc(ctime[i],&Font_16x26, (i==cursor) ? SSD1306_COLOR_BLACK:SSD1306_COLOR_WHITE);

	}else
		SSD1306_Puts(ctime, &Font_16x26, SSD1306_COLOR_WHITE);

	SSD1306_UpdateScreen();
}

uint8_t IsValid(clock_t clock){
    return (clock>=0 && clock <= 2359);
}
void SetDigit(clock_t* clock,int8_t cursor, uint8_t digit){
    if(digit>=10 || cursor >=4 || cursor<-1) return;
    int power = powl(10,3-cursor);
    clock_t cpy = * clock;
    clock_t top =(cpy/(power*10))*(power*10);
    uint8_t actualDigit = !top? (cpy/power):(cpy-top)/power;

    cpy -= actualDigit*power;
    cpy += digit*power;
    *clock = cpy;
}

void IncreaseOnCursor(clock_t* clock, int8_t cursor, int8_t increase){
	if(increase <-1 || increase >1) return;
	if(cursor<0 || cursor>3) return;
	if(!IsValid(*clock)) return;
	clock_t cpy = *clock;

	int8_t auxTime;

	if(cursor == 0){ // TOP HOUR
		auxTime =  (GetHour(cpy)/10 + increase)%3;
		if(auxTime<0) auxTime = 2;
		SetDigit(&cpy,cursor,auxTime);
		if(!IsValid(cpy)){
			SetDigit(&cpy,1, 3);
		}
	}else if(cursor == 1){ // LOW  HOUR
		uint8_t hourTop = GetHour(cpy)/10;
		auxTime = GetHour(cpy)-hourTop*10;

		if(increase>0){
			auxTime++;
			if(hourTop == 2)
				auxTime %= 4;
			else
				auxTime %= 10;
		}else if(increase < 0){
			auxTime--;
			if(auxTime<0){
				if(hourTop == 2)
					auxTime = 4;
				else
					auxTime = 9;

			}
		}
		SetDigit(&cpy,cursor, auxTime);

	}else if(cursor == 2){  // HIGH MINUTE
		auxTime =  (GetMinutes(cpy)/10 +increase)%6;
		if(auxTime < 0) auxTime = 5;
		SetDigit(&cpy,cursor,auxTime);
	}else{ // LOW MINUTE
		auxTime= GetMinutes(cpy)/10;
		auxTime = GetMinutes(cpy)-auxTime*10;
		auxTime = (auxTime + increase)%10;
		if(auxTime < 0) auxTime = 9;
		SetDigit(&cpy,cursor,auxTime);
	}


	*clock = cpy;
}

time_t DifferenceMinutes(clock_t lhs, clock_t rhs){
	if(lhs<rhs) return -1;
	time_t hourDiff=GetHour(lhs)-GetHour(rhs);
	time_t minDiff = GetMinutes(lhs) - GetMinutes(rhs);
	if(hourDiff<0) return -1;
	return hourDiff*60+minDiff;
}
