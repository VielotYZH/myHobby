package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
)

const n int = 3

func readFile(f *os.File) []byte {
	line := bufio.NewReader(f)
	source, err := line.ReadBytes('\n')
	if err != nil && err != io.EOF {
		panic(err)
	}
	return source
}

func encode(source []byte) []byte {
	var result = make([]byte, 0)
	counter := 0
	last := 1
	lastV := 1
	for i := 0; i < len(source); i++ {
		if source[i] == '1' {
			counter = 0
			if last == 1 {
				result = append(result, 'n')
			} else {
				result = append(result, '1')
			}
			last = -last
		} else if source[i] == '0' {
			counter++
			result = append(result, '0')
			if counter == n+1 {
				counter = 0
				if lastV == 1 {
					result[i] = 'n'
				} else {
					result[i] = '1'
				}
				lastV = -lastV
				if result[i-n-1] != result[i] {
					result[i-n] = result[i]
				}
			}
		}
	}
	return result
}

func decode(coding []byte) []byte {
	result := make([]byte, 0)
	last := 1
	for i := 0; i < len(coding); i++ {
		if coding[i] == 'n' {
			result = append(result, '1')
			if last == -1 {
				for j := i; j >= i-n; j-- {
					result[j] = '0'
				}
			} else {
				last = -last
			}
		} else if coding[i] == '1' {
			result = append(result, '1')
			if last == 1 {
				for j := i; j >= i-n; j-- {
					result[j] = '0'
				}
			} else {
				last = -last
			}
		} else if coding[i] == '0' {
			result = append(result, '0')
		}
	}
	return result
}

func main() {
	f, err := os.Open("HDBn/test.txt")
	if err != nil {
		fmt.Printf("file is not exist")
		os.Exit(1)
	}
	source := readFile(f)
	coding := encode(source)
	decoding := decode(coding)
	fmt.Println("source:  ", string(source))
	fmt.Println("coding:  ", string(coding))
	fmt.Println("decoding:", string(decoding))
}
