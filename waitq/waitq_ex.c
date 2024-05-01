#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
//#include<linux/fs.h>
#include<linux/wait.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>
#include<linux/kthread.h>


MODULE_AUTHOR("Kathir");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(" sample module for waitq ");


/*IN rpi old gpio pin numbers are not used to indentify the gpio pins. Instead the global gpio identifiers are used 
 * Refer cat /sys/kernel/debug/gpio --> get the pin number from pinctrl and search for the pin in this ssyfs and get the global gpio identifier */
#define LED_GPIO_PIN 403
#define IRQ_GPIO_PIN 411




DECLARE_WAIT_QUEUE_HEAD(intrWq);

static struct myExampleStrcut {
	unsigned int irq;
	struct task_struct *myTask;
}myS;

static int flag=0;
static int threadFun(void *arg) {
	while(!kthread_should_stop()) {
	 	printk("In thread fun\n");
		wait_event_interruptible(intrWq,(flag != 0));	
	 	printk("thread released \n");
		flag=0;

	}
	//do_exit(0);
	return 0;
}

static irqreturn_t my_irq_handler (int irq,void *pvtdata) {
	printk("IRQ raised\n");
	flag = 1;
	wake_up_interruptible(&intrWq);
	return IRQ_HANDLED;
}
static int __init waitq_ex_init(void){

	if(!gpio_is_valid(IRQ_GPIO_PIN)) {
		printk("Error in validating IRQ_GPIO_PIN=%d gpio\n",IRQ_GPIO_PIN);
		return -1;
	}
	if(gpio_request(IRQ_GPIO_PIN,"katWaitQ") < 0) {
		printk("Error in requesting gpio\n");
		return -1;
	}

	if(gpio_direction_input(IRQ_GPIO_PIN)) {
		printk("Error in requesting gpio\n");
		goto ret_gpio_req;
	}

	myS.irq =  gpio_to_irq(IRQ_GPIO_PIN);

	if(request_irq(myS.irq,my_irq_handler, IRQF_TRIGGER_RISING,"katWqIrq",NULL)) {
		printk("Error requesting irq\n");
		goto ret_irq_req;

	}
	if((myS.myTask = kthread_create(threadFun,NULL,"myThread")) == NULL) {
		printk("Error in creating the threead\n");
		goto ret_thread;
	}
	wake_up_process(myS.myTask);
	return 0;
ret_thread:
	free_irq(myS.irq,NULL);
ret_irq_req:
ret_gpio_req:
	gpio_free(IRQ_GPIO_PIN);
	return -1;

}


static void __exit waitq_ex_exit(void) {
	flag = 1;
	wake_up_interruptible(&intrWq);
	kthread_stop(myS.myTask);
	gpio_free(IRQ_GPIO_PIN);
	free_irq(myS.irq,NULL);
}


module_init(waitq_ex_init);
module_exit(waitq_ex_exit);



