package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"math/big"
)

func blink(stones []string) []string {
	output := []string{}

	for _, stone := range stones {
		stone := strings.TrimLeft(stone, "0")

		if stone == "" {
			output = append(output, "1")
		} else if len(stone) % 2 == 0 {
			mid := len(stone) / 2
			output = append(output, stone[:mid])
			output = append(output, stone[mid:])
		} else {
			n := new(big.Int)
			m := new(big.Int)
			n.SetString(stone, 10)
			m.SetString("2024", 10)

			result := new(big.Int).Mul(n, m)
			output = append(output, result.String())
		}
	}
	return output
}


func main() {
	filePath := "input.txt"

	file, err := os.Open(filePath)
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	var words []string

	for scanner.Scan() {
		line := scanner.Text()
		tokens := strings.Fields(line)
		words = append(words, tokens...)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		return
	}
	
	//words = []string{"125", "17"}
	
	fmt.Println("words:", words)

	for i := 1; i <= 25; i++ {
		words = blink(words)
		fmt.Println("words:", words)
	}

	answer1 := len(words)
	fmt.Println("Answer Part 1:", answer1)
}
