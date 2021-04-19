#include "bmUart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


void uartInit(uint32_t baudRate) {
	/* Converting the baud rate to a value for USART baud rate register (UBRR0).
	See table 19-1 on page 146 and section 19.11 Examples of baud rate settings
	for some common clock speed.
	
	lrint() is used to round of to nearest integer.
	*/ 
	uint16_t baudRateRegister = lrint((F_CPU / (16L * baudRate)) - 1);
	UBRR0 = baudRateRegister;
	/* Clearing the U2X0 bit of UCSR0A register, means disabling the 2x transmission mode
	of USART. For some reason this bit set at the start which it shouldn't. Hence
	clearing it (setting it to 0).*/
	UCSR0A &= ~(1 << U2X0);
	// Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// Enable the receive complete interrupt by setting the RXCIE0 bit in UCSR0B register.
	UCSR0B |= (1 << RXCIE0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C |= (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}


void uartSendChar(uint8_t ch) {
	/* Wait until USART data register empty bit is set in USART control and 
	status register A, means if UDRE0 bit is set, the transmit buffer is empty
	and ready to receive data */
	while (!(UCSR0A & (1 << UDRE0)));
	// Put data in UDR0 register. Putting data into this register begins the transmission automatically.
	UDR0 = ch;
}


uint8_t uartReceiveChar() {
	/* Wait until RXC0 (receive complete) bit is set in USART control and status register, 
	means receive is complete and data is available in receive buffer.*/
	while (!(UCSR0A & (1 << RXC0)));
	// return the data received in USART data register.
	return UDR0;
}


extern void onReceiveIntt();
/*
	NOTE: Generally it's not recommended to call a function from inside an ISR.
*/
ISR(USART_RX_vect) {
	onReceiveIntt();
}
