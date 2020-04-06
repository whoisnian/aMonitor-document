### agent理想启动流程

* 从source下载配置文件和可执行文件
* 启动agent
* 获取配置文件
* 检查本地有无token，无token时register向storage发请求进行首次注册
* sender初始化，开始监听消息缓冲区
* collector更新主机基础信息
* collector创建监控任务
* sender崩溃后向source请求新的配置文件，重启sender
* 消息缓冲区阻塞后报警

### 数据格式
| 名称      | 类型    | 说明     |
| --------- | ------- | -------- |
| Category  | string  | 数据类型 |
| MetaData  | object  | 元数据   |
| Timestamp | integer | 时间戳   |

```json
{
  "Category": "cpuInfo",
  "MetaData": {
    "UsedPCT": 950
  },
  "Timestamp": 1583331658
}
```

### 插件上报格式
POST http://127.0.0.1:8001?category=sshdInfo
```json
{
  "Username": "root",
  "RemoteHost": "127.0.0.1",
  "AuthInfo": "password"
}
```