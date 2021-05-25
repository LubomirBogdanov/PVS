
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include "firstmod_device.h"

/*
#define RCC_AHB1ENR_GPIOA_EN_MASK			0x01
#define GPIOA_MODER_PA12_MASK				0x1000000
#define GPIOA_OTYPER_PA12_MASK 				0x1000
#define GPIOA_ODR_PA12_MASK				0x1000

volatile uint32_t *ahb1enr = (volatile uint32_t *)0x40023830;
volatile uint32_t *gpio_a_moder = (volatile uint32_t *)0x40020000;
volatile uint32_t *gpio_a_otyper = (volatile uint32_t *)0x40020004;
volatile uint32_t *gpio_a_odr = (volatile uint32_t *)0x40020014;

void led_init(void){
	*ahb1enr |= RCC_AHB1ENR_GPIOA_EN_MASK;
	*gpio_a_moder |= GPIOA_MODER_PA12_MASK; //PA12 - output
	*gpio_a_otyper &= ~GPIOA_OTYPER_PA12_MASK; //PA12 - push-pull
	*gpio_a_odr |= GPIOA_ODR_PA12_MASK;
}

void led_set(void){
	*gpio_a_odr |= GPIOA_ODR_PA12_MASK;
}

void led_clear(void){
	*gpio_a_odr &= ~GPIOA_ODR_PA12_MASK;
}
*/

#define RCC_AHB1ENR_GPIOA_EN_MASK	0x01
#define GPIOA_MODER_PA12_MASK		0x1000000
#define GPIOA_OTYPER_PA12_MASK		0x1000
#define GPIOA_ODR_PA12_MASK		0x1000

#define RCC_AHB1ENR_OFFSET		0x30
#define GPIOA_MODER_OFFSET		0x00
#define GPIOA_TYPER_OFFSET		0x04
#define GPIOA_ODR_OFFSET		0x14

struct file_operations firstmod_fops;
static dev_t major; // Global variable for the first device number 
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

void __iomem *rcc_base_virtual;
void __iomem *gpioa_base_virtual;

static int firstmod_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "firstmod_open\n");
	return 0;
}

static int firstmod_close(struct inode *i, struct file *f){
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static ssize_t firstmod_read(struct file *file, char __user *buf, size_t count, loff_t *ptr){
	printk(KERN_INFO "firstmod_read\n");

	return count;
}

static ssize_t firstmod_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos){
	uint32_t *reg_pointer;
	bool led_state;

	printk(KERN_INFO "firstmod_write %s\n", buf);

	kstrtobool(buf, &led_state); // == atoi

	reg_pointer = (gpioa_base_virtual + GPIOA_ODR_OFFSET);
	
	if(led_state){
		printk(KERN_INFO "Turning LED on ...\n");
		*reg_pointer |= GPIOA_ODR_PA12_MASK;
	}
	else{
		printk(KERN_INFO "Turning LED off ...\n");
		*reg_pointer &= ~GPIOA_ODR_PA12_MASK;	
	}

	return count;
}

static long firstmod_ioctl(struct file *infile, unsigned int cmd, unsigned long arg){
	printk("firstmod_ioctl: cmd=%d, arg=%ld\n", cmd, arg);

	return 0;
}

static int firstmod_probe(struct platform_device *pdev){
	uint32_t *reg_pointer;	
	struct resource *rcc_module, *gpioa_module;

        rcc_module = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if (unlikely(!rcc_module)) {
                pr_err(" Specified Resource Not Available... 0\n");
                return -1;
        }

        printk(KERN_INFO "Physical addresses range - RCC\n");
        printk(KERN_INFO "%x - %x : %d\n", (uint32_t)rcc_module->start, (uint32_t)rcc_module->end, resource_size(rcc_module));

	//Converting the physical addresses to virtual for using in driver 
        rcc_base_virtual = ioremap_nocache(rcc_module->start, resource_size(rcc_module));
        if (unlikely(!rcc_base_virtual)) {
                printk(KERN_ALERT "(cannot map IO)\n");
                return -1;
        }
	printk(KERN_INFO "rcc_base_virtual = %p\n", rcc_base_virtual);


	gpioa_module = platform_get_resource(pdev, IORESOURCE_MEM, 1);
        if (unlikely(!gpioa_module)) {
                pr_err("Specified Resource Not Available... 1\n");
                return -1;
        }

        printk(KERN_INFO "Physical addresses range - GPIOA\n");
        printk(KERN_INFO "%x - %x : %d\n", (uint32_t)gpioa_module->start, (uint32_t)gpioa_module->end, resource_size(gpioa_module));

	 
	//Converting the physical Address to virtual for using in driver 
        gpioa_base_virtual = ioremap_nocache(gpioa_module->start, resource_size(gpioa_module));
        if (unlikely(!gpioa_base_virtual)) {
                printk(KERN_ALERT "(cannot map IO)\n");
                return -1;
        }
	printk(KERN_INFO "gpioa_base_virtual = %p\n", gpioa_base_virtual);

	printk(KERN_INFO "IRQ number of device :%d\n", platform_get_irq(pdev, 0));


	reg_pointer = (rcc_base_virtual + RCC_AHB1ENR_OFFSET);
	*reg_pointer |= RCC_AHB1ENR_GPIOA_EN_MASK;

	reg_pointer = (gpioa_base_virtual + GPIOA_MODER_OFFSET);
	*reg_pointer |= GPIOA_MODER_PA12_MASK;

	reg_pointer = (gpioa_base_virtual + GPIOA_TYPER_OFFSET);
	*reg_pointer &= ~GPIOA_OTYPER_PA12_MASK;

	reg_pointer = (gpioa_base_virtual + GPIOA_ODR_OFFSET);
	*reg_pointer |= GPIOA_ODR_PA12_MASK;

//Safer way to do it:
//	reg_value = ioread32(rcc_base_virtual + RCC_AHB1ENR_OFFSET);
//	reg_value |= RCC_AHB1ENR_GPIOA_EN_MASK;
//	iowrite32(reg_value, (rcc_base_virtual + RCC_AHB1ENR_OFFSET));

//	reg_value = ioread32(gpioa_base_virtual + GPIOA_MODER_OFFSET);
//	reg_value |= GPIOA_MODER_PA12_MASK; //PA12 - output
//	iowrite32(reg_value, (gpioa_base_virtual + GPIOA_MODER_OFFSET));

//	reg_value = ioread32(gpioa_base_virtual + GPIOA_TYPER_OFFSET);
//	reg_value &= ~GPIOA_OTYPER_PA12_MASK; //PA12 - push-pull
//	iowrite32(reg_value, (gpioa_base_virtual + GPIOA_TYPER_OFFSET));

//	reg_value = ioread32(gpioa_base_virtual + GPIOA_ODR_OFFSET);
//	reg_value |= GPIOA_ODR_PA12_MASK;
//	iowrite32(reg_value, (gpioa_base_virtual + GPIOA_ODR_OFFSET));

 	return 0;  
}

static int firstmod_remove(struct platform_device *pdev){
	iounmap(gpioa_base_virtual);
	iounmap(rcc_base_virtual);
	return 0;
}

struct file_operations firstmod_fops = {
	.owner = THIS_MODULE,
	.read = firstmod_read,
	.write = firstmod_write,
	.open = firstmod_open,
	.release = firstmod_close,
	.unlocked_ioctl = firstmod_ioctl
};

static struct platform_driver firstmod_driver = {
	.probe          = firstmod_probe,
	.remove         = firstmod_remove,
	.driver = {
			.name  = FIRSTMOD_NAME,
	},
};

static int __init firstmod_init(void){
        printk(KERN_INFO "Platform driver init\n");
	platform_driver_register(&firstmod_driver);

	if (alloc_chrdev_region(&major, 0, 1, "firstmod_dev") < 0){
		printk("Error registering char device numbers!\n");
    		return -1;
  	}

    	if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL){
		printk("Error creating device class!\n");
    		unregister_chrdev_region(major, 1);
    		return -1;
  	}

    	if (device_create(cl, NULL, major, NULL, "firstmod") == NULL){
		printk("Error creating device!\n");
		class_destroy(cl);
    		unregister_chrdev_region(major, 1);
    		return -1;
  	}

    	cdev_init(&c_dev, &firstmod_fops);
    	
	if (cdev_add(&c_dev, major, 1) == -1){
		printk("Error adding a char device to the system!\n");
    		device_destroy(cl, major);
    		class_destroy(cl);
    		unregister_chrdev_region(major, 1);
    		return -1;
  	}

	printk("Device file registered successfully in /dev!\n");

        return 0;
}

static void __exit firstmod_exit(void){
        printk("Platform driver exit\n");

	cdev_del(&c_dev);
	device_destroy(cl, major);
	class_destroy(cl);
	unregister_chrdev_region(major, 1);

	platform_driver_unregister(&firstmod_driver);
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_AUTHOR("Lubomir Bogdanov, TU-Sofia");
MODULE_DESCRIPTION("Hello World Example (driver)");
MODULE_LICENSE("GPL");
