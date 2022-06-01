/*
 * sensors.c
 *
 *  Created on: 2022. ápr. 17.
 *      Author: hanic
 */

#include "main.h"
#include "sensors.h"

sensorArrayType sensorArray; //extern


void setHalfOfArrayValues(AnalogValuesType values, uint32_t signal){
	if(signal==0){
		sensorArray.irs1=values.irs12 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsplus=values.irs3plus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs4=values.irs45 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsminus=values.irs6minus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs7=values.irs78 > IRS78_COMPARE_LVL ? 1 : 0;
		sensorArray.irsx=values.irs9x > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs0=values.irs0dot > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsdivide=values.irsequaldivide > ADC_COMPARE_LVL ? 1 : 0;
	}
	else{
		sensorArray.irs2=values.irs12 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs3=values.irs3plus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs5=values.irs45 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs6=values.irs6minus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs8=values.irs78 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs9=values.irs9x > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsdot=values.irs0dot > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsequal=values.irsequaldivide > ADC_COMPARE_LVL ? 1 : 0;
	}
}//End of setHalfOfArrayValues() function

void setBitCalcArrayValues(AnalogValuesType values){
		sensorArray.irs1=values.irs12 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsplus=values.irs3plus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irs0=values.irs45 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsminus=values.irs6minus > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsdot=values.irs78 > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsx=values.irs9x > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsequal=values.irs0dot > ADC_COMPARE_LVL ? 1 : 0;
		sensorArray.irsdivide=values.irsequaldivide > ADC_COMPARE_LVL ? 1 : 0;


} //End of setBitCalcArrayValues() function

