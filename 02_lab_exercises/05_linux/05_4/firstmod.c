#include <linux/module.h>

static int __init firstmod_init(void){
        printk(KERN_INFO "Hello Example Init\n");
        return 0;
}

static void __exit firstmod_exit(void){
        printk("Hello Example Exit\n");
}

module_init(firstmod_init);
module_exit(firstmod_exit);

MODULE_AUTHOR("Lubomir Bogdanov, TU-Sofia");
MODULE_DESCRIPTION("Hello World Example");
MODULE_LICENSE("GPL");
