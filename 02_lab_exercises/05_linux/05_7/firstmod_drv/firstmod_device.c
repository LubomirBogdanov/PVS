#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include "firstmod_device.h"

static struct resource device_resources[] = {
        {
                .start          = RCC_START_ADDRESS,
                .end            = RCC_END_ADDRESS,
                .flags          = IORESOURCE_MEM,
        },

        {
                .start          = GPIOA_START_ADDRESS,
                .end            = GPIOA_END_ADDRESS,
                .flags          = IORESOURCE_MEM,
        },

	{
                .start          = FIRSTMOD_DEV_IRQNUM,
                .end            = FIRSTMOD_DEV_IRQNUM,
                .flags          = IORESOURCE_IRQ,
        }

};

static void firstmod_release(struct device *dev){
	printk(KERN_INFO "Device release\n");
}

static struct platform_device firstmod_device = {
        .name           = FIRSTMOD_NAME,
        .id             = -1,
        .num_resources  = ARRAY_SIZE(device_resources),
        .resource       = device_resources,
	.dev = {
		.release = firstmod_release
	}
};

static int __init firstmod_init(void){
        printk(KERN_INFO "Platform device init\n");
	platform_device_register(&firstmod_device);
        return 0;
}

static void __exit firstmod_exit(void){
        printk("Platform device exit\n");
	platform_device_unregister(&firstmod_device);
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_AUTHOR("Lubomir Bogdanov, TU-Sofia");
MODULE_DESCRIPTION("Hello World Example (device)");
MODULE_LICENSE("GPL");
