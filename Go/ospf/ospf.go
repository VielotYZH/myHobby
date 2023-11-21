package main

import (
	ternaryExpression "Go/my-util"
	"fmt"
	"io"
	"os"
)

type LSP struct {
	neighbor int
	cost     int
}
type TC struct {
	destination int
	cost        int
	nextHop     int
}
type Node struct {
	number    int
	LSPList   []LSP
	tentative []TC
	confirmed []TC
}

func main() {
	f, err := os.Open("ospf/text.txt")
	if err != nil {
		fmt.Printf("file is not exist")
		os.Exit(1)
	}
	list := initNodeList(f)
	//createRoutineTable(list, &list[3])
	for _, p := range list {
		createRoutineTable(list, &p)
		q := p.confirmed
		fmt.Printf("-------confirmed of node %c-------\n", p.number+64)
		for _, qi := range q {
			fmt.Printf("(%c,%d,", qi.destination+64, qi.cost)
			if qi.nextHop == 0 {
				fmt.Printf("%c)\n", '-')
			} else {
				fmt.Printf("%c)\n", qi.nextHop+64)
			}
		}
	}
}

func createRoutineTable(list []Node, q *Node) {
	q.confirmed = append(q.confirmed, TC{q.number, 0, 0})
	cp := q.confirmed[len(q.confirmed)-1] //Always refers to the last node in the confirmed table
	for {
		r := locateLSPNode(list, cp.destination)
		for _, ri := range r {
			newCost := cp.cost + ri.cost
			s := ifExist(ri.neighbor, &q.tentative)
			if !(ifExist(ri.neighbor, &q.confirmed) != nil || s != nil) {
				q.tentative = append(q.tentative, TC{ri.neighbor, newCost, ternaryExpression.TernaryExpression(cp.nextHop == 0, ri.neighbor, cp.nextHop)})
			} else if s != nil && newCost < s.cost {
				s.cost = newCost
				s.nextHop = ternaryExpression.TernaryExpression(cp.nextHop == 0, ri.neighbor, cp.nextHop)
			}
		}
		if len(q.tentative) == 0 {
			break
		} else {
			moveTCNode(&q.confirmed, &q.tentative)
			cp = q.confirmed[len(q.confirmed)-1]
		}
	}
}

func moveTCNode(confirmed *[]TC, tentative *[]TC) {
	minCostIndex := 0
	for i, q := range *tentative {
		if q.cost < (*tentative)[minCostIndex].cost {
			minCostIndex = i
		}
	}
	*confirmed = append(*confirmed, TC{(*tentative)[minCostIndex].destination, (*tentative)[minCostIndex].cost, (*tentative)[minCostIndex].nextHop})
	*tentative = append((*tentative)[:minCostIndex], (*tentative)[minCostIndex+1:]...)
}

func ifExist(number int, list *[]TC) *TC {
	for i := 0; i < len(*list); i++ {
		if (*list)[i].destination == number {
			return &(*list)[i]
		}
	}
	return nil
}

func locateLSPNode(list []Node, number int) []LSP {
	for i := 0; i < len(list); i++ {
		if list[i].number == number {
			return list[i].LSPList
		}
	}
	return nil
}

func initNodeList(f *os.File) []Node {
	var number, neighbor, cost int
	list := make([]Node, 0)
	current := 0
	_, err := fmt.Fscan(f, &number, &neighbor, &cost)
	for err != io.EOF {
		if current != number {
			current = number
			var q Node
			q.number = number

			q.LSPList = make([]LSP, 1)
			q.LSPList[0].neighbor = neighbor
			q.LSPList[0].cost = cost

			q.tentative = make([]TC, 0)

			q.confirmed = make([]TC, 0)

			list = append(list, q)
		} else {
			list[len(list)-1].LSPList = append(list[len(list)-1].LSPList, LSP{neighbor, cost})
		}
		_, err = fmt.Fscan(f, &number, &neighbor, &cost)
	}
	return list
}
