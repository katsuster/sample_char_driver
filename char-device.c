
#define pr_fmt(fmt) SAMPLE_CHAR_DRV_NAME ": " fmt

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include "char-device.h"

#define SAMPLE_CHAR_DEVS    16

static struct cdev *sample_cdev;
static dev_t sample_devt;
static struct class *sample_class;

static int sample_char_fops_open(struct inode *inode, struct file *filp)
{
	pr_info("open.\n");

	return 0;
}

static int sample_char_fops_release(struct inode *inode, struct file *filp)
{
	pr_info("release.\n");

	return 0;
}
static ssize_t sample_char_fops_read(struct file *filp, char __user *buf,
		size_t cnt, loff_t *ppos)
{
	pr_info("read %d bytes.\n", (int)cnt);

	return cnt;
}

static ssize_t sample_char_fops_write(struct file *filp, const char __user *buf,
		size_t cnt, loff_t *ppos)
{
	pr_info("write %d bytes.\n", (int)cnt);

	return cnt;
}

static const struct file_operations sample_char_fops = {
	.owner   = THIS_MODULE,
	.open    = sample_char_fops_open,
	.release = sample_char_fops_release,
	.read    = sample_char_fops_read,
	.write   = sample_char_fops_write,
	.llseek  = no_llseek,
};

static char *sample_class_devnode(struct device *dev, umode_t *mode)
{
	if (mode != NULL) {
		*mode = 0666;
	}

	return kasprintf(GFP_KERNEL, "sample_char/%s", dev_name(dev));
}

static int __init sample_char_device_init(void)
{
	const char *name = "sample_char";
	struct device *dev;
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
	kobject_set_name(&sample_cdev->kobj, "%s", name);

	ret = cdev_add(sample_cdev, sample_devt, SAMPLE_CHAR_DEVS);
	if (ret) {
		pr_warning("cdev_add() failed.");
		goto err_out_alloc;
	}

	sample_class = class_create(THIS_MODULE, "sample_char_class");
	if (IS_ERR(sample_class)) {
		pr_warning("class_create() failed.");
		goto err_out_alloc;
	}
	sample_class->devnode = sample_class_devnode;

	dev = device_create(sample_class, NULL, sample_devt, NULL, "sample_char_dev");
	if (IS_ERR(dev)) {
		pr_warning("device_create() failed.");
		goto err_out_class;
	}

	pr_info("loaded.\n");

	return 0;

err_out_class:
	class_destroy(sample_class);

err_out_alloc:
	cdev_del(sample_cdev);

err_out_reg:
	unregister_chrdev_region(sample_devt, SAMPLE_CHAR_DEVS);

err_out:
	return ret;
}

static void __exit sample_char_device_exit(void)
{
	device_destroy(sample_class, sample_devt);
	class_destroy(sample_class);

	cdev_del(sample_cdev);
	unregister_chrdev_region(sample_devt, SAMPLE_CHAR_DEVS);

	pr_info("unloaded.\n");
}

module_init(sample_char_device_init);
module_exit(sample_char_device_exit);

MODULE_AUTHOR("Katsuhiro Suzuki <katsuhiro@katsuster.net>");
MODULE_DESCRIPTION("Sample character device module");
MODULE_LICENSE("GPL");
