#pragma once

#include "TinyTimber.h"
#include "Serial_Writer.h"
#include <stdint.h>

struct Controller
{
    Object obj;
    struct Serial_Writer *ser_writer;
    
    uint8_t on_bridge;

    uint8_t queue_north;

    uint8_t queue_south;
    
};
#define initController(serial_writer_ref) {initObject(), serial_writer_ref, 0, 0, 0}

// decodes the byte sent, handels if car passed a sensor.
// Bit 0: Northbound car arrival sensor activated
// Bit 1: Northbound bridge entry sensor activated
// Bit 2: Southbound car arrival sensor activated
// Bit 3: Southbound bridge entry sensor activated
void decode_msg(struct Controller *self, int msg);
