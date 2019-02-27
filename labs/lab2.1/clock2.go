package main

import (
	"io"
	"net"
	"time"
	"os"
)

func timeIn(name string) time.Time {
    loc, err := time.LoadLocation(name)

    if err != nil {
        panic(err)
    }
    return time.Now().In(loc)
}

func main() {
	args := os.Args[1:]
	timeZone:=string(os.Getenv("TZ"))
	puerto:=string(args[1])

	conn, err := net.Dial("tcp", "localhost:"+puerto)
	for {	
		conn, err = net.Dial("tcp", "localhost:"+puerto)
		if err != nil {
			time.Sleep(1 * time.Second)
			continue
		}else{
			break
		}
	
	}

	for {
		hora := timeIn(timeZone).Format("15:04:05")
		io.WriteString(conn, hora+"\n")
		time.Sleep(1 * time.Second)
	}
	conn.Close()
}