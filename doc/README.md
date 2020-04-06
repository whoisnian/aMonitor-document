# 通用

## git commit message
| 前缀     | 含义         |
| -------- | ------------ |
| feat     | 增加新功能   |
| fix      | 漏洞修复     |
| docs     | 完善文档     |
| style    | 整理代码格式 |
| refactor | 系统结构重构 |
| test     | 增加测试     |
| chore    | 杂务         |

## lines of code
go get -u github.com/boyter/scc

* aMonitor-agent:  
  `scc .`
* aMonitor-storage:  
  `scc --exclude-dir package-lock.json .`