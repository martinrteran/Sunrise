/*
 * OLED_WindowAdapter.h
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */

#ifndef INC_OLED_WINDOWADAPTER_H_
#define INC_OLED_WINDOWADAPTER_H_

#include "WindowAdapter.h"
#include"ssd1306.h"


namespace prSunrise{

#define DISP_HEIGHT 64
#define DISP_WIDTH 128
#define BUFF_SIZE DISP_HEIGHT*DISP_WIDTH/8;
	typedef enum Color_t{BLACK=0x00,WHITE=0x00}color_t;
	typedef enum WriteMode{COMMAND=0x00,DATA=0x40}writeMode_t;

	class Cursor{
		int _x,_y;
	public:
		inline Cursor(int x,int y):_x(x),_y(y){}
		inline void setCursor(int x,int y){_x=x;_y=y;}
		inline int getX() const{return _y;}
		inline int getY()const{return _x;}
	};

	class SSD1306_Adapter:public WindowAdapter{

		I2C_HandleTypeDef* _hi2c;
		FontDef_t* _font;
		color_t _color;
		uint8_t _buff[s_buffSize];
		Cursor _cursor;

		static const uint8_t s_memoryDir=0b01111000;
		static const uint8_t s_height=DISP_HEIGHT;
		static const uint8_t s_width=DISP_WIDTH;
		static const uint16_t s_buffSize=BUFF_SIZE;
	public:
		SSD1306_Adapter(I2C_HandleTypeDef* hi2c=0);
		~SSD1306_Adapter();
		void setFont(FontDef_t* f);
		//void setMemoryDirection(auto _mdir);
		void setColor(color_t _c);
		void setCursor(int x,int y);
	#ifdef __cplusplus
		virtual void printText(std::string,FontDef_t* font,int posX=-1,int posY=-1);
	#endif
		void printText(const char*,FontDef_t*,int =-1,int=-1);
		void printText(char,FontDef_t*,int =-1,int =-1);
		virtual void printCircle(double rad,double rot=0,int x=-1,int y=-1);
		virtual void printSquare(double w, double h,double rot=0,int=-1,int=-1);
		virtual void printTriangle(double b,double h,double rot=0,int x=-1,int y=-1);
	private:
		void DISP_INIT();
		void WriteToDisp(uint8_t,writeMode_t);
		void WriteData(uint8_t*,int);
	};
}


#endif /* INC_OLED_WINDOWADAPTER_H_ */
