#pragma once

#include "TinyTimber.h"

struct Serial_Writer
{
    Object obj;
};

#define initSerial_writer() initObject()

// NOTE required before both Serial writer and Serial Reader 
void init_uart();

// send the given byte to the uart
void write_char(struct Serial_Writer *self, int to_send);
