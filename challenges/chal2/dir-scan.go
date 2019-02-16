package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func scanDir(dir string) error {
	subDirToSkip:="skip"
	directories:=0
	symbolic:=0
	other:=0
	err:= filepath.Walk(dir, func(path string, info os.FileInfo, err error) error{
		if err != nil{
			fmt.Println("error")
			return err
		}
		if info.IsDir() && info.Name()==subDirToSkip{
			fmt.Println("skipping dir")
			return filepath.SkipDir
		}
		fi, err :=os.Lstat(path)
		switch mode:= fi.Mode();{
			case mode.IsRegular():
				other++
			case mode.IsDir():
				directories++
			case mode&os.ModeSymlink!=0:
				symbolic++
			default:
				other++
		}
		return nil
	})
	if err !=nil{
		fmt.Println("error walking in the path")
		return nil
	}
	fmt.Println("Directory Scanner Tool")
	fmt.Println("----------------------")
	fmt.Printf("Path: %v\n", dir)
	fmt.Printf("Directories: %v\n", directories)
	fmt.Printf("Symbolic Links: %v\n", symbolic)
	fmt.Printf("Other Files: %v\n", other)
	fmt.Println("----------------------")

	return nil
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
