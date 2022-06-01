/*
 * lcd2x16.c
 *
 *  Created on: Apr 13, 2022
 *      Author: hanic
 */


#include "main.h"
#include "lcd2x16.h"
#include "digoutputs.h"

//Utasitasok:
//RS RW | DB7-DB0
// 0  0 | 0 0 1 L N F X X   L: 8bit/4bit bus (1/0), N: 2 or 1 line display (1/0), F: 5x10 char display or 5x8 (1/0)
// 0  0 | 00000001 Clear display
// 0  0 | 00000010 Return home
// 0  0 | 00001dcb d: display on/off (1/0), c: cursor on/off (1/0), b: cursor blink on/off (1/0)
// 1  0 | ascii    Write character
// ...
//(0  1 | BF ... BF: Busy Flag, if 1, then we have to wait)   inkabb nem hasznalom, ha nem kell, problematikus

//E: Enable
//	Timing:
// 1) Change RS / R/W if needed, MAYBE need to wait at least 100ns (delay(1))
// 2) Change DB7-DB0, delay
// 3) Set E enable to 1
// 4) After waiting for example 1ms, reset enable



int setCommand(char character,uint32_t rs, uint32_t rw){

	int ascii = (int) character;

	command.RS=rs;
	command.RW=rw;

	command.DB7 = (ascii%256) < 128 ? 0 : 1;
	command.DB6 = (ascii%128) < 64 ? 0 : 1;
	command.DB5 = (ascii%64) < 32 ? 0 : 1;
	command.DB4 = (ascii%32) < 16 ? 0 : 1;
	command.DB3 = (ascii%16) < 8 ? 0 : 1;
	command.DB2 = (ascii%8) < 4 ? 0 : 1;
	command.DB1 = (ascii%4) < 2 ? 0 : 1;
	command.DB0 = (ascii%2) < 1 ? 0 : 1;

	return ascii;
} // End of setCommand() function

void setClearScreenCommand(void){

	command.RS=0;
	command.RW=0;

	command.DB0=1;
	command.DB1=0;
	command.DB2=0;
	command.DB3=0;
	command.DB4=0;
	command.DB5=0;
	command.DB6=0;
	command.DB7=0;

}


