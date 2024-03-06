#include "Controller.h"


// Bit 0: Northbound car arrival sensor activated
// Bit 1: Northbound bridge entry sensor activated
// Bit 2: Southbound car arrival sensor activated
// Bit 3: Southbound bridge entry sensor activated
void decode(struct Controller *self, int msg){
    if(msg&0x1){ // Bit 0: Northbound car arrival sensor activated
        self->queue_north = self->queue_north+1;

        // write NI, north in for debug purpose
        ASYNC(self->ser_writer,write_char,'n'); //TODO remove after testing
        ASYNC(self->ser_writer,write_char,'i'); //TODO remove after testing
    }
    if(msg&0x2){ // Bit 1: Northbound bridge entry sensor activated
        self->queue_north = self->queue_north+1;
        //TODO remove car after 5 sec, example AFTER(SEC(5),self, remove_bridge_car, NULL)
        // "se For our particular scenario, we will assume that a reasonable time for passing the bridge once it is entered is 5 seconds."

        // write NO, north out for debug purpose
        ASYNC(self->ser_writer,write_char,'n'); //TODO remove after testing
        ASYNC(self->ser_writer,write_char,'o'); //TODO remove after testing
    }


    
    if(msg&0x4){ // Bit 2: Southbound car arrival sensor activated
        self->queue_north = self->queue_north+1;

        // write SI, north in for debug purpose
        ASYNC(self->ser_writer,write_char,'s'); //TODO remove after testing
        ASYNC(self->ser_writer,write_char,'i'); //TODO remove after testing
    }
    if(msg&0x8){ // Bit 3: Southbound bridge entry sensor activated
        self->queue_north = self->queue_north+1;
        //TODO remove car after 5 sec, example AFTER(SEC(5),self, remove_bridge_car, NULL)
        // "se For our particular scenario, we will assume that a reasonable time for passing the bridge once it is entered is 5 seconds."

        // write SO, north out for debug purpose
        ASYNC(self->ser_writer,write_char,'s'); //TODO remove after testing
        ASYNC(self->ser_writer,write_char,'o'); //TODO remove after testing
    }

    // call light handler
}


