package main

import (
	"fmt"
	"io"
	"os"
	"strconv"
)

const INFINITY int = 1024

type RoutingInfo struct {
	destination int
	cost        int
	nextHop     int
}

var ifChanged = true

func initRoutingTable(f *os.File) [][]RoutingInfo {
	var nodeNum int
	_, err := fmt.Fscanf(f, "%d", &nodeNum)
	if err != nil {
		fmt.Printf("read file false")
		os.Exit(1)
	}
	routingTable := make([][]RoutingInfo, nodeNum)
	for i := 0; i < nodeNum; i++ {
		for j := 0; j < nodeNum; j++ {
			var routingInfo RoutingInfo
			routingTable[i] = append(routingTable[i], routingInfo)
		}
	}
	var token string
	_, err = fmt.Fscan(f, &token)
	if err != nil {
		fmt.Printf("read file false")
		os.Exit(1)
	}
	for i := 0; i < nodeNum; i++ {
		for j := 0; j < nodeNum; j++ {
			temp, err := strconv.Atoi(token)
			if err == nil {
				routingTable[i][j].cost = temp
			} else {
				if token == "i" {
					routingTable[i][j].cost = INFINITY
				}
			}
			routingTable[i][j].destination = j
			if routingTable[i][j].cost == INFINITY || routingTable[i][j].cost == 0 {
				routingTable[i][j].nextHop = len(routingTable)
			} else {
				routingTable[i][j].nextHop = j
			}
			_, err = fmt.Fscan(f, &token)
			if err == io.EOF {
			} else if err != nil {
				fmt.Printf("read file false")
				os.Exit(1)
			}
		}
	}
	return routingTable
}

func isAdjacent(routingTable [][]RoutingInfo, nodeNum1 int, nodeNum2 int) bool {
	for i := 0; i < len(routingTable); i++ {
		if routingTable[nodeNum1][i].nextHop == nodeNum2 {
			return true
		}
	}
	return false
}

func receiveRoutingTable(routingTable [][]RoutingInfo, senderNum int, receicerNum int, senderInfo []int) {
	for i := 0; i < len(routingTable); i++ {
		if routingTable[receicerNum][i].cost > routingTable[receicerNum][senderNum].cost+senderInfo[i] {
			routingTable[receicerNum][i].cost = routingTable[receicerNum][senderNum].cost + senderInfo[i]
			routingTable[receicerNum][i].nextHop = senderNum
			ifChanged = true
		}
	}
}

func sendRoutingTable(routingTable [][]RoutingInfo) {
	nodeNum := len(routingTable)
	senderInfos := make([][]int, nodeNum)
	for i := 0; i < nodeNum; i++ {
		for j := 0; j < nodeNum; j++ {
			senderInfos[i] = append(senderInfos[i], routingTable[i][j].cost)
		}
	}
	for i := 0; i < nodeNum; i++ {
		for j := 0; j < nodeNum; j++ {
			if isAdjacent(routingTable, i, j) {
				receiveRoutingTable(routingTable, i, j, senderInfos[i])
			}
		}
	}
}

func main() {
	f, err := os.Open("rip/test.txt")
	if err != nil {
		fmt.Printf("file error")
		os.Exit(1)
	}
	routingTable := initRoutingTable(f)
	nodeNum := len(routingTable)
	for ifChanged {
		ifChanged = false
		sendRoutingTable(routingTable)
		for i := 0; i < nodeNum; i++ {
			for j := 0; j < nodeNum; j++ {
				if routingTable[i][j].cost == INFINITY {
					fmt.Printf("%s ", "*")
				} else {
					fmt.Printf("%d ", routingTable[i][j].cost)
				}
			}
			fmt.Println()
		}
		fmt.Println("---------------------------------------")
	}
}
