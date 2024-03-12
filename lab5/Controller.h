#pragma once

#include <stdint.h>
#include "TinyTimber.h"
#include "Serial_Writer.h"
#include "Gui.h"

struct Controller
{
    Object obj;

    struct Serial_Writer *ser_writer;
    struct Gui *gui;

    uint8_t on_bridge;

    uint8_t queue_north;

    uint8_t queue_south;
    
};
#define initController(serial_writer_ref, gui_ref) {initObject(), serial_writer_ref, gui_ref, 0, 0, 0}

// decodes the byte sent, handels if car passed a sensor.
// Bit 0: Northbound car arrival sensor activated
// Bit 1: Northbound bridge entry sensor activated
// Bit 2: Southbound car arrival sensor activated
// Bit 3: Southbound bridge entry sensor activated
void decode_msg(struct Controller *self, int msg);

void init_stuff(struct Controller *self, int _);

// NOT FOR "PUBLIC" USE
void open_south(struct Controller *self, int times_left_to_open);

// NOT FOR "PUBLIC" USE
void open_north(struct Controller *self, int times_left_to_open);
