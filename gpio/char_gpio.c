#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>

MODULE_AUTHOR("KATHIRAVAN ESWARAN");
MODULE_LICENSE("GPL");


#define MY_MINOR_NUM 1
#define NUM_OF_DEVICES 1

/*IN rpi old gpio pin numbers are not used to indentify the gpio pins. Instead the global gpio identifiers are used 
 * Refer cat /sys/kernel/debug/gpio --> get the pin number from pinctrl and search for the pin in this ssyfs and get the global gpio identifier */
#define LED_GPIO_PIN 403
#define IRQ_GPIO_PIN 411

static unsigned int myIrq;
static dev_t my_dev_t;
static struct class *my_class;
static struct device *my_device;
static ssize_t my_read(struct file *filep,char __user *buf, size_t len, loff_t *offset);
static ssize_t my_write(struct file *filp,const char __user *buf, size_t len,loff_t *offset) ;
static int my_release(struct inode *file_inode, struct file *filep) ;
static int my_open(struct inode *file_inode,struct file *filep) ;
static long my_ioctl(struct file* filep, unsigned int cmd, unsigned long addresss);
static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read  = my_read,
    .write = my_write,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};
static struct cdev my_cdev;
static irqreturn_t gpio_irq_handler(int irq,void *dev_id){
    static unsigned long flags = 0;
    static int led_toggle = 0;
    local_irq_save(flags);
    printk("IRQ arrived\n");

    led_toggle = 1 ^ led_toggle ;
    gpio_set_value(LED_GPIO_PIN,led_toggle);
    local_irq_restore(flags);
    return IRQ_HANDLED;
}

static ssize_t my_read(struct file *filep,char __user *buf, size_t len, loff_t *offset) {
    char ker_buf[100] = "Message from linux kernel\n";
    printk("%s called\n",__func__);
    if(copy_to_user(buf,ker_buf,strlen(ker_buf))) {
        return -EFAULT;
    }
    return strlen(ker_buf);
}

static ssize_t my_write(struct file *filp,const char __user *buf, size_t len,loff_t *offset) {
    printk("%s called\n",__func__);
    return len;
}

static int my_open(struct inode *file_inode,struct file *filep) {
    printk("%s called\n",__func__);
    return 0;
}

static int my_release(struct inode *file_inode, struct file *filep) {
    printk("%s called\n",__func__);
    return 0;
}
static char *my_devnode(struct device *dev , umode_t *mode) {
    if(mode) {
        *mode = 0666;
    }
    return NULL;
}

static long my_ioctl(struct file* filep, unsigned int cmd, unsigned long address) {
    char localbuff[100];
    int on_off;
    printk("%s called\n",__func__);
         	if(copy_from_user(localbuff,(unsigned long *)address,100)) {
        		return -EFAULT;
        	}
    switch(cmd) {
        case 'A': printk("case A \n"); 
        		printk("Received string=%s\n",localbuff);
              break;
/*        case 'B': printk("case B \n"); BUG_ON("Received B\n"); break;*/
    case 'C':
    	          sscanf(localbuff,"%d",&on_off);
    		  printk("Set %d to GPIO 12 \n",on_off);
    		  gpio_set_value(LED_GPIO_PIN,on_off);
    		  break;
    }
    return 0;
}
static int __init char_drv_init(void) {
    int err = 0;

    /*dynamically allocating dev_t by the kernel */
    if( (err = alloc_chrdev_region(&my_dev_t,MY_MINOR_NUM,NUM_OF_DEVICES,"testD")) ) {
        printk("Error in allocating char device \n");
        return err;
    }
        printk("allocated major number=%d\n",MAJOR(my_dev_t));
    /*create cdev structure*/
    cdev_init(&my_cdev,&my_fops);

    /*Add cdev to the kernel*/
    if(cdev_add(&my_cdev,my_dev_t,1) < 0 ) {
        pr_err("cdev_add returned error\n");
        goto ret_class_err;
    }

    /*creating class for testD to create a device file*/
    my_class = class_create(THIS_MODULE,"testD");
    if(IS_ERR(my_class)) {
        printk("class_create returned error\n");
        goto ret_class_err;
    }

    /*change the device file permissions in my_devnode. Gets called when device_create */
    my_class->devnode = my_devnode;
    /*creatig device file using the struct class * */
    my_device = device_create(my_class,NULL,my_dev_t,NULL,"testD");
    if(IS_ERR(my_device)) {

        printk("device_create returned error\n");
        goto ret_device_err;
    }

    /*GPIO initialization */
    if(gpio_is_valid(LED_GPIO_PIN) == false) {
        printk("Error validating GPIO12\n");
        goto ret_gpio_err;
    }

    /*gpio request */
    if((err = gpio_request(LED_GPIO_PIN,"kathirGPIO")) <0 ) {
        printk("Error requesting GPIO , error =%d\n",err);
        goto ret_gpio_err;
    }
    /*set LED gpio direction to output and the intial value to 0 */
    gpio_direction_output(LED_GPIO_PIN,0);

    /* GPIO validation for interrupt gpio */
    if(gpio_is_valid(IRQ_GPIO_PIN) == false) {
        printk("Error validating GPIO12\n");
        goto ret_gpio_irq;
    }

    /* request the gpio */
    if( (err = gpio_request(IRQ_GPIO_PIN,"kathirIRQ")) < 0) {
        printk("Error requesting GPIO=%d , error =%d\n",IRQ_GPIO_PIN,err);
        goto ret_gpio_irq;
    }

    /* set the direction to input for detecting the interrupts */
    gpio_direction_input(IRQ_GPIO_PIN);

    /*get the gpio irq */
    myIrq = gpio_to_irq(IRQ_GPIO_PIN);

    /*Registering interrupt handler*/
    if( (err = request_irq(myIrq, gpio_irq_handler,IRQF_TRIGGER_RISING,"testD",NULL))) {
        printk("Reuqesting IRQ failed for irq=%d\n",myIrq);
        goto ret_irq_handler;
    }

    return 0;


ret_irq_handler:
    gpio_free(IRQ_GPIO_PIN);
ret_gpio_irq:
    gpio_free(LED_GPIO_PIN);
ret_gpio_err:
    device_destroy(my_class,my_dev_t);
ret_device_err:
    class_destroy(my_class);
ret_class_err:
    unregister_chrdev_region(my_dev_t,NUM_OF_DEVICES);
    return -1;

}

static void __exit char_drv_exit(void) {
    free_irq(myIrq,NULL);
    gpio_free(IRQ_GPIO_PIN);
    gpio_free(LED_GPIO_PIN);
    device_destroy(my_class,my_dev_t);
    class_destroy(my_class);
    unregister_chrdev_region(my_dev_t,NUM_OF_DEVICES);
    printk("module exit\n");
}

module_init(char_drv_init);
module_exit(char_drv_exit);
