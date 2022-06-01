/*
 * digoutputs.h
 *
 *  Created on: 2022. ápr. 17.
 *      Author: hanic
 */

#ifndef INC_DIGOUTPUTS_H_
#define INC_DIGOUTPUTS_H_

#include "main.h"
#include "lcd2x16.h"

#define DB0_PIN ((uint16_t)0)
#define DB1_PIN ((uint16_t)1)
#define DB2_PIN ((uint16_t)2)
#define DB3_PIN ((uint16_t)3)
#define DB4_PIN ((uint16_t)4)
#define DB5_PIN ((uint16_t)5)
#define DB6_PIN ((uint16_t)6)
#define DB7_PIN ((uint16_t)7)

#define VO_PWM_PIN ((uint16_t)8)
#define RS_PIN ((uint16_t)9)
#define RW_PIN ((uint16_t)10)
#define E_PIN ((uint16_t)11)
#define A_PWM_PIN ((uint16_t)12)



typedef struct{

	uint32_t DB0; //PC6
	uint32_t DB1; //PB9
	uint32_t DB2; //PC5
	uint32_t DB3; //PC12
	uint32_t DB4; //PC10
	uint32_t DB5; //PC11
	uint32_t DB6; //PD2
	uint32_t DB7; //PB7

	uint32_t VO_PWM; //PB12   //8
	uint32_t RS; //PC9		  //9
	uint32_t RW; //PC8		  //10
	uint32_t E; //PB8		  //11
	uint32_t A_PWM; //PC13	  //12
}outputLCDType;

extern outputLCDType outputLCD; //sztem externnel erdemes ha jol ertem externt

void setPWMPins(uint32_t VO_value, uint32_t A_value);
void setOutputPin(uint32_t pin, uint32_t value);
void setAllOutputPins(outputLCDType values);
void WriteLCDBinary(uint32_t rs, uint32_t rw, uint32_t binary);
void WriteLCDChar(uint32_t rs, uint32_t rw, char b);
void CommandTypeToOutputLCDType(outputLCDType *o,commandType command, uint32_t vo_pwm, uint32_t enable, uint32_t a_pwm);
void ClearScreen(void);
void ReturnHome(void);
void Return2ndRow(void);
void NumberToLCD(double num);

#endif /* INC_DIGOUTPUTS_H_ */
