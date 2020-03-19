package main

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"time"
)

// ioutil.ReadFile
func read0(path string) string {
	f, err := ioutil.ReadFile(path)
	if err != nil {
		panic(err)
	}
	return string(f)
}

// os.Open file.Seek file.Read
func read1(path string) string {
	fi, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer fi.Close()

	var chunks []byte
	buf := make([]byte, 4096)
	for {
		n, err := fi.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	return string(chunks)
}

// os.Open file.Seek bufio.NewReader
func read2(path string) string {
	fi, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer fi.Close()

	var chunks []byte
	buf := make([]byte, 4096)
	r := bufio.NewReader(fi)
	for {
		n, err := r.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	return string(chunks)
}

// os.Open file.Seek ioutil.ReadAll
func read3(path string) string {
	fi, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer fi.Close()
	fd, _ := ioutil.ReadAll(fi)
	return string(fd)
}

// edit from https://blog.csdn.net/wangshubo1989/article/details/74777112
func main() {

	file := "/proc/cpuinfo"
	res := ""

	start := time.Now()

	res = read0(file)
	t0 := time.Now()
	fmt.Printf("%v Cost time %v\n", len(res), t0.Sub(start))

	res = read1(file)
	t1 := time.Now()
	fmt.Printf("%v Cost time %v\n", len(res), t1.Sub(t0))

	res = read2(file)
	t2 := time.Now()
	fmt.Printf("%v Cost time %v\n", len(res), t2.Sub(t1))

	res = read3(file)
	t3 := time.Now()
	fmt.Printf("%v Cost time %v\n", len(res), t3.Sub(t2))

}
