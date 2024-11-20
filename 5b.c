#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

// Global variables to hold time data
int hours = 0, minutes = 0, seconds = 0;
sem_t sem_read, sem_write; // Renamed semaphores to avoid any naming conflicts

// Reader thread function
void* time_reader(void* arg) {
    while (1) {
        int sem_val;
        // Step 1: Get the current value of the read semaphore
        sem_getvalue(&sem_read, &sem_val);
        printf("\n[READER] Verifying if read semaphore equals 1 (current value: %d)\n", sem_val);

        if (sem_val == 1) {
            // Step 2: Decrement read semaphore when it equals 1
            sem_wait(&sem_read);
            printf("[READER] Semaphore for reading decremented to 0\n");

            // Step 3: Read the global time values
            printf("[READER] Fetching time values...\n");
            // Step 4: Display the time
            printf("[READER] Current time: %02d:%02d:%02d\n", hours, minutes, seconds);

            // Step 5: Post to the write semaphore, allowing writing
            sem_post(&sem_write);
            printf("[READER] Write semaphore incremented to 1\n");
        } else {
            printf("[READER] Read semaphore is not 1, waiting...\n");
        }
        sleep(1);  // Sleep for 1 second to avoid busy waiting
    }
    return NULL;
}

// Writer thread function
void* time_writer(void* arg) {
    while (1) {
        int sem_val;
        // Step 1: Fetch current value of write semaphore
        sem_getvalue(&sem_write, &sem_val);
        printf("\n[WRITER] Checking if write semaphore equals 1 (current value: %d)\n", sem_val);

        if (sem_val == 1) {
            // Step 2: Decrement write semaphore
            sem_wait(&sem_write);
            printf("[WRITER] Semaphore for writing decremented to 0\n");

            // Step 3: Update the time values
            seconds++;
            if (seconds == 60) {
                seconds = 0;
                minutes++;
                if (minutes == 60) {
                    minutes = 0;
                    hours++;
                    if (hours == 24) {
                        hours = 0;
                    }
                }
            }
            printf("[WRITER] Time updated to: %02d:%02d:%02d\n", hours, minutes, seconds);

            // Step 4: Post to the read semaphore, enabling reading
            sem_post(&sem_read);
            printf("[WRITER] Read semaphore incremented to 1\n");
        } else {
            printf("[WRITER] Write semaphore is not 1, waiting...\n");
        }
        sleep(1);  // Sleep for 1 second to avoid busy waiting
    }
    return NULL;
}

int main() {
    // Step 1: Program initialization
    printf("Main: Initializing the system\n");

    // Step 2: Thread creation for reader and writer
    pthread_t reader_thread, writer_thread;

    // Step 3: Initialize semaphores (reader starts at 0, writer starts at 1)
    printf("Main: Initializing semaphores (sem_read=0, sem_write=1)\n");
    sem_init(&sem_read, 0, 0);   // Initial value set to 0
    sem_init(&sem_write, 0, 1);  // Initial value set to 1

    // Create threads for reading and writing
    printf("Main: Launching reader and writer threads\n");
    pthread_create(&reader_thread, NULL, time_reader, NULL);
    pthread_create(&writer_thread, NULL, time_writer, NULL);

    // Step 4: Join the threads (ensure proper execution)
    printf("Main: Waiting for threads to finish\n");
    pthread_join(reader_thread, NULL);
    pthread_join(writer_thread, NULL);

    // Step 5: Clean up and destroy semaphores
    printf("Main: Cleaning up resources\n");
    sem_destroy(&sem_read);
    sem_destroy(&sem_write);

    // Step 6: Program completion
    printf("Main: Program execution complete\n");
    return 0;
}

