#include "elev.h"
#include "queue.h"
#include "timer.h"


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