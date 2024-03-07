#include "Serial_Reader.h"
#include "Controller.h"

void read_uart(struct Serial_Reader *self, int _){
	int data = (int) UDR0;
	ASYNC(self->cntr, decode_msg, data);
}

void init_interrupt(struct Serial_Reader *self, int _){
    // enable USART RX COMPLETE
    UCSR0B = UCSR0B | (1<<RXCIE0);

    INSTALL(self, read_uart, IRQ_USART0_RX);
}
