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
#include "Gui.h"

struct Serial_Writer ser_wri = initSerial_writer();
struct Gui gui = initGui();
struct Controller cntr = initController(&ser_wri, &gui);
struct Serial_Reader ser_rea = innitSerial_Reader(&cntr);

int main(void)
{
    init_uart();
    setupLCD(&gui,NULL);
    init_interrupt(&ser_rea,NULL);

    TINYTIMBER(&cntr, init_stuff, NULL);
}

