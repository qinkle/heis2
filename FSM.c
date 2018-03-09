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


void fsm_order_placed(){
	switch (STATE) {

		case EMERGENCY_AT_FLOOR:
		break;

		case EMERGENCY_BETWEEN_FLOORS: 
		break;

		default:
		queue_order_made();
	}
}






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