/*
 * Ambulance Management System
 * DSA Mini Project - C Language
 *
 * Concepts used:
 * - Structures
 * - Arrays
 * - Queue (FIFO)
 * - Searching and menu-driven programming
 */

#include <stdio.h>
#include <string.h>

#define MAX_AMBULANCES 5
#define MAX_PATIENTS 100
#define NAME_LEN 50
#define LOCATION_LEN 80

typedef struct {
    int id;
    char driver[NAME_LEN];
    int available; /* 1 = available, 0 = assigned */
} Ambulance;

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    char location[LOCATION_LEN];
    int emergencyLevel; /* 1 = Critical, 2 = High, 3 = Normal */
    int ambulanceId;    /* 0 means not assigned */
} Patient;

typedef struct {
    int items[MAX_PATIENTS];
    int front;
    int rear;
    int count;
} Queue;

Ambulance ambulances[MAX_AMBULANCES] = {
    {101, "Amit", 1},
    {102, "Rahul", 1},
    {103, "Suresh", 1},
    {104, "Mohit", 1},
    {105, "Anil", 1}
};

Patient patients[MAX_PATIENTS];
int patientCount = 0;
int nextPatientId = 1;
Queue emergencyQueue = {{0}, 0, -1, 0};

void initializeQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isQueueFull(const Queue *q) {
    return q->count == MAX_PATIENTS;
}

int isQueueEmpty(const Queue *q) {
    return q->count == 0;
}

int enqueue(Queue *q, int patientIndex) {
    if (isQueueFull(q)) {
        printf("Emergency queue is full. Patient could not be queued.\n");
        return 0;
    }
    q->rear = (q->rear + 1) % MAX_PATIENTS;
    q->items[q->rear] = patientIndex;
    q->count++;
    return 1;
}

int dequeue(Queue *q) {
    int value;
    if (isQueueEmpty(q)) {
        return -1;
    }
    value = q->items[q->front];
    q->front = (q->front + 1) % MAX_PATIENTS;
    q->count--;
    return value;
}

int findAvailableAmbulance(void) {
    int i;
    for (i = 0; i < MAX_AMBULANCES; i++) {
        if (ambulances[i].available) {
            return i;
        }
    }
    return -1;
}

void showAmbulances(void) {
    int i;
    printf("\n---------------- Ambulance List ----------------\n");
    printf("%-10s %-20s %-15s\n", "ID", "Driver", "Status");
    for (i = 0; i < MAX_AMBULANCES; i++) {
        printf("%-10d %-20s %-15s\n",
               ambulances[i].id,
               ambulances[i].driver,
               ambulances[i].available ? "Available" : "Assigned");
    }
}

void addPatient(void) {
    Patient p;
    int choice;

    if (patientCount >= MAX_PATIENTS) {
        printf("Patient record limit reached.\n");
        return;
    }

    p.id = nextPatientId++;
    printf("\nEnter patient name: ");
    scanf(" %49[^\n]", p.name);

    printf("Enter patient age: ");
    if (scanf("%d", &p.age) != 1 || p.age < 0) {
        printf("Invalid age. Please enter a non-negative number.\n");
        while (getchar() != '\n') {}
        nextPatientId--;
        return;
    }

    printf("Enter pickup location: ");
    scanf(" %79[^\n]", p.location);

    printf("Emergency level (1-Critical, 2-High, 3-Normal): ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid emergency level. Please choose 1, 2, or 3.\n");
        while (getchar() != '\n') {}
        nextPatientId--;
        return;
    }
    p.emergencyLevel = choice;
    p.ambulanceId = 0;

    patients[patientCount] = p;
    if (enqueue(&emergencyQueue, patientCount)) {
        patientCount++;
        printf("\nPatient registered successfully. Patient ID: %d\n", p.id);
    } else {
        nextPatientId--;
    }
}

void assignAmbulance(void) {
    int patientIndex;
    int ambulanceIndex;

    if (isQueueEmpty(&emergencyQueue)) {
        printf("\nNo patients are waiting in the queue.\n");
        return;
    }

    ambulanceIndex = findAvailableAmbulance();
    if (ambulanceIndex == -1) {
        printf("\nNo ambulance is currently available. Patient remains in queue.\n");
        return;
    }

    patientIndex = dequeue(&emergencyQueue);
    if (patientIndex < 0 || patientIndex >= patientCount) {
        printf("Queue data error. Please contact the project administrator.\n");
        return;
    }

    patients[patientIndex].ambulanceId = ambulances[ambulanceIndex].id;
    ambulances[ambulanceIndex].available = 0;

    printf("\nAmbulance assigned successfully!\n");
    printf("Patient: %s (ID: %d)\n",
           patients[patientIndex].name, patients[patientIndex].id);
    printf("Ambulance ID: %d | Driver: %s\n",
           ambulances[ambulanceIndex].id, ambulances[ambulanceIndex].driver);
    printf("Pickup location: %s\n", patients[patientIndex].location);
    printf("Note: This demo assigns patients in FIFO queue order.\n");
}

void completeTrip(void) {
    int id, i, j;
    printf("\nEnter ambulance ID whose trip is complete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n') {}
        return;
    }

    for (i = 0; i < MAX_AMBULANCES; i++) {
        if (ambulances[i].id == id) {
            if (ambulances[i].available) {
                printf("That ambulance is already available.\n");
                return;
            }

            ambulances[i].available = 1;
            for (j = 0; j < patientCount; j++) {
                if (patients[j].ambulanceId == id) {
                    patients[j].ambulanceId = -id; /* trip completed */
                }
            }
            printf("Ambulance %d is now available.\n", id);
            return;
        }
    }
    printf("Ambulance ID not found.\n");
}

void showPatients(void) {
    int i;
    printf("\n---------------- Patient Records ----------------\n");
    if (patientCount == 0) {
        printf("No patient records available.\n");
        return;
    }

    printf("%-5s %-20s %-5s %-25s %-12s %-15s\n",
           "ID", "Name", "Age", "Location", "Emergency", "Ambulance");
    for (i = 0; i < patientCount; i++) {
        const char *level;
        char ambulanceText[20];

        if (patients[i].emergencyLevel == 1) level = "Critical";
        else if (patients[i].emergencyLevel == 2) level = "High";
        else level = "Normal";

        if (patients[i].ambulanceId == 0) {
            strcpy(ambulanceText, "Waiting");
        } else if (patients[i].ambulanceId < 0) {
            strcpy(ambulanceText, "Completed");
        } else {
            snprintf(ambulanceText, sizeof(ambulanceText), "%d",
                     patients[i].ambulanceId);
        }

        printf("%-5d %-20.20s %-5d %-25.25s %-12s %-15s\n",
               patients[i].id, patients[i].name, patients[i].age,
               patients[i].location, level, ambulanceText);
    }
}

void showWaitingQueue(void) {
    int i, index;
    Queue copy = emergencyQueue;

    printf("\n---------------- Waiting Queue ----------------\n");
    if (isQueueEmpty(&copy)) {
        printf("No patients are waiting.\n");
        return;
    }

    printf("Order: ");
    while (!isQueueEmpty(&copy)) {
        index = dequeue(&copy);
        if (index >= 0 && index < patientCount) {
            printf("%s (ID %d)", patients[index].name, patients[index].id);
            if (!isQueueEmpty(&copy)) printf(" -> ");
        }
    }
    printf("\n");
}

int main(void) {
    int choice;
    initializeQueue(&emergencyQueue);

    do {
        printf("\n============================================\n");
        printf("       AMBULANCE MANAGEMENT SYSTEM\n");
        printf("============================================\n");
        printf("1. Add patient / emergency request\n");
        printf("2. Assign ambulance to next waiting patient\n");
        printf("3. Display ambulance list\n");
        printf("4. Display patient records\n");
        printf("5. Display waiting queue\n");
        printf("6. Mark ambulance trip as complete\n");
        printf("0. Exit\n");
        printf("--------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");
            while (getchar() != '\n') {}
            continue;
        }

        switch (choice) {
            case 1: addPatient(); break;
            case 2: assignAmbulance(); break;
            case 3: showAmbulances(); break;
            case 4: showPatients(); break;
            case 5: showWaitingQueue(); break;
            case 6: completeTrip(); break;
            case 0: printf("\nThank you for using the system.\n"); break;
            default: printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
