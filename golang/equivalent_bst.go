package main

import "golang.org/x/tour/tree"
import "fmt"
import "context"

// Walk walks the tree t sending all values
// from the tree to the channel ch.
func Walk(ctx context.Context, t *tree.Tree, ch chan int) {
	if t == nil {
		return
	}
	Walk(ctx, t.Left, ch)
	select {
		case ch <- t.Value:
		  ;
		case <-ctx.Done():
		  return		
	}
	Walk(ctx, t.Right, ch)
}

// Same determines whether the trees
// t1 and t2 contain the same values.
func Same(t1, t2 *tree.Tree) bool {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	
	ch1 := make(chan int, 1)
	ch2 := make(chan int, 1)
	go func() {
		Walk(ctx, t1, ch1)
		close(ch1)
	}()
	go func() {
		Walk(ctx, t2, ch2)
		close(ch2)
	}()
	
	// BEWARE: Assumes equal length t1/t2
	for v1 := range ch1 {
		v2, ok := <-ch2
		if !ok {
			// got a v1 but not a v2, trees are unequal in length
			return false
		}
		if v1 != v2 {
			return false
		}
	}
	_, ok := <-ch2
	if ok {
		// got a v2 but not a v1, trees are unequal in length
		return false
	}
	return true
}

func testCase(k1, k2 int, expect bool) {
	t1 := tree.New(k1)
	t2 := tree.New(k2)
	if v := Same(t1, t2); v != expect {
		fmt.Println("[FAILED]: k1 =", k1, "k2 =", k2, "got", v, "want", expect)
	} else {
		fmt.Println("[PASSED]: k1 =", k1, "k2 =", k2, "got", v, "want", expect)
	}
}

func main() {	
	testCase(2, 2, true)
	testCase(1, 2, false)
}

