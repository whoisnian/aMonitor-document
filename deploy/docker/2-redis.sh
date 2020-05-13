#!/bin/bash
#########################################################################
# File Name: 2-redis.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 11时01分18秒
#########################################################################
docker run --net amonitor-net --ip 172.18.0.10 -d --name redis-monitor redis:rc-alpine
