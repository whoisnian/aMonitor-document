#!/bin/bash
#########################################################################
# File Name: file-md-log.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 11时55分10秒
#########################################################################
go build -o file-md-log ./plugin/file_MD_log/main.go
mv file-md-log /opt/amonitor/agent/plugin/file-md-log
