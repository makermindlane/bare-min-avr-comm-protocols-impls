#define F_CPU				16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "bmUart.h"

volatile unsigned char inChar;
volatile uint8_t sendEnable = 0;


int main(void) {
	uartInit(2400);
	sei();
	while (1) {
		if (sendEnable) {
			sendEnable = 0;
			uartSendChar(inChar);
		}
	}
}


void onReceiveIntt() {
	inChar = UDR0;
	sendEnable = 1;
}
