#include <asm/barrier.h>
#include <linux/linkage.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/delay.h>

#define set_task_state(tsk, state_value)		\
	do { (tsk)->state = (state_value); smp_mb(); } while (0)

DECLARE_WAIT_QUEUE_HEAD(queue);
wait_queue_t *wait;
static atomic_t awake = ATOMIC_INIT(0);
//static atomic_t awake = ATOMIC_INIT(0);

static long stop_process(struct task_struct *p)
{
	wait = kmalloc(sizeof(*wait), GFP_KERNEL);
	if (!wait)
		return -1;
		
	init_wait(wait);
	wait->private = p;
	add_wait_queue_exclusive(&queue, wait);
	prepare_to_wait_exclusive(&queue, wait, TASK_INTERRUPTIBLE);
	atomic_set(&awake, 0);
	wait_event_interruptible(queue, atomic_read(&awake));

	schedule();

	return 0;
}

asmlinkage long sys_stop_process(int pid)
{
	struct task_struct *p = find_task_by_vpid(pid);
	return p ? stop_process(p) : -1;
}

asmlinkage long sys_continue_process(void)
{
	usleep_range(1000000, 1000001);
	atomic_set(&awake, 1);
	//wake_up_interruptible(&queue);
	return 0;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matheus Guilherme Gonçalves");
MODULE_DESCRIPTION("Process stop/continue module");
