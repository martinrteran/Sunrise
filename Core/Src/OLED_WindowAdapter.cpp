/*
 * OLED_WindowAdapter.cpp
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */


#include "OLED_WindowAdapter.h"
#include "stm32f4xx_hal_i2c.h"

namespace prSunrise{

#define WRITE_COMMAND(cmd) WriteToDisp(cmd,COMMAND)
#define WRITE_DATA(dt) WriteToDisp(dt,DATA)
#define TURN_ON_DISP() WRITE_COMMAND(0x8D);WRITE_COMMAND(0x14);WRITE_COMMAND(0xAF)

	SSD1306_Adapter::SSD1306_Adapter(I2C_HandleTypeDef* hic):WindowAdapter(DISP_HEIGHT,DISP_WIDTH),
			_hi2c(hic),_font(0),_color(BLACK),_cursor(0,0){
		DISP_INIT();
	}
	//uint8_t SSD1306_Adapter::s_memoryDir=0b01111000;

	SSD1306_Adapter::~SSD1306_Adapter(){}

	void SSD1306_Adapter::setFont(FontDef_t* f){
		if(_font!=0)
			delete _font;
		this->_font=f;
	}
		//void setMemoryDirection(auto _mdir);
		void SSD1306_Adapter::setColor(color_t _c);
		void SSD1306_Adapter::setCursor(int x,int y){
			if(y<0||x<0)return;
			_cursor.setCursor(x, y);
		}
	#ifdef __cplusplus
		void SSD1306_Adapter::printText(std::string,int posX,int posY,void* font){

		}
	#endif
		void SSD1306_Adapter::WriteToDisp(uint8_t data,writeMode_t wM){
			uint8_t dataBuff[2];
			dataBuff[0]=wM;
			dataBuff[1]=data;
			HAL_I2C_Master_Transmit(_hi2c,s_memoryDir, dataBuff, 2, 10);
		}
		void SSD1306_Adapter::WriteData(uint8_t* data,int size){
			uint8_t dt[size+1];
			dt[0]=DATA;
			memcpy(dt+1,data,size);
			HAL_I2C_Master_Transmit(_hi2c, s_memoryDir, dt, size+1, 10);
		}
		void SSD1306_Adapter::DISP_INIT(){
			//TURN OFF THE DISPLAY
			WRITE_COMMAND(0xAE);
				{// ADDRESSING MODE
					WRITE_COMMAND(0x20);
						WRITE_COMMAND(0x01);// vertical addressing mode, 00 for horizontal and 10 for page one.
					WRITE_COMMAND(0x21);//Set up the column addresses
						WRITE_COMMAND(0x00);//first column
						WRITE_COMMAND(0x7F);//last column
					WRITE_COMMAND(0x22);// Set up pages addresses
						WRITE_COMMAND(0x00);//first
						WRITE_COMMAND(0x7);//last
				}
				{// SET MULTIPLEXER SIZE (SPACE OF THE BUFFER AVAILABLE)
					WRITE_COMMAND(0xA8);
					WRITE_COMMAND(0x3F);
				}
				{// SET DISPLAY OFFSET
					WRITE_COMMAND(0xD3);
					WRITE_COMMAND(0x00);//0 offset
				}
				{ // SET START LINE
					WRITE_COMMAND(0x40);//0x40
				}
				{//SET SEGMENT REMAP (COLUMNS) AND SET COM OUTPUT SCAN DIRECTION
					WRITE_COMMAND(0xA1);//COLUMNS REMAP
					WRITE_COMMAND(0xC8);//UP DOWN
				}
				{//SET COM PINS CONFIG
					WRITE_COMMAND(0xDA);
					WRITE_COMMAND(0x12);//(RESET)
				}
				{
				//Set Contrast
					WRITE_COMMAND(0x81);
					WRITE_COMMAND(0x7F);//CONTRAST LEVEL
				}
				{//ON DISP-RAM AND NORMAL MODE
					WRITE_COMMAND(0xA4);
					WRITE_COMMAND(0xA6);// Set normal mode for RAM
				}
				{
					//SET UP CLOCK
					WRITE_COMMAND(0xD5);
					WRITE_COMMAND(0x80);
				}
				{// PRE-CHARGE PERIOD
					WRITE_COMMAND(0xD9);
					WRITE_COMMAND(0x22);
				}
				{//SET COMMON HIGH VOLTAGE DESELECT LEVEL
					WRITE_COMMAND(0xDB);
					WRITE_COMMAND(0x20);
				}
			    TURN_ON_DISP();
		}


}
