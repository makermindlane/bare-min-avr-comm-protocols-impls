#pragma once
#include <avr/io.h>

#define F_CPU				16000000UL


void uartInit(uint32_t baudRate);

void uartSendChar(uint8_t ch);

uint8_t uartReceiveChar();