#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_queue_push 404
#define __NR_queue_pop 405
#define __NR_queue_look_head 406

long queue_push(int value)
{
	return syscall(__NR_queue_push, value);
}

long queue_pop()
{
	return syscall(__NR_queue_pop);
}

long queue_look_head() {
	return syscall(__NR_queue_look_head);
}
