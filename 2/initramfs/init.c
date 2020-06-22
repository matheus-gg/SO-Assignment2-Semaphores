#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stop.h"

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

// Init semaphore
int init_semaphore(int count) {
	if (count == MAX) return -1;
	return count;
}

// Down on semaphore
void down(int semaphore_pid) {	
	if (mutex == 0) {
		waiting_process = semaphore_pid;
		stop_process(semaphore_pid);
	}
	mutex--;
}

// Up on semaphore
void up(int semaphore_pid) {
	mutex++;
	if (semaphore_pid == waiting_process) {
		continue_process();
	}
}

int main() {
	printf("Entered main.\n");
	int count = 0;

	while (TRUE) {
		int pid = 0;

		pid = fork();

		if (pid != 0) {
			// Grandparent code 
			printf("Inside Grandparent code.\n");
			int semaphore_id;
			while (TRUE) {
				semaphore_id = init_semaphore(count);
				printf("New semaphore: semaphore_id = %d\n", semaphore_id);
				if (semaphore_id == -1) {
					printf("Can't allocate more memory in Kernel.\n");
					return -1;
				} 
				count++;
				
				int semaphore_pid = 0;

				semaphore_pid = fork();

				// Parent - Child fork
				while (TRUE) {
					if(semaphore_pid != 0) {
						printf("Inside Child A code.\n");
						/* Child Process A */
						int parent_pid = getpid();
						while (TRUE) {
							printf("Child A process: semaphore_pid = %d\n", parent_pid);
							down(parent_pid);
							printf("Child A process (pid=%d) inside critic region.\n", parent_pid);
							up(parent_pid);
							printf("Child A process (pid=%d) outside critic region.\n", parent_pid);
							sleep(0.5);
						}
					}
					else {
						printf("Inside Child B code.\n");
						/* Child Process B */
						int child_pid = getpid();
						while (TRUE) {
							printf("Child B process: semaphore_pid = %d\n", child_pid);
							down(child_pid);
							printf("Child B process (pid=%d) inside critic region.\n", child_pid);
							up(child_pid);
							printf("Child B process (pid=%d) outside critic region.\n", child_pid);
							sleep(0.5);
						}
					}
				}
			}
		}
		else {
			/* Parent code */
			sleep(1);
		}
	}
}