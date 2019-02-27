package main

import (
	"log"
	"net"
	"fmt"
	"os"
	"time"
	"strings"
	"bufio"
)

func handleConn(city string, c net.Conn) {
	defer c.Close()
	for {
		timeoutDuration := 5 * time.Second
		bufReader := bufio.NewReader(c)
		for {
			c.SetReadDeadline(time.Now().Add(timeoutDuration))	
			bytes, err := bufReader.ReadBytes('\n')
			if err != nil {
				fmt.Println(err)
				return
			}
			fmt.Printf("%s: %s", city, bytes)
		}
		fmt.Println()
		c.Close()
	}
}

func main() {
	args := os.Args[1:]
	first:=0
	for {
		time.Sleep(1 * time.Second)
		if(first==0){
			first=1
			for index,element := range args{
				index=index
				tempList:=strings.Split(element,"=")
				

				l, err := net.Listen("tcp", tempList[1])
				if err != nil {
					log.Fatal(err)
				}

				defer l.Close()

				conn, err := l.Accept()
				if err != nil {
					log.Fatal(err)
				}		
				go handleConn(tempList[0], conn)
			}
		}
	}
}

