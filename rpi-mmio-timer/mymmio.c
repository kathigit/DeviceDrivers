#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/io.h>
#include<linux/delay.h>

MODULE_AUTHOR("Kathiravan Eswaran");
MODULE_DESCRIPTION("Sample module to access rpi5 timer mmio");
MODULE_LICENSE("GPL");

#define TIMER_PHY_BASE   0x000000107C003000
#define TIMER_PHY_SIZE   0x20
#define TIMER_REG_OFFSET 0x4

static volatile void __iomem * pointer = NULL;

static int __init mmio_init(void) {
    printk("MMIO INIT\n");

    /* Ioremap the phy addr */
    pointer = ioremap(TIMER_PHY_BASE, TIMER_PHY_SIZE);
    if(!pointer) {
        return -ENOMEM;
    }

    writel(3770660419, pointer+TIMER_REG_OFFSET);

    // printk("Data read at point 1 is %u\n", readl(pointer+TIMER_REG_OFFSET));
    // udelay(1000);
    // printk("Data read at point 2 is %u\n", readl(pointer+TIMER_REG_OFFSET));

    return 0;
}

static void __exit mmio_exit(void) {
    if(pointer) {
        iounmap(pointer);
        pointer=NULL;
    }
    printk("MMIO EXIT\n");

}

module_init(mmio_init);
module_exit(mmio_exit);