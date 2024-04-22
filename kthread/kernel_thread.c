#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/kthread.h>
#include<linux/delay.h>

MODULE_AUTHOR("KATHIRAVAN ESWARAN");
MODULE_LICENSE("GPL");


#define MY_MINOR_NUM 1
#define NUM_OF_DEVICES 1

static dev_t my_dev_t;
static struct class *my_class;
static struct device *my_device;
static ssize_t my_read(struct file *filep,char __user *buf, size_t len, loff_t *offset);
static ssize_t my_write(struct file *filp,const char __user *buf, size_t len,loff_t *offset) ;
static int my_release(struct inode *file_inode, struct file *filep) ;
static int my_open(struct inode *file_inode,struct file *filep) ;
static long my_ioctl(struct file* filep, unsigned int cmd, unsigned long addresss);
static void create_my_thread(void);
static struct cdev my_cdev;
static struct task_struct *my_threads;
static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read  = my_read,
    .write = my_write,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

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
    printk("%s called\n",__func__);
    switch(cmd) {
        case 'A': printk("case A \n"); 
    	 	if(copy_from_user(localbuff,(unsigned long *)address,100)) {
    			return -EFAULT;
    		}
    			printk("Received string=%s\n",localbuff);
    	      break;
        case 'B': printk("case B \n"); BUG_ON("Received B\n"); break;
    }
    return 0;
}

static int thread_func(void *arg) {
    while(!kthread_should_stop()) {
    	msleep(1000*5);
    	printk("Im the thread\n");
    }
    printk("exiting the thread\n");
    return 0;
}


static void create_my_thread(void) {
    my_threads = kthread_create(thread_func,NULL,"KathirThread");
    if(my_threads !=NULL) {
    	wake_up_process(my_threads);
    	printk("Kernel thred is created \n");
    } else {
    	printk("Kernel thred wasn't created \n");
    }
}
/************************************************************/
static int __init char_drv_init(void) {
    int err = 0;

    /*dynamically allocating dev_t by the kernel */
    if( (err = alloc_chrdev_region(&my_dev_t,MY_MINOR_NUM,NUM_OF_DEVICES,"threadD")) ) {
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

    /*creating class for threadD to create a device file*/
    my_class = class_create(THIS_MODULE,"threadD");
    if(IS_ERR(my_class)) {
        printk("class_create returned error\n");
        goto ret_class_err;
    }

    /*change the device file permissions in my_devnode. Gets called when device_create */
    my_class->devnode = my_devnode;
    /*creatig device file using the struct class * */
    my_device = device_create(my_class,NULL,my_dev_t,NULL,"threadD");
    if(IS_ERR(my_device)) {

        printk("device_create returned error\n");
        goto ret_device_err;
    }

    create_my_thread();
    return 0;

ret_device_err:
    class_destroy(my_class);
ret_class_err:
    unregister_chrdev_region(my_dev_t,NUM_OF_DEVICES);
    return -1;

}

static void __exit char_drv_exit(void) {
	/*Stop the thread*/
	if(my_threads) {
		kthread_stop(my_threads);
	}
    device_destroy(my_class,my_dev_t);
    class_destroy(my_class);
    unregister_chrdev_region(my_dev_t,NUM_OF_DEVICES);
    printk("module exit\n");
}

module_init(char_drv_init);
module_exit(char_drv_exit);
