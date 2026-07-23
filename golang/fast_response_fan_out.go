package main

import (
	"context"
	"fmt"
	"math/rand"
	"sync"
	"time"
)

//
// Fast-Response Fan-Out with Cancellation
// Scenario:
// Imagine you are building a search aggregator (like a flight scanner or DNS resolver).
// You query multiple backend servers concurrently, but you only need the fastest successful response.
// Once you get one valid response, you want to immediately cancel all other in-flight backend requests to save CPU and bandwidth.
//
// Task:
// Implement the following function:
// func FetchFirstSuccess(ctx context.Context, urls []string, fetch func(ctx context.Context, url string) (string, error)) (string, error)
//
// Requirements:
// 1. Launch a request for each URL in urls concurrently.
// 2. Return the response string from the first worker that succeeds (returns err == nil).
// 3. As soon as one worker succeeds, cancel the contexts of all other in-flight workers immediately.
// 4. If all workers fail (or if urls is empty), return an error indicating all fetches failed.
// 5. If the parent ctx is cancelled or times out before any worker succeeds, return ctx.Err().
// 6. No Goroutine Leaks: Guarantee that all spawned goroutines exit cleanly before FetchFirstSuccess returns, under all conditions (success, failure, or context cancellation).
//

type urlResult struct {
	result string
	err    error
}

func fetchFunc(ctx context.Context, url string) (string, error) {
	scale := rand.Intn(10)
	val := 100 + rand.Intn(1+500*scale)

	// Simulates an actual url fetch
	select {
	case <-time.After(time.Duration(val) * time.Millisecond):
		// fetched the url
	case <-ctx.Done():
		fmt.Printf("Worker early cancellation: %v\n", url)
		return "", ctx.Err()
	}

	// 10% failure chance
	val2 := rand.Intn(10)
	if val2 == 9 {
		return "", fmt.Errorf("Worker failed to fetch url %v in time %v", url, val)
	}
	return fmt.Sprintf("Worker fetched url %v in time %v", url, val), nil
}

func FetchFirstSuccess(ctx context.Context, urls []string, fetch func(ctx context.Context, url string) (string, error)) (string, error) {
	ctx, cancel := context.WithCancel(ctx)
	resultCh := make(chan urlResult, len(urls))

	var wg sync.WaitGroup
	defer wg.Wait() // main thread waits for all tries to succeed, fail, or timeout or get cancelled
	defer cancel()

	wg.Add(len(urls))
	for _, url := range urls {
		go func() {
			res, err := fetch(ctx, url)
			resultCh <- urlResult{result: res, err: err}
			wg.Done()
		}()
	}

	// Counts errors
	nerrs := 0
	done := false
	rets := ""
	for !done {
		select {
		case res := <-resultCh:
			if res.err != nil {
				nerrs += 1
				fmt.Printf("Received error %v\n", res.err)
				if nerrs == len(urls) {
					// Tried all mirrors unsuccessfully
					fmt.Printf("All mirrors failed!\n")
					done = true
				}
			} else {
				done = true
				fmt.Printf("Earliest result received: %v\n", res.result)
				rets = res.result
			}
		case <-ctx.Done():
			fmt.Printf("Done received from parent, exiting!\n")
			return "", ctx.Err()
		}
	}

	if nerrs == len(urls) {
		return "", fmt.Errorf("All mirrors failed!")
	}
	return rets, nil
}

func main() {
	// Create test data.
	urls := []string{
		"mirror1.url.com",
		"mirror2.url.com",
		"mirror3.url.com",
		"mirror4.url.com",
		"mirror5.url.com",
	}

	// Test cases: Try 10 times first.
	for i := 0; i < 10; i++ {
		fmt.Printf("--------------------------------------------------------------------------\n")
		result, err := FetchFirstSuccess(context.Background(), urls, fetchFunc)
		fmt.Printf("[TC%v] Got result %v and error %v\n", i, result, err)
		fmt.Printf("--------------------------------------------------------------------------\n")
	}

	// Try a test case with a super short cancellation from outside.

	func() {
		fmt.Printf("--------------------------------------------------------------------------\n")
		ctx, cancel := context.WithTimeout(context.Background(), 50*time.Millisecond)
		defer cancel()
		result, err := FetchFirstSuccess(ctx, urls, fetchFunc)
		fmt.Printf("[TC-timeout] Got result %v and error %v\n", result, err)
		fmt.Printf("--------------------------------------------------------------------------\n")
	}()

}
