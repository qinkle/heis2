#include "elev.h"
#include "queue.h"
//#include "timer.h"
#include "FSM.h"


enum FSM_STATE {
    ELEVATOR_MOVES,
    ELEVATOR_STOPPED,
    EMERGENCY_BETWEEN_FLOORS,
    EMERGENCY_AT_FLOOR,
    DOORS_OPEN,
} STATE; // Holds curent state


void fsm_init(){
	STATE = ELEVATOR_STOPPED;
}



//  FSM- state behaviour


void fsm_order_placed(){
	switch (STATE) {
		
		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		
		case EMERGENCY_AT_FLOOR:
			break;

		default:
			queue_order_made();
	}
}

void fsm_stop_button_pressed(){

	switch (STATE) {
		case ELEVATOR_MOVES:
			STATE = EMERGENCY_BETWEEN_FLOORS;
			elev_set_motor_direction(DIRN_STOP);
		case ELEVATOR_STOPPED:
			if (elev_get_floor_sensor_signal() != -1){
				STATE = EMERGENCY_AT_FLOOR;
				elev_set_door_open_lamp(1);
			}
			else {
				STATE = EMERGENCY_BETWEEN_FLOORS;
			}
		case EMERGENCY_BETWEEN_FLOORS:
			break;
		case EMERGENCY_AT_FLOOR:
			break;
		

		default:
			queue_clear_all_orders();
			elev_set_stop_lamp(1);
			break;
	}
}








//FSM-utility functions


int fsm_check_for_orders(){
	int order_is_made = 0;
	for (int button = 0; button < N_BUTTONS; button++){
		for (int floor = 0; floor < N_FLOORS; floor++){
			if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) ){
				if (elev_get_button_signal(button, floor)){
					order_is_made = 1;
				}
			}
		}
	}
	return order_is_made;
}