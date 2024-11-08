#include <linux/module.h>
#include <linux/init.h>

int my_init(void){
	printk("Hello kernel!\n");
	return 0;
}

void my_exit(void){
	printk("Goodbye kernel!\n"); // print to kernel log, as there is no stdout
}

module_init(my_init); // add function as initialization callback
module_exit(my_exit); // add function as exit callback

MODULE_LICENSE("GPL"); // GPL is a free open source license. Unix/Linux will reject any proprietary licenses
