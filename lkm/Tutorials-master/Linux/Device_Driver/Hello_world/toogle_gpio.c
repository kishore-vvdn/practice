#include <linux/module.h>	 /* Needed by all modules */ 
#include <linux/kernel.h>	 /* Needed for KERN_INFO */ 
#include <linux/init.h>	 /* Needed for the macros */ 
#include <linux/gpio.h>

MODULE_LICENSE("GPL"); 

MODULE_AUTHOR("vvdn"); 

MODULE_DESCRIPTION("A simple toggle LKM!"); 
 
MODULE_VERSION("0.1"); 

#define TOGGLE 65
static int value = 0;

static int __init toggle_start(void) 
{ 
	if(gpio_is_valid(TOGGLE) < 0) return -1;
	if(gpio_request(TOGGLE, "TOGGLE") < 0) return -1;
	gpio_direction_output(TOGGLE, 0 );
	value=gpio_get_value(TOGGLE);
	value = value ? (0):(1);
        gpio_set_value(TOGGLE, value);
	return 0; 
} 

static void __exit toggle_end(void) 
{ 
	gpio_set_value(TOGGLE, 0);
	gpio_free(TOGGLE);
	printk(KERN_INFO "Goodbye\n"); 
} 

module_init(toggle_start); 
module_exit(toggle_end); 

