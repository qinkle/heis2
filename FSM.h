

/*
Initializes the fsm after start-up.
*/
void fsm_init(void);

/*
State behaviour when an order is placed
*/
void fsm_order_placed();


/*
Polls all order buttons, returns 1 if one is pressed
*/
void fsm_check_for_orders(void);