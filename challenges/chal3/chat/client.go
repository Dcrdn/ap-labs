// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
)

//!+
func main() {
	argsWithoutProg := os.Args[1:]
	//-user user1 -server localhost:9000
	name:= argsWithoutProg[1]
	host:=argsWithoutProg[3]

	conn, err := net.Dial("tcp", host)
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})
	go func() {
		fmt.Fprintf(conn,  name+"\n")
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
