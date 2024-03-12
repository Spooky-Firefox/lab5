#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#include <semaphore.h>

// ansi start escaped data, 0x1b
// start control seq, [
// end control seq, E
#define COLOR(color_code) "\x1b[" color_code "m"

int north_queue = 0;
int south_queue = 0;
int green_north = 0;
int green_south = 0;
int on_bridge   = 0;

int last_north_queue = 1;
int last_south_queue = 1;
int last_green_north = 1;
int last_green_south = 1;
int last_on_bridge   = 1;

sem_t uart_mutex;
sem_t stdout_mutex;
sem_t data_mutex;

pthread_t bridge_thread[100];
int cur_on_bridge_index = 0;

int uart;
struct termios config;

    
void uart_send(char data){
    sem_wait(&uart_mutex);
    printf("I SENT %x\n", data);
    write(uart,&data,1);
    sem_post(&uart_mutex);
}

void* print_status(void* arg){
    while(1){
        // sem_wait(&data_mutex);
        if (north_queue != last_north_queue||
        south_queue != last_south_queue||
        green_north != last_green_north||
        green_south != last_green_south||
        on_bridge   != last_on_bridge){
        // sem_post(&data_mutex);
        // sem_wait(&stdout_mutex);
                if(green_north){
                    printf(COLOR("92"));
                } else {
                    printf(COLOR("91"));
                }
                printf("North light: %d", green_north);
                printf(COLOR("0"));
                printf(" North queue: %d", north_queue);
                if(green_south){
                    printf(COLOR("92"));
                } else {
                    printf(COLOR("91"));
                }
                printf(" South light: %d", green_south);
                printf(COLOR("0"));
                printf(" South queue: %d", south_queue);
                printf(" On Bridge: %d\n", on_bridge);
        // sem_post(&stdout_mutex);
                last_north_queue = north_queue;
                last_south_queue = south_queue;
                last_green_north = green_north;
                last_green_south = green_south;
                last_on_bridge   = on_bridge;
    };
    }
    
    return NULL;
}

void* remove_car_from_bridge(void* arg){
    sleep(4.9);

    // sem_wait(&data_mutex);
    on_bridge -= 1;
    // sem_post(&data_mutex);
    // printf("car left bridge \n");
    return NULL;
}

int init_uart(){
    int err;

    uart = open("/dev/ttyS2",O_RDWR | O_NOCTTY);

    // get curent config
    err = tcgetattr(uart, &config);
    if(err < 0){
        
        fprintf(stderr,COLOR("91") "error getting config %d", err);
        return -1;
    }
    // set baud to 9600
    if(cfsetospeed(&config,B9600)<0){
        fprintf(stderr,COLOR("91") "error setting baud %d", err);
        return -1;
    }
    // apply settings
    err = tcsetattr(uart,TCSAFLUSH, &config);
    if(err < 0) {
        fprintf(stderr,COLOR("91") "error setting attr %d", err);
        return -1;
    }
}

void send_user_input(){
    int cont = 1;
    char input;
    char to_write;
    while(cont){
        input = getchar();
        // sem_wait(&stdout_mutex); //ADDED
        // sem_wait(&data_mutex);
        if (input == 'n'){
            uart_send(0x01);
            north_queue += 1;
        } 
        else if (input == 's'){
            uart_send(0x04);
            south_queue += 1;
        }
        else if (input == 'e'){   
            printf("EXIT\n");
            cont = 0;
        } else if (input != '\n'){
            printf("not valid char: \"%s\"\n", &input);
        }
        // sem_post(&data_mutex);
        // sem_post(&stdout_mutex); //ADDED
    }
}

void* read_data(void* arg){
    while(1){
        // read the data
        char byte;
        while(read(uart,&byte,1)==0){};
        // sem_wait(&data_mutex);
        if((byte & 0x01)){ //if north bridge light is green, bit 0
            if(north_queue > 0){
                green_north = 1;
                // while(on_bridge > 4){}
                north_queue -= 1;
                on_bridge += 1;
                
                // send to controller "00000010"
                uart_send(0x02);

                cur_on_bridge_index += 1;
                pthread_create(&(bridge_thread[(cur_on_bridge_index-1)%100]), NULL, remove_car_from_bridge, NULL);
            }
            
        } 
        else {
            // printf("green red \n");
            green_north = 0;
        }
        
        if((byte & 0x04)){ //if south bridge light is green, bit 2
            if(south_queue > 0){
                green_south = 1;
                south_queue -= 1;
                on_bridge += 1;
                
                // send to controller "00001000"
                uart_send(0x08);

                cur_on_bridge_index += 1;
                pthread_create(&(bridge_thread[(cur_on_bridge_index-1)%100]), NULL, remove_car_from_bridge, NULL);
            }
            
        } 
        else{
            // printf("south red \n");
            green_south = 0;
        }

        if (byte & 0x0A){
        }
        // sem_post(&data_mutex);
    }
    return NULL;
}





int main(){
    if (init_uart() == -1){
        return -1;
    }

    sem_init(&uart_mutex,0,1);
    // sem_init(&data_mutex,0,0);
    // sem_init(&stdout_mutex,0,0);

    printf("program started\n");
    pthread_t rd;
    pthread_create(&rd, NULL, read_data, NULL);
    pthread_t ps;
    pthread_create(&ps, NULL, print_status, NULL);

    printf("started reading now taking user input \n");
    send_user_input();
    return(0);
}
