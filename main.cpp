/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "electromagnet.h"
#include "status.h"

#if defined (__AVR_ATmega88P__)
Timer timer(T0_PS_64, 229);
#endif
#if defined (__AVR_ATmega8__)
Timer timer(T1_PS_64, 229);
#endif

Usart usart;
UsartData usart_data;
ModbusRTU modbus_rtu;
Electromagnet electromagnet;

int main()
{
	AdcInit();
	DDRC |= (1 << 0); 		// led
	PORTC |= (1 << 0);
	sei();
	while(1);
}



