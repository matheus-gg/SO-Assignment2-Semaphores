#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stop.h"
#include "queue.h"

// Max Number os Semaphores
#define MAX 100

// Function declarations
void down(int semaphore_pid, struct Queue* q);
void up(int semaphore_pid, struct Queue* q);
int init_semaphore(int count);

// Global variables
int mutex = 1;
int semaphore_busy = 0;

// Init semaphore
int init_semaphore(int count) {
	if (count == MAX) return -1;
	return count;
}

// Down on semaphore
void down(int semaphore_pid, struct Queue* q) {
	if (mutex == 0) {
		enQueue(q, semaphore_pid);
		stop_process(semaphore_pid);
	}
	mutex--;
}

// Up on semaphore
void up(int semaphore_pid, struct Queue* q) {
	mutex ++;
	if ((semaphore_pid = deQueue(q))) {
		continue_process();
	}
}

int main() {
	int count = 0;
	int semaphore_id = 0;
	int pid = 0;

	// Grandparent - Parent fork
	pid = fork();

	if (pid != 0) {
		// Grandparent code 
		semaphore_id = init_semaphore(count);
		if (semaphore_id == -1) {
			printf("Can't allocate more memory in Kernel.");
			return -1;
		} 
		count++;
	} 
	else {
		/* Parent code */
		struct Queue* q = createQueue();
		int semaphore_pid = 0;

		// Parent - Child fork
		semaphore_pid = fork();

		if(semaphore_pid != 0) {
			/* Child Process A */
			if (semaphore_busy == 0) {
				semaphore_busy = 1;
				down(semaphore_pid, q);
				printf("Process A inside critic region in semaphore: %d", semaphore_id);
				up(semaphore_pid, q);
				semaphore_busy = 0;
				printf("Process A outside critic region in semaphore: %d", semaphore_id);
			}
			else {
				sleep(1);
			} 
		}
		else {
			/* Child Process B */
			if (semaphore_busy == 0) {
				semaphore_busy = 1;
				down(semaphore_id, q);
				printf("Process B inside critic region in semaphore: %d", semaphore_id);
				up(semaphore_id, q);
				semaphore_busy = 0;
				printf("Process B outside critic region in semaphore: %d", semaphore_id);
			}
			else {
				sleep(1);
			}
		}
	}
}