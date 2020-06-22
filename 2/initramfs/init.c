#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stop.h"
#include "queue.h"

// Consts definition
#define MAX 100
#define TRUE 1

// Function declarations
void down(int semaphore_pid);
void up(int semaphore_pid);
int init_semaphore(int count);

// Global Variables
int mutex = 1;
int waiting_process;
int semaphore_id;


// Init semaphore
int init_semaphore(int count) {
	if (count == MAX) return -1;
	return count;
}

// Down on semaphore
void down(int semaphore_pid) {	
	//printf("In down: semaphore_pid=%d\n", semaphore_pid);
	//printf("Mutex before down: %d\n", mutex);
	if (mutex == 0) {
		//printf("Putting %d in wait queue.\n", semaphore_pid);
		waiting_process = semaphore_pid;
		stop_process(semaphore_pid);
		//printf("Process %d stoped.\n", semaphore_pid);
	}
	mutex--;
	//printf("Mutex after down: %d\n", mutex);
}

// Up on semaphore
void up(int semaphore_pid) {
	//printf("In up: semaphore_pid=%d\n", semaphore_pid);
	//printf("Mutex before up: %d\n", mutex);
	mutex++;
	//printf("Mutex after up: %d\n", mutex);
	//printf("wait_queue_head: %d\n", waiting_process);
	if (semaphore_pid == waiting_process) {
		//printf("Continuing process %d\n", semaphore_pid);
		continue_process();
	}
}

int main() {
	printf("Entered main.\n");
	int pid = 0;
	int parent_pid;
	int child_pid;
	int count = 0;
	int mutex = 1;

	// Grandparent - Parent fork
	pid = fork();

	if (pid != 0) {
		/* Grandparent code */
		printf("Inside Grandparent code.\n");

		parent_pid = getpid();
		int semaphore_pid = 0;

		semaphore_pid = fork();

		// Grandparent - Child fork
		while (TRUE) {
			if(semaphore_pid != 0) {
				printf("Inside Child A code.\n");
				/* Child Process A */
				while (TRUE) {
					printf("Child A process: semaphore_pid = %d\n", parent_pid);
					down(parent_pid);
					printf("Child A process inside critic region in semaphore: %d\n", semaphore_id);
					sleep(0.5);
					up(parent_pid);
					printf("Child A process outside critic region in semaphore: %d\n", semaphore_id);
				}
			}
			else {
				printf("Inside Child B code.\n");
				/* Child Process B */
				child_pid = getpid();
				while (TRUE) {
					printf("Child B process: semaphore_pid = %d\n", child_pid);
					down(child_pid);
					printf("Child B process inside critic region in semaphore: %d\n", semaphore_id);
					sleep(0.5);
					up(child_pid);
					printf("Child B process outside critic region in semaphore: %d\n", semaphore_id);
				}
			}
		}
	} 
	else {
		printf("Inside Parent code.\n");
		// Parent code 
		while (TRUE) {
			semaphore_id = init_semaphore(count);
			printf("New semaphore: semaphore_id = %d\n", semaphore_id);
			if (semaphore_id == -1) {
				printf("Can't allocate more memory in Kernel.\n");
				return -1;
			} 
			count++;
			sleep(1);
		}
	}
}