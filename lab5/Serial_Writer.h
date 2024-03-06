#pragma once

#include "TinyTimber.h"

struct Serial_Writer
{
    Object obj;
};

#define initSerial_writer() initObject()

// NOTE required before both Serial writer and Serial Reader 
void init_uart();

void write_cyclic(struct Serial_Writer *self, int _);
