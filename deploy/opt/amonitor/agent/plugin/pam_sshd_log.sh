#!/bin/bash
#########################################################################
# File Name: pam_sshd_log.sh
# Author: nian
# Blog: https://whoisnian.com
# Mail: zhuchangbao1998@gmail.com
# Created Time: 2020年05月13日 星期三 11时56分24秒
#########################################################################
go build -o pam_sshd_log.so -buildmode=c-shared ./plugin/pam_sshd_log
mv pam_sshd_log.so /lib/security/pam_sshd_log.so
chmod +x /lib/security/pam_sshd_log.so
echo "session   optional  pam_sshd_log.so     addr=127.0.0.1:8001" >> /etc/pam.d/sshd
