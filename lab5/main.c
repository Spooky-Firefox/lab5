/*
 * lab5.c
 *
 * Created: 2024-03-05 11:16:17
 * Author : oller
 */ 

#include <avr/io.h>

#include "TinyTimber.h"
#include "Serial_Writer.h"
#include "Controller.h"
#include "Serial_Reader.h"

struct Serial_Writer ser_wri = initSerial_writer();
struct Controller cntr = initController(&ser_wri);
struct Serial_Reader ser_rea = innitSerial_Reader(&cntr);

int main(void)
{
    init_uart();
    UDR0 = 0xaa;
    TINYTIMBER(&ser_rea, init_interrupt, NULL);
}

