- [常用数据类型](#常用数据类型)
- [数据表](#数据表)
  - [agent](#agent)
  - [cpuinfo](#cpuinfo)
  - [meminfo](#meminfo)
  - [loadinfo](#loadinfo)
  - [netinfo](#netinfo)
  - [diskinfo](#diskinfo)
  - [mountinfo](#mountinfo)
  - [logininfo](#logininfo)
  - [filemdinfo](#filemdinfo)

## 常用数据类型
[postgresql dataType](https://www.postgresql.org/docs/current/datatype.html)

| 类型        | 长度         | 范围                                         |
| ----------- | ------------ | -------------------------------------------- |
| serial      | 4 bytes      | 1 to 2147483647                              |
| bigserial   | 8 bytes      | 1 to 9223372036854775807                     |
| smallint    | 2 bytes      | -32768 to +32767                             |
| integer     | 4 bytes      | -2147483648 to +2147483647                   |
| bigint      | 8 bytes      | -9223372036854775808 to +9223372036854775807 |
| boolean     | 1 byte       | 0 or 1                                       |
| timestamptz | 8 bytes      | 4713 BC to 294276 AD                         |
| varchar(n)  | n characters |                                              |
| text        | unlimited    |                                              |

## 数据表

### agent
| 字段       | 类型         | 备注                                   |
| ---------- | ------------ | -------------------------------------- |
| id         | SERIAL       | 自增主键                               |
| token      | VARCHAR(64)  | 身份标识符                             |
| distro     | VARCHAR(64)  | 发行版名称 (64 from `<sys/utsname.h>`) |
| kernel     | VARCHAR(64)  | 内核版本 (64 from `<sys/utsname.h>`)   |
| hostname   | VARCHAR(255) | 主机名 (255 from `man 7 hostname`)     |
| cpu model  | VARCHAR(255) | cpu型号                                |
| cpu cores  | SMALLINT     | cpu核心数                              |
| deleted    | BOOLEAN      | 逻辑删除标识                           |
| created_at | TIMESTAMPTZ  | 创建时间                               |
| updated_at | TIMESTAMPTZ  | 更新时间                               |

### cpuinfo
| 字段         | 类型        | 备注                |
| ------------ | ----------- | ------------------- |
| id           | BIGSERIAL   | 自增主键            |
| agent_id     | INTEGER     | 外键                |
| used_percent | SMALLINT    | 使用率 (n% * 10000) |
| time         | TIMESTAMPTZ | 数据采集时间        |

### meminfo
| 字段              | 类型        | 备注                        |
| ----------------- | ----------- | --------------------------- |
| id                | BIGSERIAL   | 自增主键                    |
| agent_id          | INTEGER     | 外键                        |
| ram_total         | INTEGER     | 总内存 (KB)                 |
| ram_cached        | INTEGER     | 缓冲内存 (KB)               |
| ram_used          | INTEGER     | 已用内存 (KB)               |
| ram_free          | INTEGER     | 空闲内存 (KB)               |
| ram_avail         | INTEGER     | 可用内存 (KB)               |
| ram_used_percent  | SMALLINT    | 内存使用率 (n% * 10000)     |
| swap_total        | INTEGER     | 总交换空间 (KB)             |
| swap_used         | INTEGER     | 已用交换空间 (KB)           |
| swap_free         | INTEGER     | 空闲交换空间 (KB)           |
| swap_used_percent | SMALLINT    | 交换空间使用率 (n% * 10000) |
| time              | TIMESTAMPTZ | 数据采集时间                |

### loadinfo
| 字段     | 类型        | 备注                       |
| -------- | ----------- | -------------------------- |
| id       | BIGSERIAL   | 自增主键                   |
| agent_id | INTEGER     | 外键                       |
| avg1     | SMALLINT    | 一分钟平均负载 (n * 100)   |
| avg5     | SMALLINT    | 五分钟平均负载 (n * 100)   |
| avg15    | SMALLINT    | 十五分钟平均负载 (n * 100) |
| time     | TIMESTAMPTZ | 数据采集时间               |

### netinfo
| 字段          | 类型        | 备注                        |
| ------------- | ----------- | --------------------------- |
| id            | BIGSERIAL   | 自增主键                    |
| agent_id      | INTEGER     | 外键                        |
| receive_rate  | INTEGER     | 下载速度 (bytes per second) |
| receive_sum   | BIGINT      | 下载量 (bytes)              |
| transmit_rate | INTEGER     | 上传速度 (bytes per second) |
| transmit_sum  | BIGINT      | 上传量 (bytes)              |
| time          | TIMESTAMPTZ | 数据采集时间                |

### diskinfo
| 字段       | 类型        | 备注             |
| ---------- | ----------- | ---------------- |
| id         | BIGSERIAL   | 自增主键         |
| agent_id   | INTEGER     | 外键             |
| read req   | INTEGER     | 每秒读取请求次数 |
| write req  | INTEGER     | 每秒写入请求次数 |
| read size  | BIGINT      | 读取总量 (KB)    |
| write size | BIGINT      | 写入总量 (KB)    |
| time       | TIMESTAMPTZ | 数据采集时间     |

### mountinfo
| 字段               | 类型          | 备注                                      |
| ------------------ | ------------- | ----------------------------------------- |
| id                 | BIGSERIAL     | 自增主键                                  |
| agent_id           | INTEGER       | 外键                                      |
| dev_name           | VARCHAR(255)  | 挂载设备                                  |
| mount_point        | VARCHAR(4096) | 挂载点路径 (4096 from `<linux/limits.h>`) |
| fs_type            | VARCHAR(64)   | 文件系统类型                              |
| total_size         | BIGINT        | 总大小 (KB)                               |
| free_size          | BIGINT        | 空闲大小 (KB)                             |
| avail_size         | BIGINT        | 可用大小 (KB)                             |
| used_size_percent  | SMALLINT      | 挂载点使用率 (n% * 10000)                 |
| total_nodes        | BIGINT        | 总node                                    |
| free_nodes         | BIGINT        | 空闲node                                  |
| used_nodes_percent | SMALLINT      | node使用率 (n% * 10000)                   |
| time               | TIMESTAMPTZ   | 数据采集时间                              |

### logininfo
| 字段        | 类型        | 备注                             |
| ----------- | ----------- | -------------------------------- |
| id          | BIGSERIAL   | 自增主键                         |
| agent_id    | INTEGER     | 外键                             |
| username    | VARCHAR(32) | 用户名 (32 from `man 8 useradd`) |
| remote_host | VARCHAR(64) | 远程主机地址                     |
| auth_method | TEXT        | 使用的认证方式                   |
| time        | TIMESTAMPTZ | 数据采集时间                     |

### filemdinfo
| 字段     | 类型          | 备注                                    |
| -------- | ------------- | --------------------------------------- |
| id       | BIGSERIAL     | 自增主键                                |
| agent_id | INTEGER       | 外键                                    |
| path     | VARCHAR(4096) | 文件路径 (4096 from `<linux/limits.h>`) |
| event    | VARCHAR(16)   | 事件类型 (create, modify, delete, move) |
| time     | TIMESTAMPTZ   | 数据采集时间                            |