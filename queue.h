
/*
Sets all lights to off and deletes all orders
*/
void queue_init(void);

/*
Clears the specified order and extinguishes the corrensponing lamp
*/
void queue_clear_order(int button, int floor);


/*
Places the specified order and lights the corrensponing lamp
*/
void queue_place_order(int button, int floor);


/*
Clears all orders and extinguishes all order lamps
*/
void queue_clear_all_orders(void);


/*
Checks for buttons pressed and places the orders that are being made
*/
void queue_order_made(void);


/*
Called by fsm whenever the elevator reaches a new floor, updates last floor
*/
void queue_update_floor(void);

/*
Checks if there are entries in the queue, returns 1 if there are none
*/
int queue_is_empty(void);

/*
Returns to the FSM the direction the elevator is supposed to move in, updates the queues direction value
*/
int queue_get_direction(void);

/*
Returns 1 if there are no stops beyond the elevator, returns 0 if there are
*/
int queue_is_last_stop(void);