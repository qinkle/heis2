#include "elev.h"
#include "queue.h"
//#include "timer.h"
#include "FSM.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    //Go to defined state without heeding buttons
    elev_set_motor_direction(DIRN_UP);
    while (elev_get_floor_sensor_signal() == -1){}
    elev_set_motor_direction(DIRN_STOP);
    fsm_init();
    queue_init();



    while (1) {
      if (fsm_check_for_orders()){
        fsm_order_placed();
      }
    }
    
    return 0;
}
