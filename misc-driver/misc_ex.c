#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>

static int sample_open(struct inode *in, struct file *file_ptr) {
    printk("Opened the file\n");
    return 0;
}

static int sample_close(struct inode *in,struct file *file_ptr){
    printk("close the file\n");
    return 0;
}

static struct file_operations sample_fops = {
    .owner = THIS_MODULE,
    .open  = sample_open,
    .release  = sample_close,
};

static struct miscdevice sample_misc = {
    .name = "kathir_misc",
    .minor = MISC_DYNAMIC_MINOR,
    .fops = &sample_fops,
};




static int __init mstart(void) {
    printk("Entering init\n");
    if(misc_register(&sample_misc)) {
        printk("Error in registering misc\n");
    }
    return 0;
}


static void  __exit mend(void) {
    printk("Exiting the module\n");
    misc_deregister(&sample_misc);
}


module_init(mstart);
module_exit(mend);
MODULE_LICENSE("GPL");