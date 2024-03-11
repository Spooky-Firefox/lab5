#include "Controller.h"

#define BLOCK_SIZE 5

// help functions to inc north queue and update display
#define inc_north()   self->queue_north = self->queue_north+1; \
                    ASYNC(self->gui, write_left, self->queue_north)

// help functions to dec north queue and update display
#define dec_north()   self->queue_north = self->queue_north-1; \
                    ASYNC(self->gui, write_left, self->queue_north)

// help functions to inc bridge queue and update display
#define inc_bridge()  self->on_bridge = self->on_bridge+1; \
                    ASYNC(self->gui, write_center, self->on_bridge);  \
                    AFTER(SEC(5), self, dec_bridge, NULL)

// help functions to inc south queue and update display
#define inc_south()   self->queue_south = self->queue_south+1; \
                    ASYNC(self->gui, write_right, self->queue_south)

// help functions to dec south queue and update display
#define dec_south()   self->queue_south = self->queue_south-1; \
                    ASYNC(self->gui, write_right, self->queue_south)

void send_north_green(struct Controller *self, int _){
    PORTE = (PORTE | (1<<PE4)) & ~(1<<PE6);
    if(self->last_state != 0x09){
        self->last_state = 0x09;
        ASYNC(self->ser_writer, write_byte, 0x09);
    }
    ASYNC(self->gui, blink_s1, NULL);
}

void send_south_green(struct Controller *self, int _){
    PORTE = (PORTE | (1<<PE6)) & ~(1<<PE4);
    if(self->last_state != 0x06){
        self->last_state = 0x06;
        ASYNC(self->ser_writer, write_byte, 0x06);
    }
    ASYNC(self->gui, blink_s2, NULL);
}

void send_red_light(struct Controller *self, int _){
    PORTE = PORTE & ~( (1<<PE4) | (1<<PE6));
    if(self->last_state != 0x05){
        self->last_state = 0x05;
        ASYNC(self->ser_writer, write_byte, 0x05);
    }
    // ASYNC(self->gui, blink_s1, NULL);
    // ASYNC(self->gui, blink_s2, NULL);
}

void dec_bridge(struct Controller *self, int _){
    self->on_bridge = self->on_bridge-1;
    ASYNC(self->gui, write_center, self->on_bridge);
}

void open_north(struct Controller *self, int times_left_to_open){
    if (times_left_to_open > 0 && self->queue_north > 0){
        send_north_green(self,NULL);
        AFTER(SEC(1), self, open_north, times_left_to_open-1);
    }
    else{  // switch green side
        if (self-> queue_north > 0 && self->queue_south == 0){
            send_north_green(self,NULL);
            AFTER(SEC(1), self, open_north, times_left_to_open);
        } else {
            if (self->on_bridge){
                send_red_light(self, NULL);
                AFTER(SEC(5), self, open_south, BLOCK_SIZE);
            } else {
                AFTER(MSEC(100), self, open_south, BLOCK_SIZE);
            }
        }

    }
}

void open_south(struct Controller *self, int times_left_to_open){
    if (times_left_to_open > 0 && self->queue_south > 0 ){
        send_south_green(self,NULL);
        AFTER(SEC(1), self, open_south, times_left_to_open-1);
    }
    else {
        if (self-> queue_south > 0 && self->queue_north == 0){
            send_south_green(self,NULL);
            AFTER(SEC(1), self, open_south, times_left_to_open);
        }
        else{ // switch green side
            if (self->on_bridge){
                send_red_light(self,NULL);
                AFTER(SEC(5), self, open_north, BLOCK_SIZE);
            }else{
                ASYNC(self, open_north, BLOCK_SIZE);
            }
        }
        
    }
}

// Bit 0: North car arrival sensor activated
// Bit 1: North bridge entry sensor activated
// Bit 2: South car arrival sensor activated
// Bit 3: South bridge entry sensor activated
void decode_msg(struct Controller *self, int msg){
    if(msg&0x1){ // Bit 0: North car arrival sensor activated
        inc_north();
    }
    
    if(msg&0x2){ // Bit 1: North bridge entry sensor activated
        inc_bridge();
        dec_north();
    }
    
    if(msg&0x4){ // Bit 2: South car arrival sensor activated
        inc_south();
    }
    if(msg&0x8){ // Bit 3: South bridge entry sensor activated
        dec_south();
        inc_bridge();
    }
}

void init_stuff(struct Controller *self, int _){

    // for bridge lights
    DDRE = DDRE | (1<<PE4) | (1<<PE6) | (1<<PE5);
    PORTE = PORTE | (1<<PE5);

    ASYNC(self, open_north, BLOCK_SIZE);
}
