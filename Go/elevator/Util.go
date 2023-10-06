package main

import (
	"fmt"
	"io"
	"os"
)

var toBeDones []map[string]int

func CreateByHand(elevatorState *ElevatorState) []map[string]int {
	fmt.Printf("Please enter the initial floor:")
	_, err := fmt.Scan(&elevatorState.floor)
	if err != nil {
		return nil
	}
	elevatorState.destination = elevatorState.floor
	fmt.Println("Please enter the call time, current floor, and target floor respectively")
	fmt.Println("The three are separated by a space,a value of 0 for all three indicates the end")
	var tasks []map[string]int
	var callTime, userFloor, destination int
	_, err = fmt.Scan(&callTime, &userFloor, &destination)
	if err != nil {
		return nil
	}
	for userFloor != 0 {
		if callTime == 0 {
			task := map[string]int{"userFloor": userFloor, "destination": destination}
			tasks = append(tasks, task)
		} else {
			toBeDone := map[string]int{"callTime": callTime, "userFloor": userFloor, "destination": destination}
			toBeDones = append(toBeDones, toBeDone)
		}
		_, err = fmt.Scan(&callTime, &userFloor, &destination)
		if err != nil {
			return nil
		}
	}
	return tasks
}

func CreateByFile(f *os.File, elevatorState *ElevatorState) []map[string]int {
	var buffer string
	_, err := fmt.Fscan(f, &buffer, &elevatorState.floor)
	if err != nil {
		return nil
	}
	var callTime, userFloor, destination int
	var tasks []map[string]int
	_, err = fmt.Fscan(f, &callTime, &userFloor, &destination)
	for err != io.EOF {
		if callTime == 0 {
			task := map[string]int{"userFloor": userFloor, "destination": destination}
			tasks = append(tasks, task)
		} else {
			toBeDone := map[string]int{"callTime": callTime, "userFloor": userFloor, "destination": destination}
			toBeDones = append(toBeDones, toBeDone)
		}
		_, err = fmt.Fscan(f, &callTime, &userFloor, &destination)
	}
	return tasks
}

func IsExist(array [MAX_LOAD]int, target int) int {
	for i := 0; i < MAX_LOAD; i++ {
		if array[i] == target {
			return i
		}
	}
	return -1
}

func DeleteSlice(slice []map[string]int, index int) []map[string]int {
	for i := 0; i < len(slice); i++ {
		if i == index {
			slice = append(slice[:i], slice[i+1:]...)
			break
		}
	}
	return slice
}

func AddTask(tasks []map[string]int, runTime int) []map[string]int {
	for i := 0; i < len(toBeDones); i++ {
		if toBeDones[i]["callTime"] == runTime {
			task := map[string]int{"userFloor": toBeDones[i]["userFloor"], "destination": toBeDones[i]["destination"]}
			tasks = append(tasks, task)
			toBeDones = DeleteSlice(toBeDones, i)
			i--
		}
	}
	return tasks
}
