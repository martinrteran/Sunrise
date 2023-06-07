/*
 * WindowAdapter.h
 *
 *  Created on: 3 jun. 2023
 *      Author: marti
 */

#ifndef INC_WINDOWADAPTER_H_
#define INC_WINDOWADAPTER_H_

#ifdef __cplusplus
#include <string>
#endif
namespace prSunrise{
	class WindowAdapter{
	protected:
		int _height;
		int _width;
	public:
		WindowAdapter(int h,int w):_height(h),_width(w){}
		virtual void printText(const char* array,int posX,int posY,void* font)=0;
		#ifdef __cplusplus
		virtual void printText(std::string,int posX,int posY,void* font)=0;
		#endif
		virtual void printCircle(int x,int y, double rad,double rot=0)=0;
		virtual void printSquare(int x,int y, double w, double h,double rot=0)=0;
		virtual void printTriangle(int x,int y,double b,double h,double rot=0)=0;
	};

}
#endif /* INC_WINDOWADAPTER_H_ */
