#!/bin/bash
#########################################################################
# File Name: 3-timescaledb.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 11时02分06秒
#########################################################################
docker run --net amonitor-net --ip 172.18.0.20 -d --name timescaledb-monitor -e POSTGRES_PASSWORD=password timescale/timescaledb:latest-pg10
