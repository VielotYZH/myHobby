#include <stdio.h>

#define LOWEST_FLOOR 1
#define HIGHEST_FLOOR 10
#define MAX_LOAD 4

typedef struct ElevatorState
{
    int currentFloor;
    int currentNumber;
    int direction; //-1=down;1=up
} ElevatorState;

ElevatorState elevatorState1 = {1, 0, 1};

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
    int A, B, C;
    scanf("%d%d%d", &A, &B, &C);
    scheduleElevator(A, B, C);
}
