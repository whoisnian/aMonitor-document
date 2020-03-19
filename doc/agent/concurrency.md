### gorilla/websocket

* https://pkg.go.dev/github.com/gorilla/websocket?tab=doc#hdr-Concurrency
* https://github.com/gorilla/websocket/issues/369#issuecomment-377728313

单个websocket连接不能在多个goroutine中并发读写；不同websocket连接相互独立，读写无影响。