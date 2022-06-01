/*
 * digoutputs.c
 *
 *  Created on: 2022. ápr. 17.
 *      Author: hanic
 */

//#include "bsp.h"
#include <stdint.h>
#include "main.h"
#include "digoutputs.h"
#include "lcd2x16.h"


void setPWMPins(uint32_t VO_value, uint32_t A_value){
	if(VO_value>0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
		outputLCD.VO_PWM=1;
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		outputLCD.VO_PWM=0;
	}


	if(A_value>0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
		outputLCD.A_PWM=1;
	}
	else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		outputLCD.A_PWM=0;
	}
}


void setOutputPin(uint32_t pin, uint32_t value){
	switch(pin){
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, value);
			outputLCD.DB0=value;
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, value);
			outputLCD.DB1=value;
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, value);
			outputLCD.DB2=value;
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, value);
			outputLCD.DB3=value;
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, value);
			outputLCD.DB4=value;
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, value);
			outputLCD.DB5=value;
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, value);
			outputLCD.DB6=value;
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, value);
			outputLCD.DB7=value;
			break;

		case 8:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, value);
			outputLCD.VO_PWM=value;
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, value);
			outputLCD.RS=value;
			break;
		case 10:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, value);
			outputLCD.RW=value;
			break;
		case 11:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, value);
			outputLCD.E=value;
			break;
		case 12:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, value);
			outputLCD.A_PWM=value;
			break;
	}//End of switch(pin)

} //End of setOutputPin() function


void setAllOutputPins(outputLCDType values){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, values.DB0);
	outputLCD.DB0=values.DB0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, values.DB1);
	outputLCD.DB1=values.DB1;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, values.DB2);
	outputLCD.DB2=values.DB2;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, values.DB3);
	outputLCD.DB3=values.DB3;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, values.DB4);
	outputLCD.DB4=values.DB4;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, values.DB5);
	outputLCD.DB5=values.DB5;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, values.DB6);
	outputLCD.DB6=values.DB6;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, values.DB7);
	outputLCD.DB7=values.DB7;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, values.VO_PWM);
	outputLCD.VO_PWM=values.VO_PWM;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, values.RS);
	outputLCD.RS=values.RS;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, values.RW);
	outputLCD.RW=values.RW;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, values.E);
	outputLCD.E=values.E;

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, values.A_PWM);
	outputLCD.A_PWM=values.A_PWM;

} //End of setAllOutputPins() function



void WriteLCDBinary(uint32_t rs, uint32_t rw, uint32_t binary){
	if(rs>0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
		outputLCD.RS=1;
	}
	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
		outputLCD.RS=0;
	}

	if(rs>0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
		outputLCD.RS=1;
	}
	else{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
		outputLCD.RS=0;
	}

	HAL_Delay(1);
	commandType commandf;

	commandf.RS=outputLCD.RS;
	commandf.RW=outputLCD.RW;
	commandf.DB7 = (binary%256) < 128 ? 0 : 1;
	commandf.DB6 = (binary%128) < 64 ? 0 : 1;
	commandf.DB5 = (binary%64) < 32 ? 0 : 1;
	commandf.DB4 = (binary%32) < 16 ? 0 : 1;
	commandf.DB3 = (binary%16) < 8 ? 0 : 1;
	commandf.DB2 = (binary%8) < 4 ? 0 : 1;
	commandf.DB1 = (binary%4) < 2 ? 0 : 1;
	commandf.DB0 = (binary%2) < 1 ? 0 : 1;

	//outputLCDType out;
	CommandTypeToOutputLCDType(&outputLCD, commandf, outputLCD.VO_PWM , 0, outputLCD.A_PWM);
	setAllOutputPins(outputLCD);
	HAL_Delay(1);
	setOutputPin(E_PIN, 1);
	HAL_Delay(1);
	setOutputPin(E_PIN, 0);


} //End of WriteLCDBinary() function

void WriteLCDChar(uint32_t rs, uint32_t rw, char b){
	uint32_t ascii = (int) b;
	WriteLCDBinary(rs, rw, ascii);
}

void CommandTypeToOutputLCDType(outputLCDType *o, commandType command, uint32_t vo_pwm, uint32_t enable, uint32_t a_pwm){
	//outputLCDType o;
	o->DB0=command.DB0;
	o->DB1=command.DB1;
	o->DB2=command.DB2;
	o->DB3=command.DB3;
	o->DB4=command.DB4;
	o->DB5=command.DB5;
	o->DB6=command.DB6;
	o->DB7=command.DB7;
	o->RS=command.RS;
	o->RW=command.RW;
	o->A_PWM = a_pwm;
	o->E=enable;
	o->VO_PWM = vo_pwm;

	//setAllOutputPins(o);
}

void ClearScreen(void){
	//outputLCDType o;
	setClearScreenCommand();
	CommandTypeToOutputLCDType(&outputLCD, command, outputLCD.VO_PWM, outputLCD.E, outputLCD.A_PWM);
	setAllOutputPins(outputLCD);
}

void ReturnHome(void){
	WriteLCDBinary(1, 0, 2);
}
void Return2ndRow(void){
	WriteLCDBinary(1, 0, 128+64);
}

void NumberToLCD(double num){
	//
}
