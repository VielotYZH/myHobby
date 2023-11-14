package main

import (
	"fmt"
	"math"
)

var length, number int

func main() {
	gray := initial()
	encode(gray)
	fmt.Printf("Number\tGray\tBinary\n")
	for i := 0; i < number; i++ {
		fmt.Printf("%d\t%s\t%s\n", i, string(gray[i]), string(decimalToBinary(i)))
	}
}

func initial() [][]byte {
	fmt.Printf("Please enter the length of the gray code you want:")
	_, err := fmt.Scanf("%d", &length)
	if err != nil {
		return nil
	}
	number = int(math.Pow(2, float64(length)))
	gray := make([][]byte, number)
	for i := 0; i < number; i++ {
		gray[i] = make([]byte, length)
		for j := 0; j < length; j++ {
			gray[i][j] = '0'
		}
	}
	return gray
}

func encode(gray [][]byte) {
	for i := 1; i <= length; i++ {
		singleOperate(i, gray)
	}
}

func singleOperate(digitalNumber int, gray [][]byte) {
	if digitalNumber == 1 {
		gray[1][length-1] = '1'
	} else {
		operateRange := int(math.Pow(2, float64(digitalNumber)))
		for i := 0; i < operateRange/2; i++ {
			for j := length - 1; j > length-digitalNumber; j-- {
				gray[operateRange-1-i][j] = gray[i][j]
				gray[operateRange-1-i][length-digitalNumber] = '1'
			}
		}
	}
}

func decimalToBinary(decimal int) []byte {
	binary := make([]byte, length)
	for i := 0; i < length; i++ {
		binary[i] = '0'
	}
	index := length - 1
	for decimal > 1 {
		quotient := decimal / 2
		if decimal-quotient*2 == 1 {
			binary[index] = '1'
		} else {
			binary[index] = '0'
		}
		index--
		decimal = quotient
	}
	if decimal == 1 {
		binary[index] = '1'
	} else {
		binary[index] = '0'
	}
	return binary
}
