package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"gopl.io/ch5/links"
)

// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

var seen = make(map[string]bool)

func crawling(depth int, url string, done chan bool){
	if(depth==0){
		done <-true
		return
	}
	if visited, ok := seen[url]; visited && ok {
		done <- true
		return
	} else {
		seen[url] = true
	}

	links := crawl(url)
	linksDone := make(chan bool)

	for _, link := range links {
		go crawling(depth-1, link, linksDone)
		<-linksDone
	}
	done <- true
}

func main() {
	parseDepth := strings.Split(os.Args[1], "=")
	depth, err := strconv.Atoi(parseDepth[1]) 
	if err != nil {
		log.Fatal("Incorrect parameters")
	}
	url := os.Args[2]
	done :=make(chan bool);

	seen[url] =false

	go crawling(depth, url, done)
	
	<-done
}