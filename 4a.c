//gcc -o name name.c -pthread

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_CAPACITY 5 // Maximum buffer size
#define STRING_MAX_LEN 10 // Maximum length for generated strings
#define NUM_PRODUCERS 2 // Number of producers
#define NUM_CONSUMERS 3 // Number of consumers

char queue[QUEUE_CAPACITY][STRING_MAX_LEN]; // Shared buffer
int itemCount = 0; // Number of items currently in the buffer

pthread_mutex_t lock; // Mutex for synchronization

// Function to display the current state of the queue
void show_queue_status() {
    printf("Queue state: [");
    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        if (i < itemCount) {
            printf("%s", queue[i]); // Print each string in the queue
        } else {
            printf("_"); // Display empty spots
        }
        if (i != QUEUE_CAPACITY - 1) {
            printf(", ");
        }
    }
    printf("] (Items = %d)\n", itemCount);
}

// Function to generate random strings
void create_random_string(char* str, int max_len) {
    int length = rand() % max_len + 1; // Random length from 1 to max_len
    for (int i = 0; i < length; i++) {
        str[i] = 'A' + rand() % 26; // Random uppercase letters
    }
    str[length] = '\0'; // Null-terminate the string
}

// Producer thread function
void* producer_routine(void* arg) {
    int producer_id = *(int*)arg; // Producer serial number

    for (int i = 0; i < 10; i++) {
        char item[STRING_MAX_LEN];
        create_random_string(item, STRING_MAX_LEN); // Produce a new string

        pthread_mutex_lock(&lock); // Lock before accessing shared buffer

        if (itemCount < QUEUE_CAPACITY) {
            strcpy(queue[itemCount++], item); // Add item to the buffer
            printf("Producer #%d added: %s\n\n", producer_id, item);
            show_queue_status(); // Show updated queue
        } else {
            printf("Producer #%d: Queue is full, waiting...\n", producer_id);
        }

        pthread_mutex_unlock(&lock); // Unlock after accessing shared buffer

        sleep(1); // Simulate delay
    }

    pthread_exit(NULL);
}

// Consumer thread function
void* consumer_routine(void* arg) {
    int consumer_id = *(int*)arg; // Consumer serial number

    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock); // Lock before accessing shared buffer

        if (itemCount > 0) {
            char consumed_item[STRING_MAX_LEN];
            strcpy(consumed_item, queue[--itemCount]); // Remove item from buffer
            printf("Consumer #%d took: %s\n\n", consumer_id, consumed_item);
            show_queue_status(); // Show updated queue
        } else {
            printf("Consumer #%d: Queue is empty, waiting...\n", consumer_id);
        }

        pthread_mutex_unlock(&lock); // Unlock after accessing shared buffer

        sleep(1); // Simulate delay
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS], consumer_ids[NUM_CONSUMERS];

    pthread_mutex_init(&lock, NULL); // Initialize mutex

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1; // Assign serial number
        pthread_create(&producers[i], NULL, producer_routine, &producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1; // Assign serial number
        pthread_create(&consumers[i], NULL, consumer_routine, &consumer_ids[i]);
    }

    // Wait for all producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for all consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&lock); // Destroy mutex

    return 0;
}
