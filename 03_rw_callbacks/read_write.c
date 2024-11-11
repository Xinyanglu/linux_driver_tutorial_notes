#include <linux/module.h>
#include <linux/fs.h> 
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("yang");
MODULE_DESCRIPTION("Registers a device number, and implement some callback functions");

/* Buffer for data */
static char buffer[255];
static int buffer_pointer = 0;

/* Variables for device */
static dev_t my_device_number;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "moduleclass"

/**
 * @brief Read data out of buffer
 */

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offset){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = umin(count, buffer_pointer);

	/*Copy data to user */
	not_copied = copy_to_user(user_buffer, buffer, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

/**
 * @brief Write data into buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offset){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = umin(count, sizeof(buffer));

	/*Copy data to user */
	not_copied = copy_from_user(buffer, user_buffer, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;
	buffer_pointer += delta;
	return delta;
}

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
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

/**
 * @brief This function is called when module is loaded into the kernel
 */
int my_init(void){ 
	printk("Hello kernel\n");

	/* Allocate device number */
	if(alloc_chrdev_region(&my_device_number, 0, 1, DRIVER_NAME) < 0) {
		printk("Device number couldn't be allocated\n");
		return -1;
	}

	printk("Device number major: %d, minor: %d registered\n", my_device_number >> 20, my_device_number & 0xfffff);

	/* Create device class */
	if ((my_class = class_create(DRIVER_CLASS)) == NULL) {
		printk("Device class creation failed\n");
		goto ClassError;
	}

	/*create device file*/
	if (device_create(my_class, NULL, my_device_number, NULL, DRIVER_NAME) == NULL) {
		printk("Device file creation failed\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Registering device into kernel */
	if(cdev_add(&my_device, my_device_number, 1) == -1) {
		printk("Registering device to kernel failed\n");
		goto AddError;
	}
	return 0;


AddError:
	device_destroy(my_class, my_device_number);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_number, 1);
	return -1;
}

/**
 * @brief This function is called when module is removed from the kernel
 */
void my_exit(void){ 
	printk("Goodbye kernel!\n"); // print to kernel log, as there is no stdout
	cdev_del(&my_device);
	device_destroy(my_class, my_device_number);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_number, 1);
}

module_init(my_init); // add function as initialization callback
module_exit(my_exit); // add function as exit callback

