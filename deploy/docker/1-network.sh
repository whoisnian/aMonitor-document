#!/bin/bash
#########################################################################
# File Name: network.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 10时58分04秒
#########################################################################
docker network create --subnet=172.18.0.0/24 amonitor-net
