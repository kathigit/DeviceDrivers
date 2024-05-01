#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/timer.h>
#include<linux/jiffies.h>
#include<linux/moduleparam.h>


typedef struct  mystruct {
    struct timer_list timer;
}tim_t;

static tim_t timer_obj;
static unsigned int timeout = 5; //default 5 seconds

module_param(timeout, int, 0);
MODULE_AUTHOR("KATHIRAVAN ESWARAN");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Timer example lkm, able to pass timeout from cmdline");

/******************************************************************/
static void timer_callback(struct timer_list *data){
    if(in_interrupt()){
        printk("Timer expired\n");
    }
    mod_timer(&timer_obj.timer, jiffies + msecs_to_jiffies(timeout*1000));
}



static int __init my_timer_init(void) {
    printk("Intialized the timer to %u seconds\n",timeout);
    timer_setup(&timer_obj.timer ,timer_callback,0);
    mod_timer(&timer_obj.timer, jiffies + msecs_to_jiffies(timeout*1000));
    return 0;
}

static void __exit my_timer_exit(void) {
    del_timer_sync(&timer_obj.timer);
    printk("Exiting timer example\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);