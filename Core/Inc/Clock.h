#include<string_view>
#include<string>
#include <iostream>
namespace prSunrise{
class Clock{
protected:
	static const uint8_t MAX_SEC=60;
	static const uint8_t MAX_MIN=60;
	static const uint8_t MAX_HOUR=24;
	int8_t seconds;
	int8_t minute;
	int8_t hour;
public:
	Clock(int hour,int minute,int seconds=0);
	bool operator==(const Clock& c);

	virtual Clock operator+(const Clock& c);
	virtual void increaseTime();
	virtual void setTime(int8_t hour,int8_t minute,int8_t second=0);
	friend std::ostream& operator<<(std::ostream& os,const Clock& c);
	std::string toString();
private:
	virtual Clock operator++();
	virtual Clock operator+(int i);
	virtual Clock operator-(int i);
};
}
