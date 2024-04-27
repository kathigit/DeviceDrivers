#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xfe990052, "gpio_free" },
	{ 0xedadcbe4, "device_destroy" },
	{ 0x48be46d6, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x4829a47e, "memcpy" },
	{ 0xdcb764ad, "memset" },
	{ 0x98cf60b3, "strlen" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x5af132a, "gpio_to_desc" },
	{ 0x1d4981c, "gpiod_set_raw_value" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x4f2b3590, "cdev_init" },
	{ 0x12567b45, "cdev_add" },
	{ 0x23d20fbd, "__class_create" },
	{ 0x255a40b1, "device_create" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x7960b459, "gpiod_direction_output_raw" },
	{ 0xa207adb, "gpiod_direction_input" },
	{ 0x4c688e22, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xec3d2e1b, "trace_hardirqs_off" },
	{ 0xd697e69a, "trace_hardirqs_on" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xf7038a43, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "9FAC42684364D7B72953BD0");
