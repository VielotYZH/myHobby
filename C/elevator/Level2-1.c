#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define MAX_LOAD 4
#define TASK_SIZE sizeof(Task)

typedef struct ElevatorState {
	int floor;
	int number;
	int direction;
	int destination;
	int userDestination[MAX_LOAD];
}ElevatorState;
typedef struct Task {
	int userFloor;
	int destination;
	struct Task* next;
	struct Task* pre;
}Task;

int toBeDone[37][3];

Task* creatByHand() {
    Task* head = (Task*)malloc(TASK_SIZE);
    head->userFloor = 1;
    head->destination = 1;
    head->pre = NULL;
    head->next = NULL;
    Task* p = head;
    int callTime, userFloor, destination;
    printf("Please enter the call time, current floor, and target floor respectively\n");
    printf("The three are separated by a space,a value of 0 for all three indicates the end\n");
    scanf("%d %d %d", &callTime, &userFloor, &destination);
    Task* q = NULL;
    int index = 0;
    while (userFloor != 0) {
        if (callTime == 0) {
            q = (Task*)malloc(TASK_SIZE);
            q->userFloor = userFloor;
            q->destination = destination;
            q->next = NULL;
            q->pre = p;
            p->next = q;
            p = q;
        }
        else {
            toBeDone[index][0] = callTime;
            toBeDone[index][1] = userFloor;
            toBeDone[index][2] = destination;
            index++;
        }
        scanf("%d %d %d", &callTime, &userFloor, &destination);
    }
    return head;
}

Task* creatByFile(FILE* fp,int* initialFloor) {
    Task* head = (Task*)malloc(TASK_SIZE);
    head->userFloor = 1;
    head->destination = 1;
    head->pre = NULL;
    head->next = NULL;
    Task* p = head;
    Task* q = NULL;
    char buffer[37];
    fscanf(fp, "%s %d", buffer, initialFloor);
    int callTime, userFloor, destination;
    int index = 0;
    while (!feof(fp)) {
        fscanf(fp, "%d %d %d", &callTime, &userFloor, &destination);
        if (callTime == 0) {
            q = (Task*)malloc(TASK_SIZE);
            q->userFloor = userFloor;
            q->destination = destination;
            q->next = NULL;
            q->pre = p;
            p->next = q;
            p = q;
        }
        else {
            toBeDone[index][0] = callTime;
            toBeDone[index][1] = userFloor;
            toBeDone[index][2] = destination;
            index++;
        }
    }
    return head;
}

void setDirection(Task* head, ElevatorState* elevatorState) {
    for (int i = 0; i < MAX_LOAD; i++) {
        if (elevatorState->userDestination[i] != 0) {
            if (elevatorState->floor - elevatorState->userDestination[i] > 0) {
                elevatorState->direction = -1;
            }
            else if (elevatorState->floor - elevatorState->userDestination[i] < 0) {
                elevatorState->direction = 1;
            }
            else {
                elevatorState->direction = 0;
            }
            return;
        }
    }
    if (head->next != NULL) {
        if (elevatorState->floor - head->next->userFloor > 0) {
            elevatorState->direction = -1;
        }
        else if (elevatorState->floor - head->next->userFloor < 0) {
            elevatorState->direction = 1;
        }
        else {
            elevatorState->direction = 0;
        }
    }
    else {
        elevatorState->direction = 0;
    }
}

void setDestination(Task* head, ElevatorState* elevatorState) {
    if (elevatorState->direction == 1) {
        for (int i = 0; i < MAX_LOAD; i++) {
            if (elevatorState->userDestination[i] > elevatorState->destination) {
                elevatorState->destination = elevatorState->userDestination[i];
            }
        }
        Task* p = head->next;
        while (p != NULL) {
            if (p->userFloor > elevatorState->destination) {
                elevatorState->destination = p->userFloor;
            }
            p = p->next;
        }
    }
    else if (elevatorState->direction == -1) {
        for (int i = 0; i < MAX_LOAD; i++) {
            if (elevatorState->userDestination[i] != 0) {
                if (elevatorState->userDestination[i] < elevatorState->destination) {
                    elevatorState->destination = elevatorState->userDestination[i];
                }
            }
        }
        Task* p = head->next;
        while (p != NULL) {
            if (p->userFloor < elevatorState->destination) {
                elevatorState->destination = p->userFloor;
            }
            p = p->next;
        }
    }
}

int ifExist(int array[MAX_LOAD], int target) {
    for (int i = 0; i < MAX_LOAD; i++) {
        if (array[i] == target) {
            return i;
        }
    }
    return -1;
}

int determineIfSyntropy(ElevatorState* elevatorState, Task* p) {
    if (elevatorState->direction == 1 && p->userFloor - p->destination < 0) {
        return 1;
    }
    else if (elevatorState->direction == -1 && p->userFloor - p->destination > 0) {
        return 1;
    }
    return 0;
}

void deleteNode(Task* target) {
    if(target->next!=NULL) {
        target->next->pre = target->pre;
    }
    target->pre->next = target->next;
}

void addTask(Task* head, int runTime) {
    Task* p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    for (int i = 0; i < 37; i++) {
        if (toBeDone[i][0] == runTime) {
            Task* q = (Task*)malloc(TASK_SIZE);
            q->userFloor = toBeDone[i][1];
            q->destination = toBeDone[i][2];
            q->pre = p;
            q->next = NULL;
            p->next = q;
            p = q;
            toBeDone[i][0] = 0;
        }
    }
}

void scheduleElevator(Task* head, ElevatorState* elevatorState) {
    int ifEmpty = 1;
    int ifEnd = 1;
    int runTime = 0;
    for (int i = 0; i < 37; i++) {
        if (toBeDone[i][0] != 0) {
            ifEnd = 0;
            break;
        }
    }
    while (!(ifEnd == 1 && ifEmpty == 1&&head->next == NULL)) {
        //The toBeDone is not empty or the elevator is not empty or task list is not empty
        int index = ifExist(elevatorState->userDestination, elevatorState->floor);
        while (index != -1) {
            //Someone needs to get off the elevator
            elevatorState->userDestination[index] = 0;//get off the elevator
            elevatorState->number--;//Number minus one
            index = ifExist(elevatorState->userDestination, elevatorState->floor);
        }
        Task* p = head->next;
        while (p != NULL) {
            if (p->userFloor == elevatorState->floor) {
                //Someone needs to get on the elevator
                int ifSyntropy = determineIfSyntropy(elevatorState, p);
                if (ifSyntropy == 1 || p->userFloor == elevatorState->destination) {
                    //The destination direction is in the same direction as the elevator 
                    //or the current floor is the destination floor
                    index = ifExist(elevatorState->userDestination, 0);
                    if (index == -1) {
                        //over load
                        printf("The elevator is overloaded on the %d floor\n", elevatorState->floor);
                    }
                    else {
                        elevatorState->number++;
                        elevatorState->userDestination[index] = p->destination;//get on the elevator
                        deleteNode(p);//delete task
                    }
                }
            }
            p = p->next;
        }
        printf("%d\t%d\t\t%d\n", runTime, elevatorState->floor, elevatorState->number);
        setDirection(head, elevatorState);
        setDestination(head, elevatorState);
        elevatorState->floor = elevatorState->floor + elevatorState->direction;//The elevator moves one floor in a given direction
        runTime++;
        addTask(head, runTime);
        for (int i = 0; i < MAX_LOAD; i++) {
            if (elevatorState->userDestination[i] != 0) {
                ifEmpty = 0;
                break;
            }
            ifEmpty = 1;
        }
        ifEnd = 1;
        for (int i = 0; i < 37; i++) {
            if (toBeDone[i][0] != 0) {
                ifEnd = 0;
                break;
            }
        }
    }
}

void main() {
    printf("Please enter a number to select the data entry method:\n");
    printf("0:manual input   1:file input\n");
    int method;
    scanf("%d", &method);
    ElevatorState elevatorState = { 1,0,0,1,{0,0,0,0} };
    Task* head = NULL;
    if (method == 0) {
        printf("Please enter the initial floor:");
        scanf("%d", &elevatorState.floor);
        elevatorState.destination = elevatorState.floor;
        head = creatByHand();
    }
    else if (method == 1) {
        FILE* fp;
        char file_name[120] = "test.txt";
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            fprintf(stderr, "File %s not found\n", file_name);
            exit(1);
        }
        head = creatByFile(fp,&elevatorState.floor);
        fclose(fp);
        elevatorState.destination = elevatorState.floor;
    }
    setDirection(head, &elevatorState);
    setDestination(head, &elevatorState);
    printf("runTime\tcurrentFloor\tcurrentNumber\n");
    scheduleElevator(head, &elevatorState);
}