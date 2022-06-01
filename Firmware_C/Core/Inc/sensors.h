/*
 * sensors.h
 *
 *  Created on: 2022. ápr. 17.
 *      Author: hanic
 */

#ifndef INC_SENSORS_H_
#define INC_SENSORS_H_

typedef struct{
	uint32_t irs1;
	uint32_t irs2;
	uint32_t irs3;
	uint32_t irsplus;
	uint32_t irs4;
	uint32_t irs5;
	uint32_t irs6;
	uint32_t irsminus;
	uint32_t irs7;
	uint32_t irs8;
	uint32_t irs9;
	uint32_t irsx;
	uint32_t irs0;
	uint32_t irsdot;
	uint32_t irsequal;
	uint32_t irsdivide;
}sensorArrayType;

extern sensorArrayType sensorArray;

void setHalfOfArrayValues(AnalogValuesType values, uint32_t signal);

#endif /* INC_SENSORS_H_ */
