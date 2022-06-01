/*
 * lcd2x16.h
 *
 *  Created on: Apr 13, 2022
 *      Author: hanic
 */

#ifndef INC_LCD2X16_H_
#define INC_LCD2X16_H_
#include "main.h"


typedef struct commandType{
	uint32_t DB0; //PC6       //0
	uint32_t DB1; //PB9
	uint32_t DB2; //PC5
	uint32_t DB3; //PC12
	uint32_t DB4; //PC10
	uint32_t DB5; //PC11
	uint32_t DB6; //PD2
	uint32_t DB7; //PB7		  //7

	uint32_t RS; //PC9		  //9  (8 is left out since VO_PWM_PIN equals 8)
	uint32_t RW; //PC8		  //10

}commandType;

extern commandType command;

int setCommand(char character,uint32_t rs, uint32_t rw);
void setClearScreenCommand(void);

#endif /* INC_LCD2X16_H_ */
