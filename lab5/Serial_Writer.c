#include "Serial_Writer.h"
#include "avr/io.h"

#define FOSC 8000000/7 // clock speed, for some reason i need to divide by 7
#define BAUD 9600
#define MYUBRR ((FOSC/(16*BAUD))-1)

void init_uart(){
    UBRR0H = (uint8_t) (MYUBRR >>8);
    UBRR0L = (uint8_t) MYUBRR;

    // enable transmitter and reciver
    UCSR0B = (1<<TXEN0) | (1<<RXEN0);

    // char size is 8 bits
    UCSR0C = (3<<UCSZ00); // | (1<<UCSZ01);
}

void write_char(struct Serial_Writer *self, int to_send){
    while(! (UCSR0A & (1<<UDRE0))){ // check if transmit buffer is empty, if not wait //TODO make beter without busy wait
        asm("NOP");
    };
    UDR0 = (uint8_t) to_send;
}
