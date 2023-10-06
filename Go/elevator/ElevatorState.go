package main

import "fmt"

const MAX_LOAD int = 4

type ElevatorState struct {
	floor           int
	number          int
	direction       int
	destination     int
	userDestination [MAX_LOAD]int
}

func (elevatorState *ElevatorState) SetDirection(tasks []map[string]int) {
	for i := 0; i < MAX_LOAD; i++ {
		if elevatorState.userDestination[i] != 0 {
			if elevatorState.floor-elevatorState.userDestination[i] > 0 {
				elevatorState.direction = -1
			} else if elevatorState.floor-elevatorState.userDestination[i] < 0 {
				elevatorState.direction = 1
			} else {
				elevatorState.direction = 0
			}
			return
		}
	}
	if len(tasks) > 0 {
		if elevatorState.floor-tasks[0]["userFloor"] > 0 {
			elevatorState.direction = -1
		} else if elevatorState.floor-tasks[0]["userFloor"] < 0 {
			elevatorState.direction = 1
		} else {
			//It is not certain that this will happen
			elevatorState.direction = 0
		}
	} else {
		elevatorState.direction = 0
	}
}

func (elevatorState *ElevatorState) SetDestination(tasks []map[string]int) {
	if elevatorState.direction == 1 {
		for i := 0; i < MAX_LOAD; i++ {
			if elevatorState.userDestination[i] > elevatorState.destination {
				elevatorState.destination = elevatorState.userDestination[i]
			}
		}
		for _, task := range tasks {
			if task["userFloor"] > elevatorState.destination {
				elevatorState.destination = task["userFloor"]
			}
		}
	} else if elevatorState.direction == -1 {
		for i := 0; i < MAX_LOAD; i++ {
			if elevatorState.userDestination[i] != 0 {
				if elevatorState.userDestination[i] < elevatorState.destination {
					elevatorState.destination = elevatorState.userDestination[i]
				}
			}
		}
		for _, task := range tasks {
			if task["userFloor"] < elevatorState.destination {
				elevatorState.destination = task["userFloor"]
			}
		}
	}
}

func (elevatorState *ElevatorState) ScheduleElevator(tasks []map[string]int) {
	isEmpty := true
	runTime := 0
	for !(len(toBeDones) == 0 && isEmpty && len(tasks) == 0) {
		index := IsExist(elevatorState.userDestination, elevatorState.floor)
		for index != -1 {
			elevatorState.userDestination[index] = 0
			elevatorState.number--
			index = IsExist(elevatorState.userDestination, elevatorState.floor)
		}
		for i := 0; i < len(tasks); i++ {
			if tasks[i]["userFloor"] == elevatorState.floor {
				if elevatorState.determineIfSyntropy(tasks[i]) || tasks[i]["userFloor"] == elevatorState.destination {
					index = IsExist(elevatorState.userDestination, 0)
					if index == -1 {
						fmt.Printf("The elevator is overloaded on the %d floor\n", elevatorState.floor)
					} else {
						elevatorState.number++
						elevatorState.userDestination[index] = tasks[i]["destination"]
						tasks = DeleteSlice(tasks, i)
						i--
					}
				}
			}
		}
		fmt.Println(runTime, "\t", elevatorState.floor, "\t\t", elevatorState.number)
		elevatorState.SetDirection(tasks)
		elevatorState.SetDestination(tasks)
		elevatorState.floor += elevatorState.direction
		runTime++
		tasks = AddTask(tasks, runTime)
		for i := 0; i < MAX_LOAD; i++ {
			if elevatorState.userDestination[i] != 0 {
				isEmpty = false
				break
			}
			isEmpty = true
		}
	}
}

func (elevatorState ElevatorState) determineIfSyntropy(task map[string]int) bool {
	if elevatorState.direction == 1 && task["userFloor"]-task["destination"] < 0 {
		return true
	} else {
		return elevatorState.direction == -1 && task["userFloor"]-task["destination"] > 0
	}
}
