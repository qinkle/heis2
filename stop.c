#include "elev.h"
#include "queue.h"
#include "timer.h"
#include "FSM.h"
#include <stdio.h>


static int stop_button = 0;




int stop_button_pressed(void) {
	if ( (stop_button != elev_get_stop_signal()) && (stop_button == 0) ){
		stop_button = elev_get_stop_signal();
		return 1;
	}
	return 0;
}

int stop_button_released(void) {
	if ( (stop_button != elev_get_stop_signal()) && (stop_button == 1) ){
		stop_button = elev_get_stop_signal();
		return 1;
	}
	return 0;
}