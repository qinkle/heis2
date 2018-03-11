// queue.c
// These functions provide utilities for managing orders


#include "FSM.h"
#include "queue.h"
#include "elev.h"
#include "timer.h"
#include <stdlib.h>
#include <stdio.h>




/**
	order_matrix has the following structure
	In the example, a person has pressed the button 
	to go to the third floor and someone wants to
	go up from the first floor
							buttons
						up	down	called
			first		1	0		0
			second		0	0		0
	floors	third		0	0		1
			fourth		0	0		0
*/
static int order_matrix[N_BUTTONS][N_FLOORS];
static int direction = 1;	// is 1 if the elevator is moving upwards, and -1 if the elevetor is moving downwards into the depths of despair
static int current_floor = -1;

void 
queue_init(void)
{
	queue_clear_all_orders();
	queue_update_floor();
}

void 
queue_clear_order(int button, int floor)
{
	order_matrix[button][floor] = 0;
	if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) )
	{
		elev_set_button_lamp(button, floor, 0);
	}
}

void 
queue_place_order(int button, int floor)
{
	if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) )
	{ 
		order_matrix[button][floor] = 1;
		elev_set_button_lamp(button, floor, 1);
	}
}

void 
queue_clear_all_orders(void)
{
	for (int button = 0; button < N_BUTTONS; button++ )
	{
		for (int floor = 0; floor < N_FLOORS; floor++)
		{
			queue_clear_order( button, floor);
		}
	}
}

void 
queue_order_made(void)
{
	for (int button = 0; button < N_BUTTONS; button++)
	{
		for (int floor = 0; floor < N_FLOORS; floor++)
		{
			if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) )
			{ // A check to make sure the elev-function isn't asked to find button signals from non-existant buttons
				if (elev_get_button_signal(button, floor))
				{
					queue_place_order(button, floor);
				}
			}
		}
	}
}


void 
queue_update_floor(void)
{ // Reads the floor sensor and updates the current floor value
	int current_floor_sensor = elev_get_floor_sensor_signal();

	if ( (elev_get_floor_sensor_signal() != -1) && (elev_get_floor_sensor_signal()!= current_floor) )
	{
		current_floor = current_floor_sensor;
		elev_set_floor_indicator(current_floor);
	}
}

int 
queue_is_empty(void)
{ // Iterates though every floor, checks if there are any calls in the queue matrix
	for (int button = 0; button < N_BUTTONS; button++)
	{
		for (int floor = 0; floor < N_FLOORS; floor++)
		{
			if (order_matrix[button][floor])
			{
				return 0;
			}
		}
	}
	return 1;
}

int 
queue_get_direction(void)
{

	if (queue_is_empty())
	{ // If the queue is empty, don't move the elevator
		return 0;
	}

	if (queue_is_last_stop())
	{ // If it's the last stop, turn around
		direction = -direction;
	}

	return direction;
}

int 
queue_is_last_stop(void)
{

	if ( ((current_floor == 0) && (direction == -1)) || ((direction == 1) && (current_floor == N_FLOORS -1)) )
	{ // Checks if the elevator is going up in the top floor or the elevator is going down in the ground floor
		return 1;
	}
	for (int floor = current_floor + direction; ((floor < N_FLOORS) && (floor >= 0)); floor = floor + direction)
	{ // Iterates through the floors in the direction the elevator is moving
		for (int button = 0; button < N_BUTTONS; button++)
		{
			if (order_matrix[button][floor])
			{
				return 0;
			}
	
		}
	}

	return 1;
}

int 
queue_stop_here(void)
{
	
	if (queue_is_last_stop()) // The elevator should stop if there are no other stops after this floor
	{
		return 1;
	}

	if (direction == 1) // The elevator is going up, checks if there is anyone going up from or to the floor
	{
		if (order_matrix[0][current_floor] || order_matrix[2][current_floor])
		{
			return 1;
		}
	}

	if(direction == -1) // The elevator is going down, checks if there is anyone going down from or to the floor
	{
		for (int button = 1; button < N_BUTTONS; button++)
		{
			if (order_matrix[button][current_floor])
			{
				return 1;
			}
		}
	}

	return 0;
}

int 
queue_floor_is_ordered(void)
{

	for (int button = 0; button < N_BUTTONS; button++)
	{ // Iterates through the queue orders on current floor, returns 1 if any one of them is ordered
		if (order_matrix[button][current_floor])
		{
			return 1;
		}
	}

	return 0;
}

void 
queue_clear_floor(void)
{
	for (int button = 0; button < N_BUTTONS; button++)
	{ // Clears every button associated with the current floor
		queue_clear_order(button, current_floor);
	}
}
