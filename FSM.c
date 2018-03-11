// FSM.c
// These functions provide events and utilities for the Finite State Machine (FSM)


#include "elev.h"
#include "queue.h"
#include "timer.h"
#include "FSM.h"
#include <stdio.h>




// Holds current state
enum FSM_STATE 
{
    ELEVATOR_MOVES,
    ELEVATOR_STOPPED,
    EMERGENCY_BETWEEN_FLOORS,
    EMERGENCY_AT_FLOOR,
    DOORS_OPEN,
} STATE;


void
fsm_init(void)
{
	STATE = ELEVATOR_STOPPED;
}



// FSM behaviour
// printf statements are for monitoring the FSM
void
fsm_order_placed(void)
{

	switch (STATE) 
	{

		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		case EMERGENCY_AT_FLOOR:
			break;
		case DOORS_OPEN:
			queue_order_made();
			queue_clear_floor();
			break;
		default:
			queue_order_made();
			break;
	}
}

void 
fsm_stop_button_pressed(void)
{
	printf("Registered stop\n");
	queue_clear_all_orders();
	elev_set_stop_lamp(1);

	switch (STATE) 
	{
		case ELEVATOR_MOVES:
			elev_set_motor_direction(DIRN_STOP);

			if (elev_get_floor_sensor_signal() != -1){
				elev_set_door_open_lamp(1);
                STATE = EMERGENCY_AT_FLOOR;
                
                printf("stop_button_pressed does\n");
                printf("From ELEVATOR_MOVES                to EMERGENCY_AT_FLOOR\n");
			}
			else{
				STATE = EMERGENCY_BETWEEN_FLOORS;
                
                printf("stop_button_pressed does\n");
                printf("From ELEVATOR_MOVES                to EMERGENCY_BETWEEN_FLOORS\n");
			}
			break;

		case ELEVATOR_STOPPED:
			if (elev_get_floor_sensor_signal() != -1){
				elev_set_door_open_lamp(1);
                STATE = EMERGENCY_AT_FLOOR;
                
                printf("stop_button_pressed does\n");
                printf("From ELEVATOR_STOPPED            to EMERGENCY_AT_FLOOR\n");
			}
			else {
				STATE = EMERGENCY_BETWEEN_FLOORS;
                
                printf("stop_button_pressed does\n");
                printf("From ELEVATOR_STOPPED            to EMERGENCY_BETWEEN_FLOORS\n");
			}
			break;

		case DOORS_OPEN:
			timer_stop();
			STATE = EMERGENCY_AT_FLOOR;
            
            printf("stop_button_pressed does\n");
            printf("From DOORS_OPEN                     to EMERGENCY_AT_FLOOR\n");
			break;
		
		default:
			break;
	}
}

void 
fsm_stop_button_released(void)
{

	elev_set_stop_lamp(0);

	switch (STATE) 
	{

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			break;

		case EMERGENCY_BETWEEN_FLOORS:
			STATE = ELEVATOR_STOPPED;
            
            printf("stop_button_released does\n");
            printf("From EMERGENCY_BETWEEN_FLOORS     to ELEVATOR_STOPPED\n");
			break;
		
		case EMERGENCY_AT_FLOOR:
			timer_start();
			STATE = DOORS_OPEN;
            
            printf("stop_button_released does\n");
            printf("From EMERGENCY_AT_FLOOR             to DOORS_OPEN\n");
			break;

		case DOORS_OPEN:
			break;

		default:
			break;
	}
}

void 
fsm_timer_is_out(void)
{


	switch (STATE) 
	{

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			break;

		case EMERGENCY_BETWEEN_FLOORS: 
			break;
		
		case EMERGENCY_AT_FLOOR:
			break;

		case DOORS_OPEN:
			timer_stop();
			elev_set_door_open_lamp(0);
			STATE = ELEVATOR_STOPPED;
            
            printf("timer_is_out does\n");
            printf("From DOORS_OPEN                 to ELEVATOR_STOPPED\n");
			break;

		default:
			break;
	}

}

void 
fsm_queue_not_empty(void)
{

	switch (STATE) 
	{

		case ELEVATOR_MOVES:
			break;

		case ELEVATOR_STOPPED:
			if (queue_floor_is_ordered() && (elev_get_floor_sensor_signal() != -1))
			{
				timer_start();
				elev_set_door_open_lamp(1);
				queue_clear_floor();
				STATE = DOORS_OPEN;
                
                printf("queue_not_empty does\n");
                printf("From ELEVATOR_STOPPED            to DOORS_OPEN\n");
			}
			else
			{
				elev_set_motor_direction(queue_get_direction());
				STATE = ELEVATOR_MOVES;
                
                printf("queue_not_empty does\n");
                printf("From ELEVATOR_STOPPED            to ELEVATOR_MOVES\n");
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

void 
fsm_arrive_at_floor(void)
{
	queue_update_floor();
	switch (STATE) 
	{

		case ELEVATOR_MOVES:
			if (queue_stop_here())
			{
				elev_set_motor_direction(0);
				STATE = ELEVATOR_STOPPED;
                
                printf("arrive_at_floor does\n");
                printf("From ELEVATOR_MOVES             to ELEVATOR_STOPPED\n");
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



//FSM-utility function

int 
fsm_check_for_orders(void)
{
	int order_is_made = 0;
	for (int button = 0; button < N_BUTTONS; button++)
	{
		for (int floor = 0; floor < N_FLOORS; floor++)
		{
			if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) )
			{
				if (elev_get_button_signal(button, floor))
				{
					order_is_made = 1;
				}
			}
		}
	}
	return order_is_made;
}
