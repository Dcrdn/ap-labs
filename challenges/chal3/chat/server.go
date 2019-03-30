// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strings"
	"os"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

var usuarios []string=[]string{}
var m map[string]string=map[string]string{}
var m2 map[string]net.Conn=map[string]net.Conn{}


//!+handleConn
func handleConn(conn net.Conn, usuario string) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	who := conn.RemoteAddr().String()
	usuarios=append(usuarios, usuario)

	m[usuario]=who
	m2[who]=conn

	ch <- "You are " + usuario+"\n"

	nm:= strings.TrimSuffix(usuario, "\n")

	messages <- nm + " has arrived \n"
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		if(strings.Compare(input.Text(), "/users") == 0){
			tmp:=""
			for index, element := range usuarios {
				if(index==len(usuarios)-1){
					break
				}
				nombre:= strings.TrimSuffix(element, "\n")

				tmp+=nombre+","
			}
			tmp+=usuarios[len(usuarios)-1]
			ch <- "irc-server >" + tmp
	
		}else if(strings.Compare(strings.Split(input.Text()," ")[0], "/user") == 0){
			target:=strings.Split(input.Text()," ")[1]+"\n"
			
			if _, ok := m[target]; ok {
				ipTarget:=m[target]
				ch <- "IP: " + ipTarget+"  Username: "+ target
			}else{
				ch <- "irc-server > User not found"
			}
	
		}else if(strings.Compare(strings.Split(input.Text()," ")[0], "/time") == 0){
			t:=time.Now()
			ch <- "Time: " + t.Format("2006-01-02 15:04:05")
	
		}else if(strings.Compare(strings.Split(input.Text()," ")[0], "/msg") == 0){
			target:=strings.Split(input.Text()," ")[1]+"\n"
			mensaje:=strings.Split(input.Text()," ")[2:]

			if _, ok := m[target]; ok {
				tmp:=""
				for _, element := range mensaje {
					tmp+=element+" "
				}
				
				conection:=m2[m[target]]
				fmt.Fprintln(conection, tmp) 
			}else{
				ch <- "irc-server > User not found"
			}
			
		}else{
			nombre:= strings.TrimSuffix(usuario, "\n")

			messages <- nombre + ": " + input.Text()+"\n"
		} 
	}
	leaving <- ch
	delete(m, usuario)
	delete(m2, who)
	var temp[]string

	for _, element := range usuarios {
		if(strings.Compare(element, usuario) != 0){
			temp=append(temp, element)
		}
	}
	usuarios=temp
	messages <- who + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	argsWithoutProg := os.Args[1:]
	//-host localhost -port 9000
	host:= argsWithoutProg[1]
	port:=argsWithoutProg[3]
	
	listener, err := net.Listen("tcp", host+":"+port)
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()
	for {
		conn, err := listener.Accept()
		message, _ := bufio.NewReader(conn).ReadString('\n')
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn, string(message))
	}
}

//!-main
