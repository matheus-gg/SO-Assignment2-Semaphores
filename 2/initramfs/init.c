#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "stop.h"
#include "queue.h"

// Consts definition
#define MAX 100
#define TRUE 1

// Function declarations
void down(int semaphore_pid);
void up(int semaphore_pid);
int init_semaphore(int count);

// ptherad id vector and lock varibale for mutual exclusion on critic region
pthread_mutex_t lock;
int mutex = 1;

// Global variables
int count = 0;

// Init semaphore
int init_semaphore(int count) {
	if (count == MAX) return -1;
	return count;
}

// Down on semaphore
void down(int semaphore_pid) {	
	printf("Mutex before down: %d\n", mutex);
	printf("In down.\n");
	int try = pthread_mutex_lock(&lock);
	if (try != 0) {
		printf("Putting %d in wait queue.\n", semaphore_pid);
		queue_push(semaphore_pid);
		stop_process(semaphore_pid);
		printf("Process %d stoped.\n", semaphore_pid);
	}
	mutex--;
	printf("Mutex after down: %d\n", mutex);
}

// Up on semaphore
void up(int semaphore_pid) {
	printf("In up.\n");
	printf("Mutex before up: %d\n", mutex);
	mutex++;
	printf("Mutex after up: %d\n", mutex);
	pthread_mutex_unlock(&lock);
	if ((long)semaphore_pid == queue_look_head()) {
		printf("Continuing process %d\n", semaphore_pid);
		long aux = queue_pop();
		printf("Queue pop: %ld", aux);
		continue_process();
	}
}

int main() {
	printf("Entered main.\n");
	int semaphore_id = 0;
	int pid = 0;
	int parent_pid;
	int child_pid;

	// Grandparent - Parent fork
	pid = fork();

	if (pid != 0) {
		printf("Inside grandparent code.\n");
		// Grandparent code 
		while (TRUE) {
			semaphore_id = init_semaphore(count);
			printf("New semaphore: semaphore_id = %d\n", semaphore_id);
			if (semaphore_id == -1) {
				printf("Can't allocate more memory in Kernel.\n");
				return -1;
			} 
			count++;
			sleep(2);
		}
	} 
	else {
		/* Parent code */
		printf("Inside parent code.\n");

		parent_pid = getpid();
		int semaphore_pid = 0;

		semaphore_pid = fork();

		// Parent - Child fork
		while (TRUE) {
			if(semaphore_pid != 0) {
				printf("Inside Parent code.\n");
				/* Child Process A */
				while (TRUE) {
					printf("Parent process: semaphore_pid = %d\n", parent_pid);
					down(parent_pid);
					printf("Parent process inside critic region in semaphore: %d\n", semaphore_id);
					sleep(0.4);
					up(parent_pid);
					printf("Parent process outside critic region in semaphore: %d\n", semaphore_id);
				}
			}
			else {
				printf("Inside Child code.\n");
				/* Child Process B */
				child_pid = getpid();
				while (TRUE) {
					sleep(0.5);
					printf("Child process: semaphore_pid = %d\n", child_pid);
					down(child_pid);
					printf("Child process inside critic region in semaphore: %d\n", semaphore_id);
					sleep(0.4);
					up(child_pid);
					printf("Child process outside critic region in semaphore: %d\n", semaphore_id);
				}
			}
		}
	}
}