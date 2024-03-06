/*
 * lab5.c
 *
 * Created: 2024-03-05 11:16:17
 * Author : oller
 */ 

#include <avr/io.h>

#include "TinyTimber.h"
#include "Serial_Writer.h"

struct Serial_Writer ser_wri = initSerial_writer();

int main(void)
{
    init_uart();
    TINYTIMBER(&ser_wri, write_cyclic, NULL);
}

