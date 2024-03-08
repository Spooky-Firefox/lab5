#include "Serial_Writer.h"
#include "avr/io.h"

//#define FOSC 8000000 // clock speed
#define FOSC 7680000 // clock speed
#define BAUD 9600
#define MYUBRR ((FOSC/(BAUD)/16)-1)

void init_uart(){
    UBRR0H = (uint8_t) (MYUBRR >>8);
    UBRR0L = (uint8_t) MYUBRR;

    // enable transmitter and reciver
    UCSR0B = UCSR0B | (1<<TXEN0) | (1<<RXEN0);

    // char size is 8 bits
    UCSR0C = (3<<UCSZ00); // | (1<<UCSZ01);
}

void write_byte(struct Serial_Writer *self, int to_send){
    while(! (UCSR0A & (1<<UDRE0))){ // check if transmit buffer is empty, if not wait //TODO make beter without busy wait
        asm("NOP");
    };
    UDR0 = (uint8_t) to_send;
}

void write_string(struct Serial_Writer *self, char *str){
    for(int i = 0; str[i]; i++){
        while(! (UCSR0A & (1<<UDRE0))){ // check if transmit buffer is empty
            asm("NOP");
        };
        UDR0 = str[i];
    }
}
