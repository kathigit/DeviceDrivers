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
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x8da6585d, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0xe2964344, "__wake_up" },
	{ 0x45204f54, "kthread_stop" },
	{ 0xfe990052, "gpio_free" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x5af132a, "gpio_to_desc" },
	{ 0xa207adb, "gpiod_direction_input" },
	{ 0x4c688e22, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x8062072c, "kthread_create_on_node" },
	{ 0xd40e7960, "wake_up_process" },
	{ 0xf7038a43, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D4BE6A79FFAACFA41DAB018");
