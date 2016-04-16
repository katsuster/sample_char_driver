
#define pr_fmt(fmt) SAMPLE_CHAR_DRV_NAME ": " fmt

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "char-device.h"

#define SAMPLE_CHAR_DEVS    16

static struct cdev *sample_cdev = NULL;
static dev_t sample_devt = 0;

static int sample_char_fops_open(struct inode *inode, struct file *filp)
{
	pr_info("open.\n");

	return 0;
}

static int sample_char_fops_release(struct inode *inode, struct file *filp)
{
	pr_info("open.\n");

	return 0;
}

static const struct file_operations sample_char_fops = {
	.owner   = THIS_MODULE,
	.open    = sample_char_fops_open,
	.release = sample_char_fops_release,
	.llseek  = no_llseek,
};

static int __init sample_char_device_init(void)
{
	const char *name = "sample_char";
	int ret;

	ret = alloc_chrdev_region(&sample_devt, 0, SAMPLE_CHAR_DEVS, name);
	if (ret != 0) {
		pr_warning("alloc_chrdev_region() failed.");
		goto err_out;
	}

	sample_cdev = cdev_alloc();
	if (!sample_cdev) {
		pr_warning("cdev_alloc() failed.");
		goto err_out_reg;
	}

	sample_cdev->owner = THIS_MODULE;
	sample_cdev->ops = &sample_char_fops;

	ret = cdev_add(sample_cdev, sample_devt, SAMPLE_CHAR_DEVS);
	if (ret) {
		pr_warning("cdev_add() failed.");
		goto err_out_alloc;
	}

	pr_info("loaded.\n");

	return 0;

err_out_alloc:
	cdev_del(sample_cdev);

err_out_reg:
	unregister_chrdev_region(sample_devt, SAMPLE_CHAR_DEVS);

err_out:
	return ret;
}

static void __exit sample_char_device_exit(void)
{
	cdev_del(sample_cdev);
	unregister_chrdev_region(sample_devt, SAMPLE_CHAR_DEVS);

	pr_info("unloaded.\n");
}

module_init(sample_char_device_init);
module_exit(sample_char_device_exit);

MODULE_AUTHOR("Katsuhiro Suzuki <katsuhiro@katsuster.net>");
MODULE_DESCRIPTION("Sample character device module");
MODULE_LICENSE("GPL");
