

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
void fsm_stop_button_pressed();


/*
Polls all order buttons, returns 1 if one is pressed
*/
int fsm_check_for_orders(void);