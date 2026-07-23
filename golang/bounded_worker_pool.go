//
// Write a function called ProcessJobs that takes a slice of job IDs (integers) and a maximum concurrency limit:
// func ProcessJobs(jobs []int, maxConcurrency int) map[int]int
// 
// Requirements:
// 1. Each job takes an integer input x and produces a result calculated by calling heavyWork(x) 
//    (you can mock heavyWork(x) simply as return x * 2 with a small time.Sleep).
// 2. You must process all jobs concurrently, but at no point should there be more than maxConcurrency goroutines actively processing jobs simultaneously.
// 3. The function must return a map[int]int mapping jobID -> result.
// 4. Your function must be completely safe from race conditions and deadlocks, and must clean up any goroutines/channels before returning.
//

package main

import (
	"fmt"
	"maps"
	"math/rand"
	"time"
)

type intPair struct {
	input int
	result int
}

// heavyWork is a mock heavy work function.
func heavyWork(in int) int {
	// Random number between 100 and 200
	val := 100 + rand.Intn(100)
	time.Sleep(time.Duration(val) * time.Millisecond)
	return in*2;
}

func performJob(in int, ch chan intPair) {
	result := heavyWork(in)
	ch <- intPair{in, result}
}

func ProcessJobs(jobs []int, maxConcurrency int) map[int]int {
	ret := make(map[int]int)
	if len(jobs) == 0 || maxConcurrency <= 0 {
		return ret
	}

	// results arrive on this channel
	results := make(chan intPair)

	// if there is a token, we launch a job.
	// When a job completes we put a token back.
	tokens := make(chan int, maxConcurrency)
	for i:= 0; i<maxConcurrency; i++ {
		tokens <- 0
	}

	j := 0     // the next job to launch
	done := 0  // counts done jobs
	for done < len(jobs) {
		select {
		case intp := <- results:			
			ret[intp.input] = intp.result
			done++
			tokens <- 0
		case <- tokens: // Not ideal: will run out of tokens at the end, but that's OK			
			if j < len(jobs) {
				go performJob(jobs[j], results)
				j++
			} 
		}
	}
	close(results)
	close(tokens)

	return ret
}

func message(conc int, results, expect map[int]int) {
	if !maps.Equal(expect, results) {
		fmt.Println("[FAILED] Concurrency ", conc)
	} else {
		fmt.Println("[PASSED] Concurrency ", conc)
	}
}

func main() {
	// Test cases to check
	jobs := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	expect := map[int]int {
		1: 2, 2: 4, 3: 6, 4: 8, 5: 10, 6: 12, 7: 14, 8: 16, 9: 18, 10: 20,
	}
	// Test concurrency 2.
	results := ProcessJobs(jobs, 2)
	message(2, results, expect)	

	// Test concurrency 1.
	results = ProcessJobs(jobs, 1)
	message(1, results, expect)

	// Test concurrency 10.
	results = ProcessJobs(jobs, 10)
	message(10, results, expect)

	// Test concurrency 100.
	results = ProcessJobs(jobs, 100)
	message(100, results, expect)
	
}