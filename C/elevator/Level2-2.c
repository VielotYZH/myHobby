#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define LOWEST_FLOOR 1
#define HIGHEST_FLOOR 10
#define MAX_LOAD 4
#define TASK_SIZE sizeof(Task)

typedef struct ElevatorState
{
    int currentFloor;
    int currentNumber;
    int direction; //-1=down;1=up
    int destination;
} ElevatorState;
typedef struct Task
{
    int userFloor;
    int destination;
    struct Task *next;
    struct Task *pre;
} Task;

ElevatorState elevatorState1 = {0, 0, 0, 0};
ElevatorState elevatorState2 = {0, 0, 0, 0};
int toBeDone[10][3] = {{-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}};
int runTime = 0;

void show(int runTime, int number)
{
    if (number == 1)
    {
        printf("%d %d %d\n", elevatorState1.currentFloor, runTime, elevatorState1.currentNumber);
    }
    else if (number == 2)
    {
        printf("           %d %d %d\n", elevatorState2.currentFloor, runTime, elevatorState2.currentNumber);
    }
}

Task *exist(Task *head, int floor)
{
    Task *p = head->next;
    while (p != NULL)
    {
        if (p->userFloor == floor)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int hasOver(int arr[], int number)
{
    for (int i = 0; i < MAX_LOAD; i++)
    {
        if (arr[i] == number)
        {
            return i;
        }
    }
    return -1;
}

void delete(Task *head, int number)
{
    Task *p = head->next;
    if (p == NULL)
    {
        return;
    }
    while (p->destination != number)
    {
        p = p->next;
    }
    if (p->next != NULL)
    {
        p->pre->next = p->next;
        p->next->pre = p->pre;
    }
    else
    {
        p->pre->next = NULL;
    }
    free(p);
}

void addTask(int runTime, Task *head)
{
    for (int j = 0; j < 10; j++)
    {
        if (toBeDone[j][2] == runTime)
        {
            Task *p = head;
            while (p->next != NULL)
            {
                p = p->next;
            }
            Task *q = (Task *)malloc(TASK_SIZE);
            q->userFloor = toBeDone[j][0];
            q->destination = toBeDone[j][1];
            q->next = NULL;
            q->pre = p;
            p->next = q;
            toBeDone[j][0] = -1;
            toBeDone[j][1] = -1;
        }
    }
}

void sort(Task *head)
{
    Task *p;
    Task *q;
    int key, key1;
    if (head->next == NULL)
    {
        return;
    }
    p = head->next->next;
    while (p != NULL)
    {
        key = p->userFloor;
        key1 = p->destination;
        q = p->pre;
        while (q->destination != 0 && q->userFloor > key)
        {
            q->next->userFloor = q->userFloor;
            q->next->destination = q->destination;
            q = q->pre;
        }
        q->next->userFloor = key;
        q->next->destination = key1;
        p = p->next;
    }
}

bool isEmpty(int toBeDone[][3])
{
    for (int i = 0; i < 10; i++)
    {
        if (toBeDone[i][0] != -1)
        {
            return false;
        }
    }
    return true;
}

void moveTask(Task *node, Task *head_)
{
    Task *p = head_;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = node;
    if (node->next != NULL)
    {
        node->next->pre = node->pre;
    }
    node->pre->next = node->next;
    node->pre = p;
    node->next = NULL;
    sort(head_);
}

bool ifTake(ElevatorState state, Task *node, int userDestination_[])
{
    if (state.direction == 1)
    {
        if (node->userFloor - node->destination < 0)
        {
            if (state.currentFloor <= node->userFloor)
            {
                if (hasOver(userDestination_, 0) != -1)
                {
                    return true;
                }
                else if (hasOver(userDestination_, node->userFloor) != -1)
                {
                    return true;
                }
            }
        }
    }
    else if (state.direction == -1)
    {
        if (node->userFloor - node->destination > 0)
        {
            if (state.currentFloor >= node->userFloor)
            {
                if (hasOver(userDestination_, 0) != -1)
                {
                    return true;
                }
                else if (hasOver(userDestination_, node->userFloor) != -1)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int maxDestination(int userDestination[], Task *head_)
{
    Task *p = head_;
    while (p->next != NULL)
    {
        p = p->next;
    }
    int result = p->userFloor;
    for (int i = 0; i < MAX_LOAD; i++)
    {
        if (userDestination[i] > result)
        {
            result = userDestination[i];
        }
    }
    return result;
}

void runElevator(ElevatorState *state, Task *head_, int userDestination_[], int number)
{
    if (head_->next == NULL)
    {
        return;
    }
    if (state->direction == 0 && head_->next != NULL)
    {
        state->direction = (head_->next->userFloor - state->currentFloor > 0) ? 1 : -1;
    }
    else
    {
        state->direction = maxDestination(userDestination_, head_) - state->currentFloor > 0 ? 1 : -1;
    }
    state->destination = maxDestination(userDestination_, head_);

    state->currentFloor += state->direction;

    int arrive = hasOver(userDestination_, state->currentFloor);
    while (arrive != -1)
    {
        delete (head_, userDestination_[arrive]);
        userDestination_[arrive] = 0;
        state->currentNumber--;
        arrive = hasOver(userDestination_, state->currentFloor);
    }
    Task *p = exist(head_, state->currentFloor);
    int empty = hasOver(userDestination_, 0);
    if (p != NULL && empty != -1)
    {
        userDestination_[empty] = p->destination;
        state->currentNumber++;
        p->userFloor = 0;
    }
    show(runTime, number);
    if (head_->next == NULL)
    {
        state->direction = 0;
    }
}

void allocateTask(Task *head, Task *head1, Task *head2, int userDestination1[], int userDestination2[])
{
    Task *p = head->next;
    Task *q = NULL;
    while (p != NULL)
    {
        q = p->next;
        if (elevatorState1.direction == 0 && elevatorState2.direction == 0)
        {
            if (abs(elevatorState1.currentFloor - p->userFloor) > abs(elevatorState2.currentFloor - p->userFloor))
            {
                moveTask(p, head2);
            }
            else
            {
                moveTask(p, head1);
            }
        }
        else if (elevatorState1.direction == 0)
        {
            if (ifTake(elevatorState2, p, userDestination2))
            {
                moveTask(p, head2);
            }
            else
            {
                moveTask(p, head1);
            }
        }
        else if (elevatorState2.direction == 0)
        {
            if (ifTake(elevatorState1, p, userDestination1))
            {
                moveTask(p, head1);
            }
            else
            {
                moveTask(p, head2);
            }
        }
        else
        {
            if (ifTake(elevatorState2, p, userDestination2))
            {
                moveTask(p, head2);
            }
            else if (ifTake(elevatorState1, p, userDestination1))
            {
                moveTask(p, head1);
            }
        }
        p = q;
    }
}

void scheduleElevator(Task *head)
{
    int userDestination1[MAX_LOAD] = {0};
    int userDestination2[MAX_LOAD] = {0};
    Task *head1 = (Task *)malloc(TASK_SIZE);
    head1->next = NULL;
    head1->pre = NULL;
    Task *head2 = (Task *)malloc(TASK_SIZE);
    head2->next = NULL;
    head2->pre = NULL;
    while (!(isEmpty(toBeDone) && head->next == NULL && head1->next == NULL && head2->next == NULL))
    {
        allocateTask(head, head1, head2, userDestination1, userDestination2);
        runTime++;
        runElevator(&elevatorState1, head1, userDestination1, 1);
        runElevator(&elevatorState2, head2, userDestination2, 2);
        addTask(runTime, head);
    }
}

Task *creatByHand()
{
    Task *head = (Task *)malloc(TASK_SIZE);
    head->userFloor = 0;
    head->pre = NULL;
    head->destination = 0;
    head->next = NULL;
    Task *p = head;
    int i = 0;
    int temp;
    scanf("%d %d %d", &temp, &toBeDone[i][1], &toBeDone[i][2]);
    Task *q = NULL;
    while (temp != 0)
    {
        toBeDone[i][0] = temp;
        if (toBeDone[i][2] == 0)
        {
            q = (Task *)malloc(TASK_SIZE);
            q->userFloor = toBeDone[i][0];
            q->destination = toBeDone[i][1];
            q->next = NULL;
            q->pre = p;
            p->next = q;
            p = q;
            toBeDone[i][0] = -1;
        }
        i++;
        scanf("%d %d %d", &temp, &toBeDone[i][1], &toBeDone[i][2]);
    }
    return head;
}

Task *creatByFile(FILE *fp)
{
    Task *head = (Task *)malloc(TASK_SIZE);
    head->userFloor = 0;
    head->pre = NULL;
    head->destination = 0;
    head->next = NULL;
    Task *p = head;
    Task *q = NULL;
    char buffer[120];
    fgets(buffer, 120, fp);
    int buffer_size = 9;
    int i = 1;
    while (buffer[buffer_size] != ' ')
    {
        elevatorState1.currentFloor += (buffer[buffer_size] - '0') * i;
        buffer_size++;
        i *= 10;
    }
    buffer_size++;
    i = 1;
    while (buffer[buffer_size] != '\n' && buffer[buffer_size] != '\0')
    {
        elevatorState2.currentFloor += (buffer[buffer_size] - '0') * i;
        buffer_size++;
        i *= 10;
    }
    int j = 0;
    while (fgets(buffer, 120, fp) != NULL)
    {
        int i = 0;
        toBeDone[j][0] = 0;
        toBeDone[j][1] = 0;
        toBeDone[j][2] = 0;
        while (buffer[i] != ' ')
        {
            toBeDone[j][0] = toBeDone[j][0] * 10 + buffer[i++] - '0';
        }
        i++;
        while (buffer[i] != ' ')
        {
            toBeDone[j][1] = toBeDone[j][1] * 10 + buffer[i++] - '0';
        }
        i++;
        while (buffer[i] != '\n' && buffer[i] != '\0')
        {
            toBeDone[j][2] = toBeDone[j][2] * 10 + buffer[i++] - '0';
        }
        if (toBeDone[j][2] == 0)
        {
            q = (Task *)malloc(TASK_SIZE);
            q->userFloor = toBeDone[j][0];
            q->destination = toBeDone[j][1];
            q->next = NULL;
            q->pre = p;
            p->next = q;
            p = q;
            toBeDone[j][0] = -1;
        }
        j++;
    }
    return head;
}

void main()
{
    printf("Please enter a number to select the data entry method:\n");
    printf("0:by hand   1:by file\n");
    int a;
    scanf("%d", &a);
    Task *head = NULL;
    if (a == 0)
    {
        scanf("%d %d", &elevatorState1.currentFloor, &elevatorState2.currentFloor);
        head = creatByHand();
    }
    else if (a == 1)
    {
        FILE *fp;
        char file_name[120] = "test.txt";
        fp = fopen(file_name, "r");
        if (fp == NULL)
        {
            fprintf(stderr, "File %s not found\n", file_name);
            exit(1);
        }
        head = creatByFile(fp);
    }
    sort(head);
    scheduleElevator(head);
}