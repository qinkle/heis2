#include "FSM.h"
#include "queue.h"
#include "elev.h"
//#include "timer.h"
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

void queue_init(){
	queue_clear_all_orders();
}

void queue_clear_order(int button, int floor){
	order_matrix[button][floor] = 0;
	if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) ){
		elev_set_button_lamp(button, floor, 0);
	}

}

void queue_place_order(int button, int floor){
	if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) ){
		order_matrix[button][floor] = 1;
		elev_set_button_lamp(button, floor, 1);
	}
}

void queue_clear_all_orders(){
	for (int button = 0; button < N_BUTTONS; button++ ){
		for (int floor = 0; floor < N_FLOORS; floor++){
			queue_clear_order( button, floor);
		}
	}
}

void queue_order_made(){
	for (int button = 0; button < N_BUTTONS; button++){
		for (int floor = 0; floor < N_FLOORS; floor++){
			if ( !( (floor == 0 && button == BUTTON_CALL_DOWN) ||  (floor == N_FLOORS -1 && button == BUTTON_CALL_UP) ) ){
				if (elev_get_button_signal(button, floor)){
					queue_place_order(button, floor);
				}
			}
		}
	}
}