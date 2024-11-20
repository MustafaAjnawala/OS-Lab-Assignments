#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Global time variables
int hours = 0, minutes = 0, seconds = 0;
pthread_mutex_t time_mutex;

// Function to update time every second
void* time_updater(void* arg) {
    while (1) {
        printf("\n[UPDATER] Waiting to acquire lock...\n");
        pthread_mutex_lock(&time_mutex);
        printf("[UPDATER] Lock acquired\n");

        // Increment seconds
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
        printf("[UPDATER] Time updated to: %02d:%02d:%02d\n", hours, minutes, seconds);

        printf("[UPDATER] Releasing lock...\n");
        pthread_mutex_unlock(&time_mutex);
        printf("[UPDATER] Lock released\n");

        sleep(1); // Delay for 1 second
    }
    return NULL;
}

// Function to display the time
void* time_display(void* arg) {
    while (1) {
        printf("\n[DISPLAY] Waiting for lock...\n");
        pthread_mutex_lock(&time_mutex);
        printf("[DISPLAY] Lock acquired\n");

        // Display the current time
        printf("[DISPLAY] Current time = %02d:%02d:%02d\n", hours, minutes, seconds);

        printf("[DISPLAY] Releasing lock...\n");
        pthread_mutex_unlock(&time_mutex);
        printf("[DISPLAY] Lock released\n");

        sleep(1); // Update every second
    }
    return NULL;
}

int main() {
    pthread_t updater_thread, display_thread;

    // Initialize mutex for synchronization
    printf("Main: Initializing mutex...\n");
    pthread_mutex_init(&time_mutex, NULL);

    // Create threads for updating and displaying time
    printf("Main: Starting update and display threads...\n");
    pthread_create(&updater_thread, NULL, time_updater, NULL);
    pthread_create(&display_thread, NULL, time_display, NULL);

    // Wait for both threads to complete
    pthread_join(updater_thread, NULL);
    pthread_join(display_thread, NULL);

    // Destroy the mutex
    printf("Main: Destroying mutex...\n");
    pthread_mutex_destroy(&time_mutex);

    return 0;
}

