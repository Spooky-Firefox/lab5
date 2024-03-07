#pragma once

#include <avr/io.h>
#include "TinyTimber.h"
#include "Controller.h"

struct Serial_Reader
{
    Object obj;
    struct Controller *cntr;
};
#define innitSerial_Reader(controller_ref) {initObject(),controller_ref}

//installs self as interuppt handler for recived
void init_interrupt(struct Serial_Reader *self, int _);
