package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Println("Please enter a number to select the data entry method:")
	fmt.Println("0:manual input   1:file input")
	elevatorState := ElevatorState{number: 0, direction: 0, userDestination: [MAX_LOAD]int{0, 0, 0, 0}}
	var tasks []map[string]int
	var method int
	_, err := fmt.Scan(&method)
	if err != nil {
		return
	}
	if method == 0 {
		tasks = CreateByHand(&elevatorState)
	} else if method == 1 {
		f, err := os.Open("elevator/test.txt")
		if err != nil {
			fmt.Printf("file error")
			os.Exit(1)
		}
		defer func(f *os.File) {
			err := f.Close()
			if err != nil {
				fmt.Printf("close error")
				os.Exit(1)
			}
		}(f)
		tasks = CreateByFile(f, &elevatorState)
		elevatorState.destination = elevatorState.floor
	}
	elevatorState.SetDirection(tasks)
	elevatorState.SetDestination(tasks)
	fmt.Println("runTime\tcurrentFloor\tcurrentNumber")
	elevatorState.ScheduleElevator(tasks)
}
