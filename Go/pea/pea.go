package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"
)

var maxPageTypes = 5
var pageFramesNumber = 3
var pagesNumber = 10000

func randomlyGeneratePages(arr []byte) {
	for i := 0; i < len(arr); i++ {
		arr[i] = byte(rand.Intn(maxPageTypes) + 65)
	}
}

func contains(arr []byte, start int, end int, element byte) int {
	for i := start; i < end; i++ {
		if arr[i] == element {
			return i
		}
	}
	return -1
}

func printPageFrames(arr []byte) {
	for i := 0; i < len(arr); i++ {
		fmt.Printf("%c\t\t", arr[i])
	}
}

func controlTime() func() ([]int, bool) {
	times := make([]int, pageFramesNumber)
	hasInit := false
	return func() ([]int, bool) {
		if !hasInit {
			for i := 0; i < pageFramesNumber; i++ {
				times[i] = -1
			}
			hasInit = true
		}
		return times, hasInit
	}
}

func OPT(pages []byte, currentIndex int, pageFrames []byte) bool {
	if contains(pageFrames, 0, len(pageFrames), pages[currentIndex]) == -1 {
		if availableIndex := contains(pageFrames, 0, len(pageFrames), 0); availableIndex != -1 {
			pageFrames[availableIndex] = pages[currentIndex]
		} else {
			targetIndex, maxIndex := 0, 0
			for i := 0; i < len(pageFrames); i++ {
				tempIndex := contains(pages, currentIndex+1, len(pages), pageFrames[i])
				if tempIndex == -1 {
					targetIndex = i
					break
				} else if tempIndex > maxIndex {
					maxIndex = tempIndex
					targetIndex = i
				}
			}
			pageFrames[targetIndex] = pages[currentIndex]
		}
		return true
	}
	return false
}

func FIFO(page byte, pageFrames []byte, p func() ([]int, bool)) bool {
	times, _ := p()
	missingPage := false
	if contains(pageFrames, 0, pageFramesNumber, page) == -1 {
		missingPage = true
		if availableIndex := contains(pageFrames, 0, pageFramesNumber, 0); availableIndex != -1 {
			pageFrames[availableIndex] = page
			times[availableIndex] = 0
		} else {
			targetIndex, i := 0, 1
			for ; i < pageFramesNumber; i++ {
				if times[i] > times[targetIndex] {
					targetIndex = i
				}
			}
			pageFrames[targetIndex] = page
			times[targetIndex] = 0
		}
	}
	for i := 0; i < pageFramesNumber; i++ {
		if times[i] != -1 {
			times[i]++
		}
	}
	return missingPage
}

func LRU(page byte, pageFrames []byte, p func() ([]int, bool)) bool {
	times, _ := p()
	missingPage := false
	if index := contains(pageFrames, 0, pageFramesNumber, page); index == -1 {
		missingPage = true
		if availableIndex := contains(pageFrames, 0, pageFramesNumber, 0); availableIndex != -1 {
			pageFrames[availableIndex] = page
			times[availableIndex] = 0
		} else {
			targetIndex, i := 0, 1
			for ; i < pageFramesNumber; i++ {
				if times[i] > times[targetIndex] {
					targetIndex = i
				}
			}
			pageFrames[targetIndex] = page
			times[targetIndex] = 0
		}
	} else {
		times[index] = 0
	}
	for i := 0; i < pageFramesNumber; i++ {
		if times[i] != -1 {
			times[i]++
		}
	}
	return missingPage
}

func LFU(page byte, pageFrames []byte, p func() ([]int, bool)) bool {
	times, _ := p()
	if index := contains(pageFrames, 0, pageFramesNumber, page); index == -1 {
		if availableIndex := contains(pageFrames, 0, pageFramesNumber, 0); availableIndex != -1 {
			pageFrames[availableIndex] = page
			times[availableIndex]++
		} else {
			targetIndex := 0
			for i := 0; i < pageFramesNumber; i++ {
				if times[i] < times[targetIndex] {
					targetIndex = i
				}
			}
			pageFrames[targetIndex] = page
			for i := 0; i < pageFramesNumber; i++ {
				times[i] = 0
			}
		}
		return true
	} else {
		times[index]++
		return false
	}
}

func runOPT(p func([]byte, int, []byte) bool, printProcedure bool, pages []byte, c chan int) int {
	missingPageNumber := 0
	pageFrames := make([]byte, pageFramesNumber)
	for i := 0; i < pageFramesNumber; i++ {
		pageFrames[i] = 0
	}
	if printProcedure {
		for i := 0; i < pageFramesNumber; i++ {
			fmt.Printf("pageFramed%d\t", i+1)
		}
		fmt.Println("pageMissing")
	}
	for i := 0; i < len(pages); i++ {
		missingPage := p(pages, i, pageFrames)
		if missingPage {
			missingPageNumber++
		}
		if printProcedure {
			printPageFrames(pageFrames)
			if missingPage {
				fmt.Print("*")
			}
			fmt.Println()
		}
	}
	/*fmt.Printf("OPT missing page number:%d\n", missingPageNumber)
	fmt.Printf("OPT page fault rate:%.2f%%\n", float64(missingPageNumber)/float64(len(pages))*100)
	fmt.Print("\n\n")*/
	c <- missingPageNumber
	return missingPageNumber
}

func runFunction(p func(byte, []byte, func() ([]int, bool)) bool, functionName string, printProcedure bool, pages []byte, c chan int) int {
	missingPageNumber := 0
	pageFrames := make([]byte, pageFramesNumber)
	for i := 0; i < pageFramesNumber; i++ {
		pageFrames[i] = 0
	}
	if printProcedure {
		for i := 0; i < pageFramesNumber; i++ {
			fmt.Printf("pageFramed%d\t", i+1)
		}
		fmt.Println("pageMissing")
	}
	f := controlTime()
	for i := 0; i < len(pages); i++ {
		missingPage := p(pages[i], pageFrames, f)
		if missingPage {
			missingPageNumber++
		}
		if printProcedure {
			printPageFrames(pageFrames)
			if missingPage {
				fmt.Print("*")
			}
			fmt.Println()
		}
	}
	/*fmt.Printf("%s missing page number:%d\n", functionName, missingPageNumber)
	fmt.Printf("%s page fault rate:%.2f%%\n", functionName, float64(missingPageNumber)/float64(len(pages))*100)
	fmt.Print("\n\n")*/
	c <- missingPageNumber
	return missingPageNumber
}

func testOPT(p func([]byte, int, []byte) bool, c chan int) {
	var arr = []byte{'A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E'}

	runOPT(p, true, arr, c)

	os.Exit(0)
}

func test(p func(byte, []byte, func() ([]int, bool)) bool, functionName string, c chan int) {
	var arr = []byte{'A', 'B', 'C', 'D', 'A', 'B', 'E', 'A', 'B', 'C', 'D', 'E'}

	runFunction(p, functionName, true, arr, c)

	os.Exit(0)
}

func main() {
	/*testOPT(OPT)
	test(FIFO, "FIFO")
	test(LRU, "LRU")
	test(LFU, "LFU")*/

	pages := make([]byte, pagesNumber)
	randomlyGeneratePages(pages)

	c := make(chan int, 1)

	go runOPT(OPT, false, pages, c)

	go runFunction(FIFO, "FIFO", false, pages, c)

	go runFunction(LRU, "LRU", false, pages, c)

	go runFunction(LFU, "LFU", false, pages, c)

	fmt.Println(<-c, <-c, <-c, <-c)

	time.Sleep(1 * time.Second)

	/*runOPT(OPT, false, pages)

	runFunction(FIFO, "FIFO", false, pages)

	runFunction(LRU, "LRU", false, pages)

	runFunction(LFU, "LFU", false, pages)*/
}
