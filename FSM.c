#include "elev.h"
#include "queue.h"
#include "timer.h"
#include "FSM.h"
#include <stdio.h>


enum FSM_STATE {
    ELEVATOR_MOVES,
    ELEVATOR_STOPPED,
    EMERGENCY_BETWEEN_FLOORS,
    EMERGENCY_AT_FLOOR,
    DOORS_OPEN,
} STATE; // Holds curent state


void fsm_init(void){
	STATE = ELEVATOR_STOPPED;
}



//  FSM- state behaviour


void fsm_order_placed(void){

	

	switch (STATE) {

	
		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		case EMERGENCY_AT_FLOOR:
			break;

		case DOORS_OPEN:
			queue_clear_floor();
			break;
		default:
			queue_order_made();
			break;
	}
}

void fsm_stop_button_pressed(void){
	printf("Registered stop\n");
	queue_clear_all_orders();
	elev_set_stop_lamp(1);

	switch (STATE) {
		case ELEVATOR_MOVES:
			elev_set_motor_direction(DIRN_STOP);

			if (elev_get_floor_sensor_signal() != -1){
				printf("stop_button_pressed does\n");
				printf("From ELEVATOR_MOVES				to EMERGENCY_AT_FLOOR\n");
				STATE = EMERGENCY_AT_FLOOR;
				elev_set_door_open_lamp(1);
			}
			else{
				printf("stop_button_pressed does\n");
				printf("From ELEVATOR_MOVES				to EMERGENCY_BETWEEN_FLOORS\n");
				STATE = EMERGENCY_BETWEEN_FLOORS;
			}
			break;

		case ELEVATOR_STOPPED:
			if (elev_get_floor_sensor_signal() != -1){
				printf("stop_button_pressed does\n");
				printf("From ELEVATOR_STOPPED			to EMERGENCY_AT_FLOOR\n");
				STATE = EMERGENCY_AT_FLOOR;
				elev_set_door_open_lamp(1);
			}
			else {
				printf("stop_button_pressed does\n");
				printf("From ELEVATOR_STOPPED			to EMERGENCY_BETWEEN_FLOORS\n");
				STATE = EMERGENCY_BETWEEN_FLOORS;
			}
			break;

		case DOORS_OPEN:
			printf("stop_button_pressed does\n");
			printf("From DOORS_OPEN		 			to EMERGENCY_AT_FLOOR\n");
			timer_stop();
			STATE = EMERGENCY_AT_FLOOR;
			break;
		
		default:
			break;
	}
}

void fsm_stop_button_released(void){

	elev_set_stop_lamp(0);

	switch (STATE) {

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			break;

		case EMERGENCY_BETWEEN_FLOORS:
			printf("stop_button_released does\n");
			printf("From EMERGENCY_BETWEEN_FLOORS 	to ELEVATOR_STOPPED\n");
			STATE = ELEVATOR_STOPPED;
			break;
		
		case EMERGENCY_AT_FLOOR:
			printf("stop_button_released does\n");
			printf("From EMERGENCY_AT_FLOOR		 	to DOORS_OPEN\n");
			timer_start();
			STATE = DOORS_OPEN;
			break;

		case DOORS_OPEN:
			break;

		default:
			break;
	}
}

void fsm_timer_is_out(void){


	switch (STATE) {

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			break;

		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		
		case EMERGENCY_AT_FLOOR:
			break;

		case DOORS_OPEN:
			printf("timer_is_out does\n");
			printf("From DOORS_OPEN		 		to ELEVATOR_STOPPED\n");
			timer_stop();
			elev_set_door_open_lamp(0);
			STATE = ELEVATOR_STOPPED;
			break;

		default:
			break;
	}

}

void fsm_queue_not_empty(void){

	switch (STATE) {

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			if (queue_floor_is_ordered() && (elev_get_floor_sensor_signal() != -1)){
				timer_start();
				elev_set_door_open_lamp(1);
				queue_clear_floor();
				printf("queue_not_empty does\n");
				printf("From ELEVATOR_STOPPED			to DOORS_OPEN\n");
				STATE = DOORS_OPEN;
			}
			else{
				elev_set_motor_direction(queue_get_direction());
				printf("queue_not_empty does\n");
				printf("From ELEVATOR_STOPPED			to ELEVATOR_MOVES\n");
				STATE = ELEVATOR_MOVES;
			}
			break;

		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		
		case EMERGENCY_AT_FLOOR:
			break;

		case DOORS_OPEN:
			break;

		default:
			break;
	}

}

void fsm_arrive_at_floor(void){
	queue_update_floor();
	switch (STATE) {

		case ELEVATOR_MOVES:
			if (queue_stop_here()){
				elev_set_motor_direction(0);
				printf("arrive_at_floor does\n");
				printf("From ELEVATOR_MOVES		 	to ELEVATOR_STOPPED\n");
				STATE = ELEVATOR_STOPPED;
			}
			break;

		case ELEVATOR_STOPPED:
			break;

		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		
		case EMERGENCY_AT_FLOOR:
			break;

		case DOORS_OPEN:
			break;

		default:
			break;
	}

}





//FSM-utility functions


int fsm_check_for_orders(void){
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