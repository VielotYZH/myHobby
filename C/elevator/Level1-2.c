#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOWEST_FLOOR 1
#define HIGHEST_FLOOR 10
#define MAX_LOAD 4

typedef struct ElevatorState
{
    int currentFloor;
    int currentNumber;
    int direction; //-1=down;1=up
} ElevatorState;

ElevatorState elevatorState1 = {1, 0, 0};

void show(int runTime)
{
    printf("%d %d %d\n", elevatorState1.currentFloor, runTime, elevatorState1.currentNumber);
}

void scheduleElevator(int wait, int destination, int currentFloor)
{
    int runTime = 0;

    elevatorState1.currentFloor = currentFloor;
    show(runTime);

    elevatorState1.direction = elevatorState1.currentFloor - wait >= 0 ? -1 : 1;

    while (elevatorState1.currentFloor != wait)
    {
        runTime++;
        elevatorState1.currentFloor += elevatorState1.direction;
        show(runTime);
    }                               // 跳出这个循环后，电梯到达用户等待楼层
    elevatorState1.currentNumber++; // 用户上电梯
    show(runTime);

    elevatorState1.direction = elevatorState1.currentFloor - destination >= 0 ? -1 : 1;

    while (elevatorState1.currentFloor != destination)
    {
        runTime++;
        elevatorState1.currentFloor += elevatorState1.direction;
        show(runTime);
    }                               // 跳出这个循环后，电梯到达用户目标楼层
    elevatorState1.currentNumber--; // 用户出电梯
    show(runTime);
}

void main()
{
    FILE *fp;
    char file_name[120] = "test.txt";
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "File %s not found\n", file_name);
        exit(1);
    }
    int A = 0, B = 0, C = 0;
    char buffer[120];
    if (fgets(buffer, 120, fp))
    {
        int buffer_size = 9;
        int i = 1;
        while (buffer[buffer_size] != '\n'&& buffer[buffer_size] != '\0')
        {
            C += (buffer[buffer_size] - '0') * i;
            buffer_size++;
            i *= 10;
        }
    }
    if (fgets(buffer, 120, fp))
    {
        int i = 0;
        while (buffer[i] != ' ')
        {
            A = A * 10 + buffer[i++] - '0';
        }
        i++;
        while (buffer[i] != '\n'&&buffer[i]!='\0')
        {
            B = B * 10 + buffer[i++] - '0';
        }
    }
    scheduleElevator(A, B, C);
}