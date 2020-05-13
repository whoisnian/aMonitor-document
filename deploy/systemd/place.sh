#!/bin/bash
#########################################################################
# File Name: place.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 11时33分36秒
#########################################################################
cp amonitor-storage.service /lib/systemd/system/amonitor-storage.service
cp amonitor-agent.service /lib/systemd/system/amonitor-agent.service
cp file-md-log.service.service /lib/systemd/system/file-md-log.service.service