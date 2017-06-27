/*
 * timer.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>			// NULL
#include "timer.h"
#include "usart.h"

volatile TimerHandler STHandlers[8];

Timer::Timer(T0Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < 8; n++)
	{
		STHandlers[n].Active = false;
		STHandlers[n].Counter = 0;
		STHandlers[n].Interval = 0;
		STHandlers[n].fp = NULL;
	}
	TCCR0A |= (1 << WGM01); 		// CTC
	TCCR0B |= Prescaller;
	OCR0A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK0 |= (1 << OCIE0A);
}

void Timer::Assign(uint8_t HandlerNumber, uint64_t Interval, void(*fp)())
{
	STHandlers [HandlerNumber].Interval = Interval;
	STHandlers [HandlerNumber].Counter = 0;
	STHandlers [HandlerNumber].Active = true;
	STHandlers [HandlerNumber].fp = fp;
}

void Timer::Enable(uint8_t HandlerNumber)
{
	STHandlers [HandlerNumber].Active = true;
	STHandlers [HandlerNumber].Counter = 0;
}

void Timer::Disable(uint8_t HandlerNumber)
{
	STHandlers [HandlerNumber].Active = false;
}

void ModbusRTU35T()
{
	usart.RTU35T();
//	PORTC ^= (1 << 0);
}
ISR(TIMER0_COMPA_vect)
{
	for(uint8_t n = 0; n < 8; n++)
	{
		if ((STHandlers[n].Active) && (STHandlers[n].fp != NULL))
		{
			if ((STHandlers[n].Counter == STHandlers[n].Interval))
			{
				STHandlers[n].Counter = 0;
				STHandlers [n].fp();
			}
			else
			{
				STHandlers[n].Counter++;
			}
		}
	}
}


