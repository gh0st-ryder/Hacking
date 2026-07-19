package main

// A toy web crawler example from a tour of go.
// Makes the web crawl happen in parallel.
// Makes sure we don't crawl already crawled pages.

import (
	"fmt"
	"sync"
)

type Fetcher interface {
	// Fetch returns the body of URL and
	// a slice of URLs found on that page.
	Fetch(url string) (body string, urls []string, err error)
}

// PCrawler is a parallel crawler.
type PCrawler struct {
	mu sync.Mutex
	cache map[string]bool
}

func NewPCrawler() *PCrawler {
	return &PCrawler {
		cache: make(map[string]bool),
	}
}

func (pc *PCrawler) shouldFetch(url string) bool {
	pc.mu.Lock()
	defer pc.mu.Unlock()
	_, ok := pc.cache[url]
	if !ok {
		// Not found in the cache, put it in there and return yes
		pc.cache[url] = true
		return true
	}
	return false
}

// Crawl uses fetcher to recursively crawl
// pages starting with url, to a maximum of depth.
func (pc *PCrawler) Crawl(url string, depth int, fetcher Fetcher, wg *sync.WaitGroup) {
	defer wg.Done()
	
	// Done: Fetch URLs in parallel.
	// Done: Don't fetch the same URL twice.
	// This implementation doesn't do either:
	if depth <= 0 {
		return
	}
	if !pc.shouldFetch(url) {
		return
	}
	
	body, urls, err := fetcher.Fetch(url)
	if err != nil {
		fmt.Println(err)
		return
	}
	
	fmt.Printf("found: %s %q\n", url, body)
	for _, u := range urls {
		wg.Add(1)
		go pc.Crawl(u, depth-1, fetcher, wg)
	}
	return
}

func main() {
	var wg sync.WaitGroup
	pc := NewPCrawler()
	
	wg.Add(1)
	go pc.Crawl("https://golang.org/", 4, fetcher, &wg)
	wg.Wait()	
}

// fakeFetcher is Fetcher that returns canned results.
type fakeFetcher map[string]*fakeResult

type fakeResult struct {
	body string
	urls []string
}

func (f fakeFetcher) Fetch(url string) (string, []string, error) {
	if res, ok := f[url]; ok {
		return res.body, res.urls, nil
	}
	return "", nil, fmt.Errorf("not found: %s", url)
}

// fetcher is a populated fakeFetcher.
var fetcher = fakeFetcher{
	"https://golang.org/": &fakeResult{
		"The Go Programming Language",
		[]string{
			"https://golang.org/pkg/",
			"https://golang.org/cmd/",
		},
	},
	"https://golang.org/pkg/": &fakeResult{
		"Packages",
		[]string{
			"https://golang.org/",
			"https://golang.org/cmd/",
			"https://golang.org/pkg/fmt/",
			"https://golang.org/pkg/os/",
		},
	},
	"https://golang.org/pkg/fmt/": &fakeResult{
		"Package fmt",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
	"https://golang.org/pkg/os/": &fakeResult{
		"Package os",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
}

