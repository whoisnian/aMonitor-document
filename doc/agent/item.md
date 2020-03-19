* basic
  * distro (read '/etc/os-release')
  * kernel (read '/proc/sys/kernel/osrelease')
  * hostname (read '/proc/sys/kernel/hostname')
  * cpu model (read '/proc/cpuinfo')
  * cpu cores (read '/proc/cpuinfo')
* cpu (read '/proc/stat')
  * used percent (n% * 10000)
* mem (read '/proc/meminfo')
  * ram total (KB)
  * ram cached (KB)
  * ram used (KB)
  * ram free (KB)
  * ram avail (KB)
  * ram used percent (n% * 10000)
  * swap total (KB)
  * swap used (KB)
  * swap free (KB)
  * swap used percent (n% * 10000)
* load (read '/proc/loadavg')
  * avg1
  * avg5
  * avg15
* net (read '/sys/class/net/' and '/proc/net/dev')
  * receive rate (bytes per second)
  * receive sum (bytes)
  * transmit rate (bytes per second)
  * transmit sum (bytes)
* disk R/W (read '/proc/diskstats' and https://github.com/torvalds/linux/blob/master/Documentation/ABI/testing/procfs-diskstats)
  * read req per second
  * write req per second
  * read size (KB)
  * write size (KB)
* mounts usage (read '/proc/mounts' and syscall 'statfs')
  * dev name
  * mount point
  * filesystem type
  * total size (KB)
  * free size (KB)
  * avail size (KB)
  * used size percent (n% * 10000)
  * total nodes
  * free nodes
  * used nodes percent (n% * 10000)
* login (write PAM module http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_MWG.html)
  * username
  * remote host
  * auth method
* database query (TODO)
* network latency (TODO)
