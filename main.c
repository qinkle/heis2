#include "elev.h"
#include "queue.h"
#include "timer.h"
#include "FSM.h"
#include "stop.h"
#include <stdio.h>


int main(void) {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    // Go to defined state without heeding buttons
    elev_set_motor_direction(DIRN_UP);
    while (elev_get_floor_sensor_signal() == -1){}
    elev_set_motor_direction(DIRN_STOP);
    fsm_init();
    queue_init();


    // State machine while-loop, checks for events
    while (1) {
		if (fsm_check_for_orders()){
			fsm_order_placed();
		}

		if (stop_button_pressed()){
			fsm_stop_button_pressed();
            printf("Registered stop pressed in main \n");
		}

        if (stop_button_released()){
            fsm_stop_button_released();
            printf("Registered stop released in main \n");
        }

        if (timer_is_out()){
            fsm_timer_is_out();
            printf("Timer is out in main\n");
        }

        if (!queue_is_empty()){
            fsm_queue_not_empty();
        }

        if (elev_get_floor_sensor_signal() != -1){
            fsm_arrive_at_floor();
        }

    }
	
	
    return 0;
}
