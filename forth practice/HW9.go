//NIMA BAHRAMI

package main

import (
	"bufio"
	"fmt"
	"net/http"
	"os"
	"regexp"
	"sync"
	"time"
	"io"
)

func main() {
	// Check if a command-line argument is provided
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run main.go <filename>")
		return
	}

	// Read the name of the file containing the list of URLs from the command-line argument
	filename := os.Args[1]

	// Open and read the contents of the file
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Failed to open file: %v\n", err)
		return
	}
	defer file.Close()

	// Store the URLs in a string slice
	urlList := []string{}
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		urlList = append(urlList, scanner.Text())
	}

	// Use a WaitGroup to wait for all goroutines to finish
	var wg sync.WaitGroup

	// Start a new goroutine for each URL in the list
	for _, url := range urlList {
		// Increment the WaitGroup counter
		wg.Add(1)

		// Define an anonymous function that takes the URL as an argument
		go func(url string) {
			// Decrement the WaitGroup counter when the goroutine is finished
			defer wg.Done()

			// Make an HTTP GET request to the URL
			response, err := http.Get(url)
			if err != nil {
				fmt.Printf("Failed to fetch URL %s: %v\n", url, err)
				return
			}
			defer response.Body.Close()

			// Extract the title from the response body using regular expressions
			titleRegex := regexp.MustCompile("<title>(.+?)</title>")
			body, err := io.ReadAll(response.Body)
			if err != nil {
				fmt.Printf("Failed to read URL body %s: %v\n", url, err)
				return
			}
			titleMatches := titleRegex.FindSubmatch(body)
			if len(titleMatches) > 1 {
				title := string(titleMatches[1])
				fmt.Printf("URL: %s\nTitle: %s\nLength: %d bytes\n", url, title, response.ContentLength)
			} else {
				fmt.Printf("URL: %s\nTitle: N/A\nLength: %d bytes\n", url, response.ContentLength)
			}
		}(url)
	
	}

	// Wait for all goroutines to finish
	wg.Wait()

	// Wait for 10 seconds
	fmt.Println("10 seconds wait")
	time.Sleep(10 * time.Second)
	fmt.Println("THE END.")
}
