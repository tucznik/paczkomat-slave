/*
 * elecromagnet.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include "electromagnet.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "usart.h"

Electromagnet::Electromagnet() : Machine(ST_MAX_STATES)
{
	ELECTROMAGNET_INIT;
}

void MakeTest()
{
	timer.Disable(3);
	if(ELECTROMAGNET_TEST_COIL_PPIN & (1 << ELECTROMAGNET_TEST_COIL_PIN)) modbus_rtu.HoldingRegisters[0] = 35;
	else
		modbus_rtu.HoldingRegisters[0] = 200;
}

void ElectromOff()
{
	ELECTROMAGNET_OFF;
	modbus_rtu.PrepareFrame(usart_data.frame);
	timer.Disable(2);
	electromagnet.Event(0, NULL); // na chama
}

void Electromagnet::TestCoil(ElectromagnetData* pdata)
{
	const uint8_t Transitions[] =
	{
		// next						// current
		ST_ON,						// ST_OFF
		ST_NOT_ALLOWED, 			// ST_ON
		ST_NOT_ALLOWED				// ST_TEST
	};
	Event(Transitions[current_state], pdata);
}

void Electromagnet::ST_Off(ElectromagnetData* pdata)
{

}

void Electromagnet::ST_On(ElectromagnetData* pdata)
{
	ELECTROMAGNET_ON;
	timer.Assign(2, 200, ElectromOff);
	timer.Assign(3, 100, MakeTest);
}

void Electromagnet::ST_Test(ElectromagnetData* pdata)
{

}
