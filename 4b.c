#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_SIZE 5
#define STRING_MAX_LEN 10 // Maximum length for generated strings
#define PRODUCER_COUNT 2  // Number of producers
#define CONSUMER_COUNT 3  // Number of consumers

// Shared queue to hold strings
char queue[QUEUE_SIZE][STRING_MAX_LEN];
int currentItems = 0; // Number of items currently in the queue

// Semaphores for tracking empty/full slots in the queue
sem_t slots_filled, slots_empty;

// Mutex for synchronizing access to the shared queue
pthread_mutex_t queue_mutex;

// Function to display the current state of the queue
void display_queue_status() {
    printf("Queue state: [");
    for (int i = 0; i < QUEUE_SIZE; i++) {
        if (i < currentItems) {
            printf("%s", queue[i]);
        } else {
            printf("_"); // Display empty slots
        }
        if (i != QUEUE_SIZE - 1) {
            printf(", ");
        }
    }
    printf("] (Items = %d)\n", currentItems);
}

// Function to generate a random string of a given length
void create_random_string(char* str, int max_length) {
    int length = rand() % max_length + 1; // Random length from 1 to max_length
    for (int i = 0; i < length; i++) {
        str[i] = 'A' + rand() % 26; // Generate random uppercase letter
    }
    str[length] = '\0'; // Null-terminate the string
}

// Producer thread function
void* producer_routine(void* arg) {
    int producer_id = *(int*)arg; // Get the producer ID
    for (int i = 0; i < 10; i++) {
        char item[STRING_MAX_LEN];
        create_random_string(item, STRING_MAX_LEN); // Generate a new string

        sem_wait(&slots_empty); // Wait if no empty slots are available
        pthread_mutex_lock(&queue_mutex); // Lock the queue for exclusive access

        strcpy(queue[currentItems++], item); // Add item to the queue
        printf("Producer %d inserted: %s\n", producer_id, item);
        display_queue_status(); // Show updated queue status

        pthread_mutex_unlock(&queue_mutex); // Unlock the queue
        sem_post(&slots_filled); // Signal that a slot has been filled

        sleep(1); // Simulate delay
    }
    pthread_exit(NULL);
}

// Consumer thread function
void* consumer_routine(void* arg) {
    int consumer_id = *(int*)arg; // Get the consumer ID
     for (int i = 0; i < 10; i++) {
        sem_wait(&slots_filled); // Wait if no items are available
        pthread_mutex_lock(&queue_mutex); // Lock the queue for exclusive access

        if (currentItems > 0) {
            char item[STRING_MAX_LEN];
            strcpy(item, queue[--currentItems]); // Remove an item from the queue
            printf("Consumer %d took: %s\n", consumer_id, item);
            display_queue_status(); // Show updated queue status
        }

        pthread_mutex_unlock(&queue_mutex); // Unlock the queue
        sem_post(&slots_empty); // Signal that a slot is now empty

        sleep(1); // Simulate delay
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producers[PRODUCER_COUNT], consumers[CONSUMER_COUNT]; // Thread variables
    int producer_ids[PRODUCER_COUNT], consumer_ids[CONSUMER_COUNT];

    // Initialize the semaphores
    sem_init(&slots_filled, 0, 0); // Initially, no slots are filled
    sem_init(&slots_empty, 0, QUEUE_SIZE); // All slots are initially empty

    // Initialize the mutex
    pthread_mutex_init(&queue_mutex, NULL);

    // Create the producer threads
    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producer_ids[i] = i + 1; // Unique ID for each producer
        pthread_create(&producers[i], NULL, producer_routine, &producer_ids[i]);
    }

    // Create the consumer threads
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumer_ids[i] = i + 1; // Unique ID for each consumer
        pthread_create(&consumers[i], NULL, consumer_routine, &consumer_ids[i]);
    }

    // Wait for producer threads to finish
    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }

    // Optionally cancel the consumer threads after producers finish
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        // pthread_cancel(consumers[i]); // Terminate consumers
        pthread_join(consumers[i], NULL); // Wait for them to exit
    }

    // Destroy the semaphores and mutex
    sem_destroy(&slots_filled);
    sem_destroy(&slots_empty);
    pthread_mutex_destroy(&queue_mutex);

    return 0;
}
