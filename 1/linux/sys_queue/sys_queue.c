#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/slab.h>


static DECLARE_WAIT_QUEUE_HEAD(wq);
static LIST_HEAD(data_queue);

struct queue_value {
	struct list_head queue;
	char data;
};

static unsigned long count;
static struct kobject *sys_queue_kobject;

asmlinkage long sys_queue_push(int value)
{
	struct queue_value *entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		pr_debug("Device queue data allocation failed\n");
		return -1;
	}
	entry->data = value;
	list_add_tail(&entry->queue, &data_queue);
	return 0;
}

asmlinkage long sys_queue_pop(void)
{
	int value;
	struct queue_value *entry;

	if (list_empty(&data_queue))
		return -1;

	entry = list_first_entry(&data_queue, struct queue_value, queue);
	value = entry->data;
	list_del(&entry->queue);
	kfree(entry);
	
	return value;
}

asmlinkage long sys_queue_look_head(void) {
	int value;
	struct queue_value *entry;

	if (list_empty(&data_queue))
		return -1;

	entry = list_first_entry(&data_queue, struct queue_value, queue);
	value = entry->data;
	kfree(entry);

	return value;
}

static ssize_t count_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%lu\n", count);
}

static struct kobj_attribute count_attribute = __ATTR_RO(count);

static struct attribute *attrs[] = {
	&count_attribute.attr,
	NULL,
};

static int __init sys_queue_init(void)
{
	int retval;

	sys_queue_kobject = kobject_create_and_add("sys_queue", kernel_kobj);
	if (!sys_queue_kobject)
		pr_debug("queue_dev: can't create kobject");

	retval = sysfs_create_file(sys_queue_kobject, *attrs);
	if (retval) {
		pr_debug("queue_dev: can't create sysfs group");
		kobject_put(sys_queue_kobject);
		return retval;
	}

	return 0;
}

static void __exit sys_queue_exit(void)
{
	kobject_put(sys_queue_kobject);
}

module_init(sys_queue_init);
module_exit(sys_queue_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matheus Guilherme Gonçalves");
MODULE_DESCRIPTION("Simple queue module");
