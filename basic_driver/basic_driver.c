#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>


static int __init mstart(void) {
    printk("Hello world\n");
    return 0;
}


static void  __exit mend(void) {
    printk("Exiting the module\n");
    return;
}


module_init(mstart);
module_exit(mend);
MODULE_LICENSE("GPL");