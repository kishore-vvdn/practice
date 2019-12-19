#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include<linux/ioctl.h>

#define W_VALUE _IOW('A','A',int32_t*)
#define R_VALUE _IOR('A','B',int32_t*)

#define mem_size        1024

dev_t dev =0;
static struct class *dev_class;
static struct cdev etx_cdev;


struct data
{
	 int i;
	 char c;
	 float fl;
	
}dat;

static int etx_open(struct inode *inode ,struct file *file);
static int etx_release(struct inode *inode ,struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len,loff_t * off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .release        = etx_release,
	.unlocked_ioctl = etx_ioctl,
};

static int etx_open(struct inode *inode ,struct file *file)
{
	printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int etx_release(struct inode *inode ,struct file *file)
{
        printk(KERN_INFO "Device File Released...!!!\n");
        return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off)
{
	//copy_to_user(buf, &dat, sizeof(dat));
        printk(KERN_INFO "Read Fuction called\n");
        return 0;
}

static ssize_t etx_write(struct file *filp,const char *buf, size_t len,loff_t * off)
{
	//copy_from_user(&dat, buf, sizeof(dat));
	//dat.i++;
        printk(KERN_INFO "write Fuction called\n");
        return len; 
}

static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case W_VALUE:
                        copy_from_user(&dat ,(int32_t*) arg, sizeof(dat));
                        dat.i++;
//			dat.fl=dat.fl+1;
			dat.c++;
                        break;
                case R_VALUE:
                        copy_to_user((int32_t*) arg, &dat, sizeof(dat));
                        break;
        }
        return 0;
}

static int __init hello_world_init(void)
{
	if((alloc_chrdev_region(&dev,0,1,"sampledd"))<0){
		printk(KERN_INFO "Can't allocate major number\n");
		return -1;
	}
	/*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);
        etx_cdev.owner = THIS_MODULE;
        etx_cdev.ops = &fops;
	
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if((dev_class = class_create(THIS_MODULE,"sampledd")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if((device_create(dev_class,NULL,dev,NULL,"sampledd")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
        }


        printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
	return 0;
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

void __exit hello_world_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("My Program");
MODULE_DESCRIPTION("A simple hello world driver and character driver registeration");
MODULE_VERSION("0.01");
