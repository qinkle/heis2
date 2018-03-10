

/*
Initializes the fsm after start-up.
*/
void fsm_init(void);

/*
State behaviour when an order is placed
*/
void fsm_order_placed(void);


/*
State behaviour when the stop button is pressed
*/
void fsm_stop_button_pressed(void);

/*
State behaviour when the stop button is released
*/
void fsm_stop_button_released(void);

/*
State behaviour when the timer runs out, 3 seconds
*/
void fsm_timer_is_out(void);

/*
State behaviour for unanswered orders
*/
void fsm_queue_not_empty(void);

/*
Polls all order buttons, returns 1 if one is pressed
*/
int fsm_check_for_orders(void);
