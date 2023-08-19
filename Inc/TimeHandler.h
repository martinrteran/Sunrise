#include <time.h>

// an hour is 60 minutes, a day is 1440. This values can be
//typedef uint8_t time_t;
//typedef uint16_t clock_t;

time_t GetMinutes(clock_t clock);
time_t GetHour(clock_t clock);
void SetMinutes(clock_t* clock, time_t minute);
void SetHour(clock_t* clock,time_t hour);
void AddMinute(clock_t* clock);

uint8_t Equals(clock_t lhs, clock_t rhs);
time_t DifferenceMinutes(clock_t lhs, clock_t rhs);

clock_t Clock(time_t hour, time_t minute);
void ChangeClockDigit(clock_t* clock,uint16_t digit,int8_t increase);

uint8_t IsValid(clock_t clock);


