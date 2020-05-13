- [常用数据类型](#常用数据类型)
- [监控数据表](#监控数据表)
  - [agents](#agents)
  - [cpuinfos](#cpuinfos)
  - [meminfos](#meminfos)
  - [loadinfos](#loadinfos)
  - [netinfos](#netinfos)
  - [diskinfos](#diskinfos)
  - [mountinfos](#mountinfos)
  - [sshdinfos](#sshdinfos)
  - [filemdinfos](#filemdinfos)
- [通用数据表](#通用数据表)
  - [users](#users)
  - [groups](#groups)
  - [rules](#rules)
  - [agent_group](#agent_group)
  - [receivers](#receivers)
  - [receiver_group](#receiver_group)
  - [messages](#messages)
  - [preferences](#preferences)

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

## 监控数据表

### agents
| 字段       | 类型         | 备注                                   |
| ---------- | ------------ | -------------------------------------- |
| id         | SERIAL       | 自增主键                               |
| token      | VARCHAR(64)  | 身份标识符                             |
| distro     | VARCHAR(64)  | 发行版名称 (64 from `<sys/utsname.h>`) |
| kernel     | VARCHAR(64)  | 内核版本 (64 from `<sys/utsname.h>`)   |
| hostname   | VARCHAR(255) | 主机名 (255 from `man 7 hostname`)     |
| cpu_model  | VARCHAR(255) | cpu型号                                |
| cpu_cores  | SMALLINT     | cpu核心数                              |
| deleted    | BOOLEAN      | 逻辑删除标识                           |
| created_at | TIMESTAMPTZ  | 创建时间                               |
| updated_at | TIMESTAMPTZ  | 更新时间                               |

### cpuinfos
| 字段         | 类型        | 备注                |
| ------------ | ----------- | ------------------- |
| id           | BIGSERIAL   | 自增主键            |
| agent_id     | INTEGER     | 外键                |
| used_percent | SMALLINT    | 使用率 (n% * 10000) |
| time         | TIMESTAMPTZ | 数据采集时间        |

### meminfos
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

### loadinfos
| 字段     | 类型        | 备注                       |
| -------- | ----------- | -------------------------- |
| id       | BIGSERIAL   | 自增主键                   |
| agent_id | INTEGER     | 外键                       |
| avg1     | SMALLINT    | 一分钟平均负载 (n * 100)   |
| avg5     | SMALLINT    | 五分钟平均负载 (n * 100)   |
| avg15    | SMALLINT    | 十五分钟平均负载 (n * 100) |
| time     | TIMESTAMPTZ | 数据采集时间               |

### netinfos
| 字段          | 类型        | 备注                        |
| ------------- | ----------- | --------------------------- |
| id            | BIGSERIAL   | 自增主键                    |
| agent_id      | INTEGER     | 外键                        |
| receive_rate  | INTEGER     | 下载速度 (bytes per second) |
| receive_sum   | BIGINT      | 下载量 (bytes)              |
| transmit_rate | INTEGER     | 上传速度 (bytes per second) |
| transmit_sum  | BIGINT      | 上传量 (bytes)              |
| time          | TIMESTAMPTZ | 数据采集时间                |

### diskinfos
| 字段       | 类型        | 备注             |
| ---------- | ----------- | ---------------- |
| id         | BIGSERIAL   | 自增主键         |
| agent_id   | INTEGER     | 外键             |
| read_req   | INTEGER     | 每秒读取请求次数 |
| write_req  | INTEGER     | 每秒写入请求次数 |
| read_size  | BIGINT      | 读取总量 (KB)    |
| write_size | BIGINT      | 写入总量 (KB)    |
| time       | TIMESTAMPTZ | 数据采集时间     |

### mountinfos
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

### sshdinfos
| 字段        | 类型        | 备注                             |
| ----------- | ----------- | -------------------------------- |
| id          | BIGSERIAL   | 自增主键                         |
| agent_id    | INTEGER     | 外键                             |
| username    | VARCHAR(32) | 用户名 (32 from `man 8 useradd`) |
| remote_host | VARCHAR(64) | 远程主机地址                     |
| auth_method | TEXT        | 使用的认证方式                   |
| time        | TIMESTAMPTZ | 数据采集时间                     |

### filemdinfos
| 字段     | 类型          | 备注                                    |
| -------- | ------------- | --------------------------------------- |
| id       | BIGSERIAL     | 自增主键                                |
| agent_id | INTEGER       | 外键                                    |
| path     | VARCHAR(4096) | 文件路径 (4096 from `<linux/limits.h>`) |
| event    | VARCHAR(16)   | 事件类型 (create, modify, delete, move) |
| time     | TIMESTAMPTZ   | 数据采集时间                            |

## 通用数据表

### users
| 字段       | 类型         | 备注                |
| ---------- | ------------ | ------------------- |
| id         | SERIAL       | 自增主键            |
| email      | VARCHAR(255) | E-mail地址 (unique) |
| username   | VARCHAR(255) | 用户名              |
| password   | VARCHAR(255) | 密码 (哈希)         |
| deleted    | BOOLEAN      | 逻辑删除标识        |
| created_at | TIMESTAMPTZ  | 创建时间            |
| updated_at | TIMESTAMPTZ  | 更新时间            |

### groups
| 字段       | 类型         | 备注         |
| ---------- | ------------ | ------------ |
| id         | SERIAL       | 自增主键     |
| name       | VARCHAR(255) | 规则组名称   |
| deleted    | BOOLEAN      | 逻辑删除标识 |
| created_at | TIMESTAMPTZ  | 创建时间     |
| updated_at | TIMESTAMPTZ  | 更新时间     |

### rules
| 字段       | 类型          | 备注                    |
| ---------- | ------------- | ----------------------- |
| id         | SERIAL        | 自增主键                |
| name       | VARCHAR(255)  | 规则名称                |
| target     | VARCHAR(255)  | 目标 (cpu, mem, load等) |
| addition   | VARCHAR(4096) | 目标额外信息            |
| event      | VARCHAR(255)  | 触发事件                |
| threshold  | INTEGER       | 阈值                    |
| interval   | INTEGER       | 聚合间隔 (second)       |
| level      | VARCHAR(64)   | 等级                    |
| group_id   | INTEGER       | 所属规则组 (外键)       |
| deleted    | BOOLEAN       | 逻辑删除标识            |
| created_at | TIMESTAMPTZ   | 创建时间                |
| updated_at | TIMESTAMPTZ   | 更新时间                |

### agent_group
| 字段       | 类型        | 备注            |
| ---------- | ----------- | --------------- |
| id         | SERIAL      | 自增主键        |
| agent_id   | INTEGER     | 对应主机 (外键) |
| group_id   | INTEGER     | 对应规则 (外键) |
| created_at | TIMESTAMPTZ | 创建时间        |
| updated_at | TIMESTAMPTZ | 更新时间        |

### receivers
| 字段       | 类型          | 备注         |
| ---------- | ------------- | ------------ |
| id         | SERIAL        | 自增主键     |
| name       | VARCHAR(255)  | 接收者名称   |
| type       | VARCHAR(64)   | 接收者类型   |
| addr       | VARCHAR(4096) | 接收者地址   |
| token      | VARCHAR(255)  | 访问凭据     |
| deleted    | BOOLEAN       | 逻辑删除标识 |
| created_at | TIMESTAMPTZ   | 创建时间     |
| updated_at | TIMESTAMPTZ   | 更新时间     |

### receiver_group
| 字段        | 类型        | 备注              |
| ----------- | ----------- | ----------------- |
| id          | SERIAL      | 自增主键          |
| receiver_id | INTEGER     | 对应接收者 (外键) |
| group_id    | INTEGER     | 对应规则组 (外键) |
| created_at  | TIMESTAMPTZ | 创建时间          |
| updated_at  | TIMESTAMPTZ | 更新时间          |

### messages
| 字段       | 类型        | 备注              |
| ---------- | ----------- | ----------------- |
| id         | SERIAL      | 自增主键          |
| content    | TEXT        | 消息内容          |
| agent_id   | INTEGER     | 对应主机 (外键)   |
| rule_id    | INTEGER     | 对应规则 (外键)   |
| group_id   | INTEGER     | 对应规则组 (外键) |
| level      | VARCHAR(64) | 等级              |
| deleted    | BOOLEAN     | 逻辑删除标识      |
| created_at | TIMESTAMPTZ | 创建时间          |
| updated_at | TIMESTAMPTZ | 更新时间          |

### preferences
| 字段  | 类型         | 备注     |
| ----- | ------------ | -------- |
| id    | SERIAL       | 自增主键 |
| key   | VARCHAR(255) | 配置项键 |
| value | VARCHAR(255) | 配置项值 |
