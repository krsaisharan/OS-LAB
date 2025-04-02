#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define BUFFER_SIZE 3

sem_t mutex, empty, full;
int x = 0;

void producer() {
    sem_wait(&empty);
    sem_wait(&mutex);
    x++;
    printf("Producer has produced: Item %d\n", x);
    sem_post(&mutex);
    sem_post(&full);
}

void consumer() {
    sem_wait(&full);
    sem_wait(&mutex);
    printf("Consumer has consumed: Item %d\n", x);
    x--;
    sem_post(&mutex);
    sem_post(&empty);
}

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    int choice;
    while(1) {
        printf("Enter 1.Producer 2.Consumer 3.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 3) break;

        int sem_val;
        switch(choice) {
            case 1:
                sem_getvalue(&empty, &sem_val);
                if (sem_val <= 0) {
                    printf("Buffer is full!\n");
                } else {
                    producer();
                }
                break;
            case 2:
                sem_getvalue(&full, &sem_val);
                if (sem_val <= 0) {
                    printf("Buffer is empty!\n");
                } else {
                    consumer();
                }
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
