#include <linux/module.h>
#include <linux/fs.h> // include linux filesystem
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yang");
MODULE_DESCRIPTION("Registers a device number, and implement some callback functions");

/**
 * @brief This function is called when the device file is opened
 */

static int driver_open(struct inode *device_file, struct file *instance) {
	printk("driver_open called\n");
	return 0;
}	

/**
 * @brief This function is called whne the device file is closed
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("driver_closed called\n");
	return 0;
}	


/* Register device to perform open and release callback functions */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

# define MAJOR_DEVICE_NUMBER 30

/**
 * @brief This function is called when module is loaded into the kernel
 */
int my_init(void){ 
        int return_val;	
	printk("Hello kernel!\n");  // print to kernel log, as there is no stdout
	/* Register device number */ 

	return_val = register_chrdev(MAJOR_DEVICE_NUMBER, "my_device_number", &fops);

	if(return_val == 0){
		printk("Device number successfully registered: Major: %d, Minor: %d"
				, MAJOR_DEVICE_NUMBER, 0);
	}else if(return_val > 0){
		printk("Device number successfully registered: Major: %d, Minor: %d", 
				return_val>>20, return_val&0xfffff);
	}else{
		printk("Could not register device number\n");
		return -1;
	}
		
	return 0;
}

/**
 * @brief This function is called when module is removed from the kernel
 */
void my_exit(void){ 
	printk("Goodbye kernel!\n"); // print to kernel log, as there is no stdout
	unregister_chrdev(MAJOR_DEVICE_NUMBER, "my_device_number");
}

module_init(my_init); // add function as initialization callback
module_exit(my_exit); // add function as exit callback

