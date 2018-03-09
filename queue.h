
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