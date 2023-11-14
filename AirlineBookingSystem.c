#include <stdio.h>
#include <stdlib.h>

#define TOTAL_SEATS 100

// Define a structure for a passenger
typedef struct Passenger {
    char name[50];
    int seatNumber;
    struct Passenger* next;
} Passenger;

// Define a queue structure for booking requests
typedef struct Queue {
    Passenger* front;
    Passenger* rear;
} Queue;

int seatAvailability[TOTAL_SEATS];

// Function to initialize seat availability
void initializeSeatAvailability() {
    for (int i = 0; i < TOTAL_SEATS; i++) {
        seatAvailability[i] = 1; // 1 represents available, 0 represents booked
    }
}

// Function to create a new passenger
Passenger* createPassenger(char name[], int seatNumber) {
    Passenger* newPassenger = (Passenger*)malloc(sizeof(Passenger));
    if (newPassenger != NULL) {
        strcpy(newPassenger->name, name);
        newPassenger->seatNumber = seatNumber;
        newPassenger->next = NULL;
    }
    return newPassenger;
}

// Function to initialize a queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue != NULL) {
        queue->front = queue->rear = NULL;
    }
    return queue;
}

// Function to enqueue a booking request
void enqueue(Queue* queue, char name[], int seatNumber) {
    Passenger* newPassenger = createPassenger(name, seatNumber);
    if (queue->rear == NULL) {
        queue->front = queue->rear = newPassenger;
    } else {
        queue->rear->next = newPassenger;
        queue->rear = newPassenger;
    }
    seatAvailability[seatNumber - 1] = 0; // Mark the seat as booked
}

// Function to cancel a booking by seat number
void cancelBooking(Queue* queue, int seatNumber) {
    Passenger* current = queue->front;
    Passenger* prev = NULL;

    while (current != NULL) {
        if (current->seatNumber == seatNumber) {
            if (prev == NULL) {
                // If the first node is to be deleted
                queue->front = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            if (queue->front == NULL) {
                queue->rear = NULL; // The queue is now empty
            }
            seatAvailability[seatNumber - 1] = 1; // Mark the seat as available
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Booking with seat number %d not found.\n", seatNumber);
}

// Function to display the booking queue
void displayQueue(Queue* queue) {
    Passenger* current = queue->front;
    while (current != NULL) {
        printf("Name: %s, Seat Number: %d\n", current->name, current->seatNumber);
        current = current->next;
    }
}

int main() {
    Queue* bookingQueue = createQueue();
    initializeSeatAvailability();

    int choice, seatNumber;
    char name[50];

    while (1) {
        printf("\n1. Book a ticket\n");
        printf("2. Cancel a booking\n");
        printf("3. Display booking queue\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter passenger name: ");
                scanf("%s", name);
                printf("Enter seat number (1-%d): ", TOTAL_SEATS);
                scanf("%d", &seatNumber);
                if (seatNumber < 1 || seatNumber > TOTAL_SEATS) {
                    printf("Invalid seat number. Please try again.\n");
                } else if (seatAvailability[seatNumber - 1] == 0) {
                    printf("Seat %d is already booked. Please choose another seat.\n", seatNumber);
                } else {
                    enqueue(bookingQueue, name, seatNumber);
                    printf("Booking added for seat %d.\n", seatNumber);
                }
                break;
            case 2:
                printf("Enter seat number to cancel: ");
                scanf("%d", &seatNumber);
                if (seatNumber < 1 || seatNumber > TOTAL_SEATS) {
                    printf("Invalid seat number. Please try again.\n");
                } else {
                    cancelBooking(bookingQueue, seatNumber);
                    printf("Booking for seat %d is canceled.\n", seatNumber);
                }
                break;
            case 3:
                printf("Booking Queue:\n");
                displayQueue(bookingQueue);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}