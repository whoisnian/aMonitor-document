# Linux服务器监控告警系统的设计与实现 <!-- omit in toc -->
- [1. 绪论](#1-绪论)
  - [1.1 课题研究背景](#11-课题研究背景)
  - [1.2 课题研究意义](#12-课题研究意义)
  - [1.3 国内外现状](#13-国内外现状)
  - [1.4 论文研究内容](#14-论文研究内容)
  - [1.5 论文的组织结构](#15-论文的组织结构)
- [2. 关键技术介绍](#2-关键技术介绍)
  - [2.1 Golang](#21-golang)
  - [2.2 Node.js](#22-nodejs)
  - [2.3 Express](#23-express)
  - [2.4 React](#24-react)
  - [2.5 TimescaleDB](#25-timescaledb)
  - [2.6 本章小结](#26-本章小结)
- [3. 需求分析](#3-需求分析)
  - [3.1 系统概述](#31-系统概述)
  - [3.2 功能性需求分析](#32-功能性需求分析)
    - [3.2.1 数据采集](#321-数据采集)
    - [3.2.2 数据存储](#322-数据存储)
    - [3.2.3 监控报警](#323-监控报警)
    - [3.2.4 可视化](#324-可视化)
  - [3.3 非功能性需求分析](#33-非功能性需求分析)
    - [3.3.1 可行性分析](#331-可行性分析)
    - [3.3.2 性能效率分析](#332-性能效率分析)
    - [3.3.3 安全性分析](#333-安全性分析)
  - [3.4 本章小结](#34-本章小结)
- [4. 系统设计](#4-系统设计)
  - [4.1 系统总体设计](#41-系统总体设计)
  - [4.2 功能模块设计](#42-功能模块设计)
    - [4.2.1 数据采集模块](#421-数据采集模块)
    - [4.2.2 数据存储与处理模块](#422-数据存储与处理模块)
    - [4.2.3 前端可视化模块](#423-前端可视化模块)
  - [4.3 数据库设计](#43-数据库设计)
  - [4.5 本章小结](#45-本章小结)
- [5. 系统实现](#5-系统实现)
  - [5.1 数据采集模块](#51-数据采集模块)
  - [5.2 数据存储与处理模块](#52-数据存储与处理模块)
  - [5.3 前端可视化模块](#53-前端可视化模块)
  - [5.4 本章小结](#54-本章小结)
- [6. 系统测试](#6-系统测试)
  - [6.1 数据量测试](#61-数据量测试)
  - [6.2 负载均衡测试](#62-负载均衡测试)
  - [6.3 告警测试](#63-告警测试)
  - [6.4 历史数据查询测试](#64-历史数据查询测试)
  - [6.5 本章小结](#65-本章小结)
- [7. 总结与展望](#7-总结与展望)
- [致谢](#致谢)

## 1. 绪论

### 1.1 课题研究背景
阐述选题的理由。

### 1.2 课题研究意义

### 1.3 国内外现状
对本课题现有的研究进展情况的简要介绍。

### 1.4 论文研究内容
本文所要解决的问题，采用的手段、方法和步骤，所需要的条件，预期成果。

### 1.5 论文的组织结构
本毕业设计的主要目的在于实现一套完整的Linux服务器监控告警系统，为Linux服务器的故障检测报警和性能瓶颈分析提供有力依据，提高企业内运维人员的工作效率。本论文各章节结构安排如下：  
第1章，绪论。介绍Linux服务器监控告警系统的研究背景与意义，分析国内外研究现状及发展趋势，最后安排论文的章节组织结构。  
第2章，关键技术。结合监控告警系统的技术选型过程，简述最终所选关键技术的主要优势以及选择的原因，为后文的设计与实现提供充足的理论基础。  
第3章，需求分析。从监控告警系统的整体需求入手，先对系统的各个模块进行梳理，分析各模块功能性需求，再从可行性、性能效率、安全性几个方面结合已划分的模块分析系统非功能性需求。  
第4章，系统设计。剖析监控告警系统整体架构，明确系统的模块划分及各模块承担的任务，给出各个模块的实现思路，最后结合数据库E-R图设计数据库的具体结构。  
第5章，系统实现。按照系统设计的功能模块划分，依次介绍了数据采集模块，数据存储与处理模块，和前端可视化模块的具体实现，搭配流程图介绍了关键部分的代码逻辑，并对一些重要流程进行了详细说明。  
第6章，系统测试。选取了监控告警系统最为关键的几个方面进行测试，检验系统实现的最终成果，确保了该监控告警系统能够应用于真实的线上环境。  
第7章，总结与展望。对本次毕业设计所完成的工作做了简要总结，并对后续改进提出了部分设想。  

## 2. 关键技术介绍

### 2.1 Golang
Golang是Google在2009年正式推出的一门编译型编程语言，其主要特点包括静态类型，原生支持并发，方便交叉编译，可混合调用C语言代码等，正是这些特点促使它成为了编写监控数据采集端的最佳选择。  
为了采集Linux系统的各项指标，首先想到的应该是使用C语言编写监控程序，方便直接利用Linux内核提供的系统调用读取各项系统信息，例如procps工具包中常用的free，ps，top等命令就全由C语言编写而成。但监控程序除了负责数据采集之外，还要对数据进行预处理，并将封装好的数据包发送到存储端，然而偏底层的特点使得C语言在数据封装和网络通信上具有一定的劣势，缺少包管理也让引入第三方依赖的成本较为高昂。相比之下，Golang丰富的标准库中提供了诸如net/http，encoding/json，crypto等常用的库，这使得Golang的数据封装和网络通信变得极为方便，而C语言所擅长的数据采集部分，Google官方也提供了golang.org/x/sys库封装好了常用的操作系统底层调用，因此在当前的使用场景下，Golang十分适合替代C语言担任监控数据采集端的角色。  
除了C语言，其他常用的编程语言还有C++，Java，Python等。C++对比C语言的主要优势是面向对象和标准模板库，但在当前的场景下其优势并不能发挥出来，和Golang比较时有着和C语言相同的缺点，因此不适合用来编写数据采集端。Java和Python相同，都需要提前配置额外的运行环境，Java需要JRE，Python需要解释器，作为Linux系统的监控数据采集端，应当尽可能地减少外部依赖，不能影响到被监控系统本身的生产环境，Golang作为编译型语言在这一点上同样占有优势。  
除此之外，Golang原生支持并发的特点使得各项监控数据很容易就可以做到并行采集，采集到的数据后续的封装和发送也可以与采集过程并行处理，这使得采集端拥有了提供极细粒度监控数据的能力。Golang方便交叉编译的特点体现在改变编译时的GOARCH环境变量就可以得到不同架构下的可执行程序，例如386，amd64，arm，arm64，mips等，可以轻松提供多个版本的监控端。

### 2.2 Node.js
Node.js是一个基于Google V8引擎的服务端JavaScript运行环境，主要特点是事件驱动和非阻塞异步I/O。JavaScript最初是运行在浏览器中的，V8引擎就是Google为其浏览器Chrome所设计的开源JavaScript引擎，但V8引擎并不局限于在浏览器中运行，它还提供了“嵌入”的功能，开发者可以在自己的程序中嵌入该引擎。于是在提供了一系列的文件系统I/O，网络通信，二进制数据流，加密算法等API接口之后，JavaScript就可以借助V8引擎运行在服务端。  
传统的服务端开发语言如PHP，Java等，在处理并发的连接时，通常会启动新的线程，每个线程对应一个连接，而Node.js打破了这一常规，通过事件驱动和非阻塞异步I/O，Node.js可以在单个线程中处理大量并发连接，减小了线程上下文切换的开销，有效地提升了对于大量并发连接的承载能力。生产环境下为了充分发挥多核CPU的优势，往往会启动多个服务进程或者利用Node.js本身的Cluster模块启动多个worker。  
考虑到数据采集端与存储端之间准备通过WebSocket长连接的方式进行通信，存储端的主要任务是转发监控数据到数据库和从数据库查询已有的监控数据，不需要在存储端进行大量的计算，因此选择了Node.js作为存储端实现方式，Node.js的特点使得其十分适合处理IO密集型任务而不擅长于CPU密集型任务。  
除了语言本身的特点之外，Node.js还提供了一个默认的包管理器npm，开发者可以利用npm将自己编写好的模块上传至官方仓库，也可以利用npm下载其它人的模块。在2019年6月初，官方仓库中的模块总量就突破了一百万，足以证明Node.js社区的活跃，在npm的帮助下Node.js的开发效率也相当不错。

### 2.3 Express
Node.js下的Web框架有很多，如Express，Koa，NestJS等，每个框架都有自己的独特之处，从其中选取用于存储端的Web框架之前，要明确存储端在整个监控系统中扮演的具体角色。  
在本系统中，存储端需要承担的任务有：接收WebSocket连接中采集端上报的数据，将收到的数据写入数据库，提供查询数据的HTTP API用于前端可视化，接入监控告警模块。众多监控采集端在运行过程中会持续不停地向存储端上报数据，相当于存储端不是仅会在特定的某段时间内遇到流量高峰，而是始终运行在较高的流量压力之下，因此对Web框架的性能有一定的要求。其次，存储端对外服务方式为WebSocket和HTTP API，不需要像传统的Web应用那样还需要提供具体页面，所以用不到视图层或HTML模板一类的内容。  
Express是Node.js下的一个老牌Web框架，主打快速灵活和极简主义，它的历史最为悠久，使用人数也最多，利用社区提供的丰富中间件可以方便地进行定制和扩展。Koa是另一个主打轻量的框架，其开发者主要来自于Express，开发过程中追求最新的JavaScript语言特性，它比Express更小，性能也更高，默认不提供任何中间件，基本的路由功能也需要额外引入。NestJS相比前两者显得更为全面，它基于Express，使用微软的TypeScript编写，提供了TypeORM对象关系映射器，拥有类似于Java注解的装饰器。  
考虑到存储端的性能要求，NestJS被首先排除，它在各方面所做的优化使得它更符合传统Web框架的定义，适合用来编写大型的完整Web应用，但也因此损耗了Node.js的部分性能，不适合更加具体化的单一场景。Koa和Express相比显得过于简洁了，像是为那些优化已有程序并且追求极致的开发者准备的，完全由开发者自己进行定制。因此存储端的Web框架最终选择了Express，简洁灵活的同时又提供了Web开发的基础工具，社区规模庞大，和其它工具库的协作也较为方便。

### 2.4 React
前端三大框架分别为Angular，React和Vue。Angular作为其中最为完备的框架，学习曲线也最为陡峭，专注于大规模的复杂应用，灵活度受限。React是一套构建UI的框架，操作只集中在视图层，它可以与已知的各类前端库很好地配合，灵活度比Angular更高。Vue则是三者中最为灵活的一个，开发门槛低，易于理解和学习。  
本系统中设计的前端和后端是完全解耦的，单纯通过HTTP API进行通信，因此不需要用到Angular这种大而全的框架，React或Vue即可满足全部需求。相比Vue来说React更加成熟，社区也更加庞大，因此最终选择了React来构建前端页面。  
React作为前端框架提供的是编写UI的方法，并没有现成的UI组件可以立即使用，所以实际开发中还需要搭配合适的UI库。Material-UI是GitHub上最受欢迎的React UI库之一，它不仅符合Google的Material Design规范，还提供有Material Design之外的实用组件，其完善的开发文档和丰富的代码示例十分适合入门学习，于是界面所用的UI库就选定了Material-UI。监控数据可视化部分需要用到大量的图表，React框架下最热门的图表库是Recharts，但在调研过程中发现它只适合用来做静态内容的展现，对于图表的刷新或者动态增加新数据的处理比较困难，因此换用了对动态图表更加友好的Chart.js，虽然其属于通用的JavaScript库，但得益于React的灵活性，可以使用原生JavaScript进行一系列的封装，绕过React直接调用Chart.js操作图表数据，再封装成React组件后就不会影响到前端应用的其他部分。

### 2.5 TimescaleDB
对于持续增长的海量监控数据，传统的关系型数据库显得力所不逮，而专门的时序数据库则恰好适用于类似场景：与时间有关的数据大量写入，已写入的数据不需要修改，查询多为区间内数据统计。时序数据库会对在数据的压缩存储、聚合查询等多个方面进行针对性的优化。  
在DB-Engines的排行榜上，比较靠前的开源时序数据库有InfluxDB，OpenTSDB，TimescaleDB等。InfluxDB在时序数据库排行榜上稳居第一，它由Golang编写，目标是提供高性能的写入和查询，部署时无需任何外部依赖，但其开源版本只支持单机实例，集群部署属于商业版的高级功能。OpenTSDB基于HBase，依托于Hadoop生态系统下的HDFS进行存储，因此在巨量数据所必需的分布式架构下具有绝对优势，但仅仅用来存储各项系统资源占用的话未免有点大材小用了。而且使用OpenTSDB还需要依次配置Hadoop，ZooKeeper，Hbase，最后才是OpenTSDB的部署，引入OpenTSDB将极大地增加整个系统的复杂度，因此不再考虑。最后是TimescaleDB，它基于PostgreSQL数据库，在时序数据的快速存储和复杂查询上进行了优化，并扩充了PostgreSQL的函数库，例如加入了histogram()计算直方图，lsof()向前查找最新的数据，time_bucket()按时间段聚合数据等。其最大的优点是继承了PostgreSQL的完整SQL支持，可以利用PostgreSQL生态下的现有工具。相比之下，InfluxDB由于使用Golang从头编写，就需要自己实现数据库相关的容错机制，如主从复制，高可用性，备份等，继承自PostgreSQL的TimescaleDB明显在可靠性上占有优势。除此之外，TimescaleDB官方提供了多个版本的Docker镜像，开发或部署时都可以轻松的构建所需环境。

### 2.6 本章小结
技术选型在毕业设计的准备阶段极为重要，没有任何一门技术可以完美地适用于所有开发场景，必须要根据实际需要选取最为合适的解决方案，技术选型将直接影响到接下来的系统设计以及系统实现。本章主要对技术选型最终确定的关键技术进行了简要的论述，明确了所选技术的主要优势以及选择的原因，在后文的系统设计和系统实现部分均会体现出该技术选型所带来的长远影响。

## 3. 需求分析

### 3.1 系统概述
本系统的目标是设计并实现一套用于Linux服务器的监控告警系统，其主要任务包括：服务器正常运行时可以查看历史监控数据，为应用程序的分析优化提供依据；服务器出现异常情况时及时推送告警信息，协助开发人员定位问题。  
根据主要任务来对系统需求进行整体分析，查看历史监控数据要求监控数据必须具备完整的采集、传输、存储流程，并且提供历史数据的查询接口，能够以可视化图表的形式展示历史监控数据，便于开发人员根据历史数据分析资源指标变化趋势，在时间维度上纵向分析服务器状态；异常发生时的及时告警要求此系统可以对各个监控指标设定阈值，对采集到的监控数据进行分析判断，当监控数据达到阈值或满足指定的报警条件后，触发告警机制，根据提前设定好的信息推送渠道将告警信息发送至接收者，同时明确触发告警的服务器及监控规则，方便及时定位服务器故障详细位置，此外监控系统还要允许在故障排除后将异常服务器状况标记为已修复，防止已被处理的过期告警引起混乱。  
对于用户来说，运维人员在相应的服务器上部署完对应的模块后，即可在核心控制台上查看加入到监控系统中的各服务器状态，可以根据服务器运行的历史状态调整指定资源的报警阈值，并添加多种推送渠道用于接收告警信息。

### 3.2 功能性需求分析

#### 3.2.1 数据采集
数据采集模块是监控告警系统的基础，它需要通过Linux系统调用、访问/proc伪文件系统等方式获取Linux系统的资源占用与性能信息，然后将采集到的数据进行预处理，再封装成数据包通过WebSocket发送给存储模块。  
参考国内著名云服务商阿里云所提供的云监控说明文档，数据采集模块从被监控服务器上获取的监控项至少要包含以下几项：基础信息（发行版，内核版本，主机名，CPU型号，CPU核心数），CPU使用率，内存使用情况，交换空间使用情况，系统1/5/15分钟内平均负载，网络流量在单位时间内的上传与下载情况，系统磁盘在单位时间内的读取与写入情况，系统挂载点统计（设备名称，挂载点位置，文件系统，总容量，已用容量，总Inode数量，已用Inode数量）。除此之外，为增强Linux服务器的安全性，数据采集模块还可以监听SSH远程登录事件，指定文件的修改删除事件。  
数据采集模块在部署后将自动运行，无需参与者，其数据流如图3-1。

图3-1 数据采集模块数据流图

#### 3.2.2 数据存储
数据存储模块需要对监控告警系统采集到的各项指标进行存储，判断存储方案是否合适需要考虑写入速度，空间占用，数据安全，查询速度等多个方面。由于数据存储模块所存储的监控数据具有明显的特征，如随时间变化，持续不断增长等，所以可以采用更具有针对性的时序数据库来进行存储。  
时序数据库普遍性地优化了大批量数据的并行插入，并对时间维度下的数据使用更为高效的压缩算法进行存储，减小磁盘空间占用，在查询方面也会优化大量数据下常用的一些聚合函数，可以为系统中的数据可视化提供方便。数据安全方面则得益于TimescaleDB继承的PostgreSQL生态，可以使用流式复制保证数据库的高可用性，主库出现异常情况宕机时从机可以快速切换，防止单个数据库的宕机影响到整个监控系统。PostgreSQL也提供了完善的物理和逻辑备份工具，该工具可以完美用于TimescaleDB，进一步确保数据的安全性。  
数据存储模块在整个系统中承担桥梁的作用，联系着各个模块。它需要为数据采集模块提供初始化注册功能，接收来自已注册数据采集模块的数据；它需要连接数据库，将校验后的合法数据存入数据库；它还连接着监控报警模块，存储管理员设定的报警规则，并将数据采集模块上报的数据推送给监控报警模块进行检查；最后它还要为可视化模块提供数据来源，根据需要的图表类型进行适当的查询优化。  
数据存储模块在部署后同样自动运行，仅与其它各个模块直接发生数据传输，无需具体参与者，其数据流如图3-2。

图3-2 数据存储模块数据流图

#### 3.2.3 监控报警
监控报警算是一个相对比较独立的模块，其主要任务是分析数据存储模块发送过来的数据，当检测到对应的报警规则被触发后，根据设定的推送渠道发送告警信息。这部分就不仅仅是模块之间的内部合作了，还需要管理员的参与。  
监控报警模块首先需要确定监控规则的结构，针对数据采集端收集到的各项监控数据设定的各类规则都要符合或者转化成统一的结构之后才能利用关系型数据库进行存储。作为监控规则，首先需要有明确的监控目标，例如CPU，内存，负载等，有了目标之后需要指定明确的事件，例如使用率达到阈值，速度达到阈值等，接下来指定阈值。为了减少监控数据的偶然峰值造成的误告警，监控规则还可以指定数据聚合区间，例如CPU在过去的五分钟内持续达到阈值才触发告警，而不是单次监控数据达到阈值。触发告警后对应的监控规则还应有可设定的静默时间，防止告警后不断重复告警形成的信息轰炸。  
告警模块应当支持当前常用的通知方式，例如电子邮件，短信，常用的办公IM（企业微信，钉钉，飞书等），管理员可以添加推送渠道后按需设定监控规则的接收者。  
监控报警模块的主要需求不再是数据在不同部分的流动了，而是涉及到了管理员的各项操作，需要提供每种操作对应的后端API，其用例图如图3-3。

图3-3 监控报警模块用例图

#### 3.2.4 可视化
可视化模块是直接与管理员交互的模块，一方面需要负责监控数据的查看，在服务器正常运行时显示历史数据供管理员分析系统瓶颈，预测可能出现的威胁，在服务器异常时明确引发报警的数据，帮助管理员快速定位问题；另一方面需要提供系统功能的控制界面，管理员应当在可视化模块的控制界面上进行相关操作，如查看已注册的主机列表，管理报警规则，管理推送渠道等。  
对于历史数据的查看，应当提供不同的区间设置或者允许管理员自定义区间进行查看，这样管理员既可以缩小区间查看某一小段时间内的详细监控数据，又可以增大区间查看较大时间范围内的数据变化趋势。可视化模块中也可增加独立的概览页面，显示监控系统内整体主机运行状况统计，并提供常用的功能入口，如显示最近查看过的主机，最近修改过的监控规则。  
可视化模块需要提供管理员操作的直接交互接口，其用例图如图3-4。

图3-3 可视化模块用例图

### 3.3 非功能性需求分析

#### 3.3.1 可行性分析
本监控告警系统主要针对Linux服务器，Linux系统是一类开放的操作系统，对开发者极为友好，例如man手册中包含了所有的Linux内核相关系统调用，还介绍了/proc伪文件系统中的文件含义及内容格式，常用的系统资源查看命令如top，free，ps，htop等均提供了开源代码，因此在监控数据采集上不存在无法解决的难题，具有程序可行性，还可以参考常用监控命令的计算方式计算出更符合运维人员直觉的监控指标。  
数据存储模块有时序数据库TimescaleDB提供底层存储支持，其背后是拥有二十多年历史的PostgreSQL，庞大的社区保证了在学习，开发，部署的完整过程中都有足够的资料进行查询，对于常见的各种问题都已经有了相应的解决方案，因此数据存储模块的设计与实现也是切实可行的。  
监控报警主要是接入各种消息推送渠道，常见的邮件发送方案是SMTP协议，配合自己的域名邮箱可以轻松发送告警邮件，只需要注意免费的邮箱通常有每日发送数量限制，生产环境下应当接入自建的企业邮箱或使用邮件相关的付费服务。常用的办公IM如企业微信，钉钉，飞书等，均提供了在企业群组中添加通知机器人的功能，查看相关文档后发现都是类似的Webhook接口，在编程过程中可以轻松调用。对于更加实时的短信推送，查看相关服务后发现均需企业认证，以个人身份申请需要较长审核流程，而且无免费服务，因此本地开发时只能使用kde-connect直接连接手机模拟通过程序进行短信告警。  
可视化模块选用的是流行的React前端框架，搭配组件丰富的Material-UI前端库，足够实现该系统所需的各类交互，为操作者提供良好的操作体验。图表方面Chart.js官方对于各类图表都提供了具体的代码示例，并且文档中也结合常用场景介绍了相关优化，GitHub的issue页面可以查找到开发过程中的常见问题，因此可视化模块方面也不存在明显困难。

#### 3.3.2 性能效率分析
考虑到监控系统需要处理持续不断的新增数据，技术选型时就选择了具有一定性能优势的技术。例如监控数据采集端选用Golang进行编写，原生支持并发使得并行采集各项监控数据十分简单，采集到的数据后续的处理也可以与采集过程并行处理，采集端能够提供极细粒度下的监控数据。采集到的监控数据通过WebSocket长连接进行发送，相比普通的HTTP连接减小了重复建立连接的消耗，不需要每次都发送Header或是进行身份验证，提高了数据传输效率，除此之外也可以利用WebSocket长连接的特点监控目标主机的连接与断开，避免了轮询方式带来的性能损耗。数据存储端选用的是Node.js，其事件驱动和非阻塞异步I/O有效地提升了对于大量并发连接的承载能力，对于当前这种非计算密集型应用十分高效。可视化模块中的系统功能控制界面展示数据量有限，不会遇到性能方面的问题，但历史监控数据展示界面的Chart.js图表将会处理大量数据，例如管理员查看近一个月内的监控数据来分析整体趋势，每10秒上报一次的监控采集端将会累计达到二十五万个数据点，前端无法直接处理如此多的数据，后端发送这么多数据的代价也是极大的，因此后端需要对数据进行聚合，此处正好用到了时序数据库TimescaleDB提供的聚合函数，可以根据总的时间范围计算出合适的聚合区间，然后按聚合区间将数据进行压缩，减少数据点数量同时不影响整体趋势。

#### 3.3.3 安全性分析
本系统分为多个模块，其中模块之间需要相互通信，部分模块还需要连接外网，必须要严格考虑系统整体的对外安全性。  
数据采集模块不需要连接外网，只需要连接存储模块进行监控数据上报，因此考虑安全性的地方是WebSocket连接的认证。Linux系统本身提供有machine-id，采集模块首次启动时发送machine-id到存储模块请求注册，存储模块使用AES-256-CFB进行加密，其中key在存储模块中提前配置，iv使用Node.js随机生成，加密后的结果作为token返回给采集模块，同时插入数据库。此后WebSocket握手过程中需要指明token用于身份认证。使用加密而不是哈希是为了能够逆向计算出原machine-id，允许数据采集模块主动刷新token，对旧的token进行失效处理，AES-256-CFB则可以保证每次刷新都可以生成不同的token。  
存储模块除了接收数据采集模块上报的监控数据外，还需要为可视化模块提供查询功能，因此实现了一个简单的登录注册，登录后才可以访问到可视化模块，存储模块的各个查询接口都需要校验用户登录状态。用户的密码在数据库中与邮箱共同哈希后进行存储，哈希可以有效防止数据库泄露后一同泄露用户密码，与邮箱共同哈希则大大增加了哈希碰撞的难度。用户的登录状态使用服务端session进行存储，技术选型中选择的Express提供了成熟的redis-session中间件，redis统一管理session可以为存储端的多机部署提前打下基础。  
除具体模块外，数据库相关的安全设置则推荐设置数据库为内网监听，仅允许存储模块通过内网IP进行访问，不对外暴露端口。

### 3.4 本章小结
需求分析是毕业设计前期一个极为重要的步骤，本章的系统概述章节介绍了Linux服务器监控告警系统的整体需求，功能性需求分析则对系统的各个模块进行梳理，结合实际监控场景明确该系统要实现的功能，同时保留适当的扩展性，非功能性需求分析分别从可行性、性能效率、安全性几个方面结合已划分的几个模块进行论述，分析了需要在各个方面的优化上做出的努力。

## 4. 系统设计

### 4.1 系统总体设计
根据上文需求分析的结果，可以确定该监控告警系统主要功能需求包括：数据采集，数据存储，监控告警，可视化。数据采集模块将采集到的监控数据通过WebSocket上报到数据存储模块，数据存储模块校验数据后将数据存入时序数据库，同时发送一份数据供监控告警模块进行阈值检查，触发告警后根据推送渠道发送告警信息，管理员与可视化模块进行直接交互，在控制界面设置监控规则和推送目标，也可选择主机查看历史监控数据，可视化模块所调用的HTTP接口均由存储模块提供。  
考虑到存储模块和监控告警模块之间会有频繁的数据传输，且报警规则的查询也由存储模块提供，所以在具体实现过程中存储模块与监控告警模块不适合分开来写，将这两部分将会整合到同一个数据存储与处理模块之中。除此之外，数据存储与处理模块本身应当是无状态的，可以同时部署多个来进行横向扩展，用户session存储在共享的redis中，持久性数据存储在共享的TimescaleDB中，利用高性能服务器Nginx进行负载均衡，对于来自数据采集模块的连接或者是可视化模块发过来的请求，Nginx会根据连接数自动选择负载更低的数据存储与处理模块。当某个数据存储与处理模块出现异常导致崩溃时，保证还有其它的数据存储与处理模块可以处理请求。  
除了横向扩展的数据存储与处理模块互为备份保证系统可用性之外，监控采集模块和数据存储与处理模块也要利用Linux本身的systemd守护进程编写合适的service文件，设定崩溃时的自动重启。整体的系统设计如图4-1。

4-1 系统整体设计

### 4.2 功能模块设计

#### 4.2.1 数据采集模块
数据采集模块的目标监控项目有基础信息（发行版，内核版本，主机名，CPU型号，CPU核心数），CPU使用率，内存使用情况，交换空间使用情况，系统1/5/15分钟内平均负载，网络流量在单位时间内的上传与下载情况，系统磁盘在单位时间内的读取与写入情况，系统挂载点统计（设备名称，挂载点位置，文件系统，总容量，已用容量，总Inode数量，已用Inode数量），除此之外以可选插件的形式支持SSH远程登录事件和指定文件的修改删除事件的监控上报，接下来将介绍各个监控指标的具体采集方法。  
基础信息主要采集的是主机上基本不会发生改变的数据，对于这些数据无需记录其变化历史，只需关注其当前值即可，因此只在每次数据采集模块刚与存储模块建立WebSocket连接后触发一次更新。发行版名称来自/etc/os-release文件中的PRETTY_NAME项，man 5 os-release可以找到具体的介绍，其中的PRETTY_NAME是一个适合对用户展示的操作系统名称。内核版本来自/proc/sys/kernel/osrelease文件的全部内容，man 5 procfs中在/proc/version部分提到了该文件会包含具体的内核版本号。主机名通常是运维人员手动设置的主机标识，在主机数量较多的情况下对于区分不同主机十分重要，Linux系统从/etc/hostname文件和/proc/sys/kernel/hostname文件中都可以读出主机名，但/proc伪文件系统作为内存的映射，其时效性显然高于/etc/hostname，man 5 hostname也可以看到/etc/hostname是在系统启动时的sethostname()系统调用设置的，因此从/proc/sys/kernel/hostname文件中读取主机名相对更为准确。CPU型号和CPU核心数在Linux下可以通过lscpu命令进行查看，但在监控模块中解析shell命令是相当丑陋的实现。得益于Linux的开放性，在GitHub的util-linux仓库中可以找到lscpu的源码，阅读源码得知其数据来自于/proc/cpuinfo文件，读取其中的model name和cpu cores可以得到CPU型号和CPU核心数。除此之外，IP地址也是一项十分重要的数据，但由于不可控的网络结构，主机本身是无法获取到自己对外的IP的，于是更新IP地址的任务交给了存储端，当WebSocket握手成功后存储端会更新对应主机的IP地址，需要注意由于Nginx负载均衡的存在，Nginx配置中需要额外设置HTTP请求头来保留请求的原始IP。  
CPU使用率在top，htop等Linux常用工具上都有体现，为了使得数据采集模块上报的CPU使用率更加符合运维人员直觉，模块计算出的CPU使用率应该与top或htop一致或近似，于是分别阅读了top和htop关于计算CPU使用率的相关代码。其核心思想均为计算一段时间内CPU实际运行时间的占比，数据则来源于/proc/stat，手册man 5 procfs中/proc/stat节详细介绍了该文件的具体格式。对比之后发现htop的计算考虑更加全面，因此数据采集模块需要使用Golang读取/proc/stat文件，参考htop的计算方法，计算出主机整体的CPU使用率。  
内存和交换空间的使用情况通常使用free命令直接查看，但其显示的只有各个部分的实际占用，包括总计，已用，空闲，共享，缓存，可用，如果要计算出一个具体的占用百分比的话，并不能从free命令的实现代码中找到计算方法。Linux下还有一个常用的系统信息查看脚本screenfetch，其内存占用栏会计算出一个直观的百分比，于是在GitHub上查看了screenfetch的具体内容，发现其最新版本代码中解析的是free命令的输出，总内存减去可用内存得到实际占用的内存，然后再计算得到内存占用率。接下来又在GitLab上的procps仓库找到了free命令的源码，得知该命令首先会读取/proc/meminfo文件，然后将其中MemAvailable的值记作可用内存，如果在/proc/meminfo文件中没有找到MemAvailable项，则执行另一套较复杂的计算方法。在手册man 5 procfs中找到/proc/meminfo部分，其附加说明中提到MemAvailable是Linux 3.14之后才增加的新项，其值表示启动新程序时的真实可用内存。但除了读取/proc/meminfo文件，在搜索过程中也发现了Linux的系统调用int sysinfo(struct sysinfo *info)可以读取到相关内存和交换空间信息，进一步研究之后发现该系统调用取到的数据只是/proc伪文件系统提供数据的一个子集，仅通过该系统调用并不能得到真实的可用内存，相比之下还是主动读取/proc/meminfo文件得到的数据更为准确。在维基百科的Linux kernel version history页面上查找到Linux内核的3.14版本在2014年3月份发布，当前还在维护的最旧的版本是3.16，所以不考虑过老系统的话数据采集模块可以直接读取/proc/meminfo，系统整体的内存占用则通过Memtotal减去MemAvailable进行计算，然后除以总内存得到内存占用率。  
系统平均负载的查看命令是uptime，末尾的三个值分别表示一分钟，五分钟，十五分钟的平均负载，查看GitLab上的procps仓库中相关源码，其平均负载的数据来自于/proc/loadavg文件。由于平均负载的值通常较小，且保留两位小数，所以数据采集模块在读取到该数据后可以在预处理阶段将值扩大100倍，作为整型发送到数据存储模块及存入数据库，整型数据在计算和存储上都比IEEE 754标准下的浮点数具有一定优势。  
网络流量的统计数据可以从/proc/net/dev文件中读取到，手册man 5 procfs中/proc/net/dev部分有详细的格式介绍。但Linux下/proc/net/dev文件中通常不只有一块网卡，还会包含本地回环设备lo，使用Docker时还会有Docker的虚拟网卡，特殊情况下还会有运维人员手动创建的网桥。对于管理员来说，关心的网络流量应该是与其它主机之间发生的网络传输，而不是本地虚拟网卡上发生的数据交换，因此需要采取合适的方法排除掉虚拟网卡上的流量。Linux系统除了/proc伪文件系统之外，还提供了一个/sys伪文件系统，包含的是Linux内核上的一些设备对象，在手册man 5 sysfs中可以找到/sys/class/net部分，介绍该目录下会包含多个实体，每个实体都是对应物理或虚拟网络设备的符号链接。查看本机的/sys/class/net目录后发现，虚拟网络设备的符号链接都会指向/sys/devices/virtual/net目录下的设备。因此可以首先读取/sys/class/net目录得到所有的网络设备，再通过判断符号链接的路径排除掉虚拟设备，剩下的物理网络设备则到/proc/net/dev中读取对应的流量信息，然后将物理网卡上的流量进行汇总，上报到数据存储模块。  
系统磁盘的读写情况在手册man 5 procfs中提到了/proc/diskstats文件，但并未给出此文件的格式说明，而是说详细信息记录在Linux内核源码的Documentation/iostats.txt文档中。在GitHub上Linux仓库中搜索，发现2019年6月的一次commit将文档从Documentation/iostats.txt迁移到了Documentation/admin-guide/iostats.rst，但内容并未发生改变。文档中表面/proc/diskstats里包含有读写请求次数和大小，且各项均为系统启动到当前为止的累计值，因此数据采集模块每次计算与前一次读取的差值，再除以时间即可得到单位时间内的速率。需要注意的是一块磁盘可以有多个分区，/proc/diskstats中对于每块磁盘都有一行总计数据，然后是各个分区的统计数据，数据采集模块计算时要避免对一块磁盘进行重复计算。  
Linux下常用的df命令对应数据采集模块的系统挂载点统计，对于每个挂载点，应当包含设备名称，挂载点位置，文件系统，总容量，已用容量，总Inode数量，已用Inode数量。df命令的源码在GitHub上的coreutils镜像仓库，其中先从/proc/mounts文件中找到物理磁盘的挂载位置，然后调用get_fs_usage()函数读取磁盘统计情况，而get_fs_usage()函数的实现则在另一gnulib镜像仓库中，其涉及到的系统调用有statvfs()，statvfs64()，statfs()，会根据系统类型选取对应的系统调用。在Golang官方的golang.org/x/sys/unix包中，封装了unix.Statfs()供开发者使用，因此系统挂载点的统计首先要读取/proc/mounts得到实际的物理磁盘，然后通过unix.Statfs()读取磁盘使用情况，处理好计算单位后将数据发送到数据存储模块。  
上面是数据采集模块需要周期性采集的监控数据，对于不需要周期性采集的监控项，数据采集模块提供了插件机制进行处理。插件模块监听本地的127.0.0.1，保证只有本地可以向插件提交数据，然后各个插件通过HTTP协议的POST方式以JSON格式提交事件，插件模块收到请求后进行相应的二次封装，再上报给数据存储模块。  
为增强主机安全，可以使用数据采集模块的插件机制实现SSH远程登录事件上报。SSH远程登录不需要数据采集模块周期性地进行检查，而是当有用户通过SSH成功远程登录时才将事件上报到数据存储模块，该插件利用了Linux下的PAM机制。PAM是Linux系统上用来为服务或应用提供系统身份认证的一种机制，例如当用户通过SSH登录时，PAM会根据/etc/pam.d/sshd文件中为SSH服务指定的认证方式进行身份验证，通过后用户才算登录成功。因此本插件需要根据PAM规范编写一个共享库文件，并在/etc/pam.d/sshd配置文件中加入该共享库，设定为可选认证，即每次SSH登录都会执行，但不影响SSH登录的成功与否。因为要通过HTTP接口上报信息，所以该插件也用Goalng编写，利用Goalng的cgo特点实现PAM规范所要求的C函数入口，编写完成后将编译结果放入系统PAM模块目录下。  
除了SSH远程登录的监控，对指定文件的修改删除事件监控也可以有效提高系统安全性，例如SSH的authorized_keys文件，sudo命令对应的/etc/sudoers配置文件等。Linux内核提供有inotify API，可以监听到指定文件或目录的修改删除操作，相对应的，Golang官方的golang.org/x/sys/unix包中也封装了相关API。由于Linux文件系统的分层结构，文件名称的实际数据存储于上层目录中，因此使用inotify单纯监控文件时无法感知到文件的改名操作，恶意访问者可以将文件改名后在目录下用原文件名新建文件来绕过inotify的监听，因此插件中需要一同监听指定文件的上层目录，对于目录中产生的新建，移动事件判断是否涉及被监控的文件，操作被监控文件时插件及时更新文件监视器。

#### 4.2.2 数据存储与处理模块
存储模块与监控告警模块合并为数据存储与处理模块，负责接收数据采集端上报的数据，将数据插入时序数据库，在监控数据达到监控规则设定的阈值时触发告警推送，为可视化模块提供数据API。接下来按功能介绍相关设计。  
存储模块选用的是Node.js下的老牌Web框架Express，集成WebSocket服务已有十分成熟的方案，它可以监听现有服务器的upgrade事件，当收到的连接类型指明为upgrade时，转发到WebSocket路由进行后续处理。WebSocket路由中先检查Header中是否含有合法token，在数据库中查找到token对应的主机时，将主机ID附到连接上，并返回握手成功，主机ID将会用于后续监控数据的插入。之后WebSocket模块将对收到的消息进行JSON解析得到数据包，根据数据包的category交给不同的存储函数。其中对于基础信息的数据包，除了更新数据库内的主机基础信息之外还会更新IP，由于在系统的整体设计中加入了Nginx负载均衡层，所以IP的获取需要按照一定的顺序，首先判断是否有x-nginx-proxy标识，有的话以x-real-ip作为主机IP，x-real-ip不存在时则以x-forwarded-for最左端IP作为主机IP；如果请求头中不包含x-nginx-proxy标识，则认为数据采集端属于直连，读取request.socket.remoteAddress获取主机真实IP。  
时序数据库TimescaleDB所选用的客户端是node-postgres，其官方文档中就有Express框架下的推荐结构，数据存储与处理模块中与数据库的交互将单独整理到一个Node.js模块中，对外暴露出各种函数供其它模块调用，这样如果以后数据量过大，更换存储端数据库实现时，可以方便对现有代码进行迁移，不会影响到其它内容。数据库连接方面也利用到了node-postgres的连接池特性，提高存储端并行处理能力，对于系统挂载点这样一个数据包中含有多条数据的，采用了事务的方式执行批量插入，有利于提高插入效率。  
监控告警模块则可以抽象为分析器和告警器，分析器负责按定义好的监控规则分析数据，触发告警事件时通知告警器进行告警，告警器集成常见的推送方法，如邮件，Webhook，短信等。分析器支持的规则包括：过去一段时间内平均CPU使用率达到阈值；过去一段时间内平均内存占用率达到阈值；1分钟系统平均负载达到阈值；5分钟系统平均负载达到阈值；15分钟系统平均负载达到阈值；过去一段时间内平均下载速度达到阈值；过去一段时间内平均上传速度达到阈值；过去一段时间内平均磁盘读取速度达到阈值；过去一段时间内平均磁盘写入速度达到阈值；指定挂载点磁盘空间利用率达到阈值；指定挂载点磁盘inode利用率达到阈值；用户使用密码进行了SSH登录；有用户首次从陌生IP地址登录；指定文件被修改。  
对于提供给可视化模块的API，路由均以/api开头，方便利用Nginx统一进行反向代理，同时结合RESTful风格，在易用性方面对路由设置进行优化。API的身份认证方面，常用的有服务端session和客户端session，考虑到认证方面的安全性，服务端session可以保证用户注销时对旧session进行有效的销毁，还可以轻松限制帐号的多端登录，因此选用的是服务端session。为了方便在不同的数据存储与处理模块之间同步用户登录状态，应当使用redis作为session存储器，通过Express的redis-session中间件对session进行统一管理。
对于可视化模块请求较长时间范围内的历史数据，管理员关注的往往是历史数据的整体趋势，而不是每个数据点的详细值，因此存储模块需要利用TimescaleDB提供的聚合函数对数据进行聚合，根据总的时间范围计算出合适的聚合区间，然后按聚合区间将数据进行压缩，减少数据点数量同时不影响整体趋势，极大地减轻前端图表的性能压力。

#### 4.2.3 前端可视化模块
可视化模块与其它模块相对独立，仅通过HTTP接口与数据存储模块通信，同时随着浏览器的不断向前发展，内置的fetch()调用就足够发起各种类型的XHR请求，不需要再引入臃肿的第三方request包，因此在API部分可以先将内置的fetch()调用封装成常用的请求方法，再用这些请求方法将用到的接口封装为API模块导出。  
UI设计上自身缺少该方面的基础，因此目标是在尽量简洁干净的基础上提高可视化界面的易用性，而不过分执着于追求美观。参考了众多常用网站后确定了其基本布局，可视化界面先分左右两栏，左栏为常驻抽屉，提供导航功能，指向各主功能入口，右栏再上下分割，上栏为页面标题，提示当前所处页面对应功能，下栏为页面主体，显示主要内容。布局详情如图4-2。

图4-2 可视化界面基本布局

可视化模块共提供概览，主机列表，规则管理，推送管理，警报历史，系统设置作为主要功能页面。概览页统计近7日报警状况，正在报警主机数，运行正常主机数，并提供最近访问主机和规则组作为快速入口，最近访问数据使用浏览器提供的localStorage接口存储在本地即可，无需持久保存在数据库中。主机列表页显示所有主机简要信息，包含发行版，主机名，IP和当前状态，可点击查看主机详情，在详情页再显示主机详细信息，除简要信息之外还包含内核版本，CPU型号，CPU核心数等。对于历史监控数据的查看也在主机详情页进行，默认显示近一个小时内的监控数据，提供6小时，12小时，1天，3天，7天，14天的快速切换选项，同时支持自定义时间范围。规则管理页显示规则组名称及创建时间，可以在该页面删除或新建规则组。以规则组为入口，可以选择管理其中的规则，管理加入的主机，管理告警时的推送渠道。推送管理页面会列出全部的推送渠道详情，包含类型，名称，地址，凭据和创建时间，可以新建删除，或是修改现有推送，修改后将会对所有规则组生效，即已添加该推送渠道的规则组中会同步更新到新的推送地址。警报历史显示系统已发出的所有告警详情，通过选择告警信息可以跳转到关联的主机详情页或关联的规则组详情页。最后的系统设置页则提供关闭注册的功能，不允许新的用户注册。  
React编写完成的前端项目还需要经过Webpack的编译打包才能用于部署，本项目中准备将各个页面进行拆分，因此需要使用到Webpack的HtmlWebpackPlugin对各个html页面进行处理，将带有哈希的JavaScript文件路径注入到各个页面中，JavaScript文件名中带有哈希可以方便生产环境下的缓存设置，不需要担心前端代码更新后用户加载到旧的JavaScript文件。对于长期不会发生修改的静态资源，如项目logo，发行版图标等，不需要打包进JavaScript中，因此可以将静态资源统一放到一个目录下，使用Webpack的CopyPlugin单独处理。在为生产环境编译打包时，可以利用Webpack设置更多的优化项，如TerserPlugin用于压缩混淆JavaScript代码，减小文件体积，splitChunks可以将代码中的公共部分提取出来，便于多个页面复用，提升用户体验。除了最终编译打包时会用到Webpack之外，开发过程中也需要用到Webpack的devServer，devServer可以提供开发时需要的本地HTTP服务以及数据存储模块的API反向代理。

### 4.3 数据库设计
本系统所选数据库为TimescaleDB，在实际的使用过程中需要对已有数据表执行create_hypertable()操作将其转换为时序数据表，之后才会应用TimescaleDB的各项优化，若数据表没有执行过create_hypertable()操作，那么该数据表就会被当成普通的PostgreSQL数据表对待，因此除时序数据之外，TimescaleDB也可处理普通的关系型数据，不需要再额外选择单独的关系型数据库。系统整体数据库设计的E-R简图如图4-3。

图4-3 系统数据库设计E-R简图

除图4-3中所示实体之外，还有与其它实体无关的用户实体和系统设置实体，用户实体用于基本的登录注册，系统设置存储系统相关首选项，如是否禁止其它用户注册，因此对数据库所需的所以数据表进行汇总，得表4-1。

表4-1 数据表汇总
| 序号 | 表名           | 说明                 |
| ---- | -------------- | -------------------- |
| 1    | agents         | 基础信息表           |
| 2    | cpuinfos       | CPU占用统计          |
| 3    | meminfos       | 内存占用统计         |
| 4    | loadinfos      | 平均负载统计         |
| 5    | netinfos       | 流量统计             |
| 6    | diskinfos      | 磁盘读写统计         |
| 7    | mountinfos     | 挂载点统计           |
| 8    | sshdinfos      | SSH登录事件统计      |
| 9    | filemdinfos    | 文件修改删除事件统计 |
| 10   | users          | 用户基础信息         |
| 11   | groups         | 规则组               |
| 12   | rules          | 监控规则             |
| 13   | agent_group    | 主机所属规则组       |
| 14   | receivers      | 接收者（推送渠道）   |
| 15   | receiver_group | 接收者所属规则组     |
| 16   | messages       | 告警信息             |
| 17   | preferences    | 系统设置             |

基础信息表用来存储主机的基础数据项，对于这些变化频率极低的数据，无需记录其变化历史，只需关注其当前值即可，因此主要内容只在建立WebSocket连接时进行更新，无需作为时序数据进行存储。

表4-2 基础信息表
| 字段       | 类型         | 备注                                 |
| ---------- | ------------ | ------------------------------------ |
| id         | SERIAL       | 自增主键                             |
| token      | VARCHAR(64)  | 身份标识符                           |
| distro     | VARCHAR(64)  | 发行版名称 (64 from <sys/utsname.h>) |
| kernel     | VARCHAR(64)  | 内核版本 (64 from <sys/utsname.h>)   |
| hostname   | VARCHAR(255) | 主机名 (255 from man 7 hostname)     |
| cpu_model  | VARCHAR(255) | cpu型号                              |
| cpu_cores  | SMALLINT     | cpu核心数                            |
| deleted    | BOOLEAN      | 逻辑删除标识                         |
| created_at | TIMESTAMPTZ  | 创建时间                             |
| updated_at | TIMESTAMPTZ  | 更新时间                             |

CPU占用统计表存储各主机的整体CPU使用率，数据采集模块需将计算得到的百分比乘上10000进行取整，保证数据传输和存储过程中均使用整型，设置外键agent_id关联到主机，作为时序数据进行存储。

表4-3 CPU占用统计
| 字段         | 类型        | 备注                |
| ------------ | ----------- | ------------------- |
| id           | BIGSERIAL   | 自增主键            |
| agent_id     | INTEGER     | 外键                |
| used_percent | SMALLINT    | 使用率 (n% * 10000) |
| time         | TIMESTAMPTZ | 数据采集时间        |

内存占用统计表存储各主机的内存和交换空间详细使用情况，数据采集模块计算出的百分比也乘上10000进行取整，在数据传输和存储过程中均使用整型，存储空间以KB为单位，设置外键agent_id关联到主机，作为时序数据进行存储。

表4-4 内存占用统计
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

平均负载统计表存储各主机的1/5/15分钟系统平均负载，由于平均负载的值通常较小且保留两位小数，所以数据采集模块需将原值乘上100，作为整型发送到数据存储模块及存入数据库，外键agent_id用于关联到主机，此表也作为时序数据进行存储。

表4-5 平均负载统计
| 字段     | 类型        | 备注                       |
| -------- | ----------- | -------------------------- |
| id       | BIGSERIAL   | 自增主键                   |
| agent_id | INTEGER     | 外键                       |
| avg1     | SMALLINT    | 一分钟平均负载 (n * 100)   |
| avg5     | SMALLINT    | 五分钟平均负载 (n * 100)   |
| avg15    | SMALLINT    | 十五分钟平均负载 (n * 100) |
| time     | TIMESTAMPTZ | 数据采集时间               |

流量统计表存储各主机的近期流量使用情况，其中数据单位精确到字节，为防止累计数据量过大导致整型溢出，对于上传下载总量应使用BIGINT进行存储，除此之外还要设置外键agent_id关联到主机，作为时序数据进行存储。

表4-6 流量统计
| 字段             | 类型        | 备注                        |
| ---------------- | ----------- | --------------------------- |
| id               | BIGSERIAL   | 自增主键                    |
| agent_id         | INTEGER     | 外键                        |
| receive_rate     | INTEGER     | 下载速度 (bytes per second) |
| receive_sum      | BIGINT      | 下载量 (bytes)              |
| receive_packets  | INTEGER     | 下载数据包数量              |
| transmit_rate    | INTEGER     | 上传速度 (bytes per second) |
| transmit_sum     | BIGINT      | 上传量 (bytes)              |
| transmit_packets | INTEGER     | 上传数据包数量              |
| time             | TIMESTAMPTZ | 数据采集时间                |

磁盘读写统计表存储各主机的近期磁盘读写情况，其中累计读写量单位精确到KB，同时使用BIGINT进行存储防止溢出，除此之外还要设置外键agent_id关联到主机，并作为时序数据进行存储。

表4-7 磁盘读写统计
| 字段       | 类型        | 备注                        |
| ---------- | ----------- | --------------------------- |
| id         | BIGSERIAL   | 自增主键                    |
| agent_id   | INTEGER     | 外键                        |
| read_req   | INTEGER     | 每秒读取请求次数            |
| write_req  | INTEGER     | 每秒写入请求次数            |
| read_rate  | INTEGER     | 读取速度 (bytes per second) |
| write_rate | INTEGER     | 写入速度 (bytes per second) |
| read_size  | BIGINT      | 读取总量 (KB)               |
| write_size | BIGINT      | 写入总量 (KB)               |
| time       | TIMESTAMPTZ | 数据采集时间                |

挂载点统计表存储各主机的挂载点使用情况，其中涉及到使用率的数据项也乘上10000作为整型存储，磁盘容量单位精确到KB，同时使用BIGINT进行存储防止溢出，除此之外还要设置外键agent_id关联到主机，并作为时序数据进行存储。

表4-8 挂载点统计
| 字段               | 类型          | 备注                                    |
| ------------------ | ------------- | --------------------------------------- |
| id                 | BIGSERIAL     | 自增主键                                |
| agent_id           | INTEGER       | 外键                                    |
| dev_name           | VARCHAR(255)  | 挂载设备                                |
| mount_point        | VARCHAR(4096) | 挂载点路径 (4096 from <linux/limits.h>) |
| fs_type            | VARCHAR(64)   | 文件系统类型                            |
| total_size         | BIGINT        | 总大小 (KB)                             |
| free_size          | BIGINT        | 空闲大小 (KB)                           |
| avail_size         | BIGINT        | 可用大小 (KB)                           |
| used_size_percent  | SMALLINT      | 挂载点使用率 (n% * 10000)               |
| total_nodes        | BIGINT        | 总node                                  |
| free_nodes         | BIGINT        | 空闲node                                |
| used_nodes_percent | SMALLINT      | node使用率 (n% * 10000)                 |
| time               | TIMESTAMPTZ   | 数据采集时间                            |

SSH登录事件统计表用于记录主机上成功的远程登录事件，认证方式通常为password或者publickey，且publickey情况下会记录用户登录所使用的公钥，该表也设置有外键agent_id关联到主机，并作为时序数据进行存储。

表4-9 SSH登录事件统计
| 字段        | 类型        | 备注                           |
| ----------- | ----------- | ------------------------------ |
| id          | BIGSERIAL   | 自增主键                       |
| agent_id    | INTEGER     | 外键                           |
| username    | VARCHAR(32) | 用户名 (32 from man 8 useradd) |
| remote_host | VARCHAR(64) | 远程主机地址                   |
| auth_method | TEXT        | 使用的认证方式                 |
| time        | TIMESTAMPTZ | 数据采集时间                   |

文件修改删除事件统计表记录各主机上由插件监控的文件的修改删除操作，通常用于监控不应被修改的文件，如SSH的authorized_keys文件，sudo命令对应的/etc/sudoers配置文件等，该表也设置有外键agent_id关联到主机，并作为时序数据进行存储。

表4-10 文件修改删除事件统计
| 字段     | 类型          | 备注                                    |
| -------- | ------------- | --------------------------------------- |
| id       | BIGSERIAL     | 自增主键                                |
| agent_id | INTEGER       | 外键                                    |
| path     | VARCHAR(4096) | 文件路径 (4096 from <linux/limits.h>)   |
| event    | VARCHAR(16)   | 事件类型 (create, modify, delete, move) |
| time     | TIMESTAMPTZ   | 数据采集时间                            |

用户基础信息表存储的是已注册的用户相关信息，主要用于管理员身份认证，防止监控数据被未授权访问，该表无需作为时序数据进行存储。

表4-11 用户基础信息
| 字段       | 类型         | 备注                |
| ---------- | ------------ | ------------------- |
| id         | SERIAL       | 自增主键            |
| email      | VARCHAR(255) | E-mail地址 (unique) |
| username   | VARCHAR(255) | 用户名              |
| password   | VARCHAR(255) | 密码 (哈希)         |
| deleted    | BOOLEAN      | 逻辑删除标识        |
| created_at | TIMESTAMPTZ  | 创建时间            |
| updated_at | TIMESTAMPTZ  | 更新时间            |

规则组是监控规则的集合，方便对监控规则进行统一管理，以规则组为单位设置告警信息接收者，主机可加入到多个规则组中，各规则组的监控规则均会对其生效，该表无需作为时序数据进行存储。

表4-12 规则组
| 字段       | 类型         | 备注         |
| ---------- | ------------ | ------------ |
| id         | SERIAL       | 自增主键     |
| name       | VARCHAR(255) | 规则组名称   |
| deleted    | BOOLEAN      | 逻辑删除标识 |
| created_at | TIMESTAMPTZ  | 创建时间     |
| updated_at | TIMESTAMPTZ  | 更新时间     |

监控规则表存储具体的监控规则，通过外键group_id指明所属的规则组，数据采集模块成功连接上数据存储与处理模块时，数据存储与处理模块中的分析器会从数据库中查询出主机上所应用的规则，作为告警检测依据。

表4-13 监控规则
| 字段       | 类型          | 备注                    |
| ---------- | ------------- | ----------------------- |
| id         | SERIAL        | 自增主键                |
| name       | VARCHAR(255)  | 规则名称                |
| target     | VARCHAR(255)  | 目标 (cpu, mem, load等) |
| addition   | VARCHAR(4096) | 目标额外信息            |
| event      | VARCHAR(255)  | 触发事件                |
| threshold  | INTEGER       | 阈值                    |
| interval   | INTEGER       | 聚合间隔 (m seconds)    |
| silent     | INTEGER       | 静默时间 (m seconds)    |
| level      | VARCHAR(64)   | 等级                    |
| group_id   | INTEGER       | 所属规则组 (外键)       |
| deleted    | BOOLEAN       | 逻辑删除标识            |
| created_at | TIMESTAMPTZ   | 创建时间                |
| updated_at | TIMESTAMPTZ   | 更新时间                |

主机所属规则组记录主机和规则组之间的关联关系，双方属于多对多，一个规则组可以添加多个主机，一个主机也可以加入多个规则组，通过外键agent_id和group_id指明关联的主机和规则组。

表4-14 主机所属规则组
| 字段       | 类型        | 备注            |
| ---------- | ----------- | --------------- |
| id         | SERIAL      | 自增主键        |
| agent_id   | INTEGER     | 对应主机 (外键) |
| group_id   | INTEGER     | 对应规则 (外键) |
| created_at | TIMESTAMPTZ | 创建时间        |
| updated_at | TIMESTAMPTZ | 更新时间        |

接收者存储添加进系统中的所有推送渠道，计划支持邮件，Webhook，短信等，并针对常用的企业IM进行适配，一个接收者被添加进系统中之后，在任意规则组中都可以选择关联，对接收者的修改也会在所有关联的规则组中生效。

表4-15 接收者
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

接收者所属规则组记录接收者和规则组之间的关联关系，双方属于多对多，一个规则组可以添加多个接收者用于告警通知，一个接收者也可以加入多个规则组，通过外键receiver_id和group_id指明关联的接收者和规则组。

表4-16 接收者所属规则组
| 字段        | 类型        | 备注              |
| ----------- | ----------- | ----------------- |
| id          | SERIAL      | 自增主键          |
| receiver_id | INTEGER     | 对应接收者 (外键) |
| group_id    | INTEGER     | 对应规则组 (外键) |
| created_at  | TIMESTAMPTZ | 创建时间          |
| updated_at  | TIMESTAMPTZ | 更新时间          |

告警信息表存储已发送的所有告警消息，每条告警信息都会有关联的主机和监控规则，通过agent_id和rule_id外键指定，同时也加入了group_id外键和继承自对应监控规则的level作为冗余字段，提高查询效率，使得查看告警信息时不需要频繁跨表查询。

表4-17 告警信息
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

系统设置表以key-value形式存储系统首选项，例如是否禁止其他用户注册等，虽然系统中用到了redis，但其仅作为非持久性存储保存用户session，系统设置项更适合与其它持久性数据存放在一起，方便整体进行备份。

表4-18 系统设置
| 字段  | 类型         | 备注     |
| ----- | ------------ | -------- |
| id    | SERIAL       | 自增主键 |
| key   | VARCHAR(255) | 配置项键 |
| value | VARCHAR(255) | 配置项值 |

### 4.5 本章小结
系统设计是系统实现的前置步骤，本章先对监控告警系统整体架构进行剖析，明确了系统的模块划分及各模块承担的任务，然后讨论了各个模块的实现思路以及实现过程中可能遇到的问题，并对相关问题给出了解决方案，最后给出数据库E-R图，根据E-R图设计出了数据库的具体表结构。通过系统设计提前规划好了系统实现的具体方法，为下一章的系统实现做好充足准备。

## 5. 系统实现

### 5.1 数据采集模块
数据采集模块大量的使用了Golang提供的goroutine，如各个监控项的数据采集都是在各自的goroutine中并行采集，WebSocket发送数据包也有单独的goroutine统一发送，避免多个goroutine中向同一WebSocket并发写入触发panic。不同goroutine之间通过channel传递数据，然后利用context机制在主程序退出前通知所有goroutine安全结束，利用sync.WaitGroup使主程序等待所有goroutine结束之后再退出，模块核心流程如图5-1。

图5-1 数据采集模块核心流程图

各监控采集goroutine均由主程序启动，并行运行，互不干扰。对于每个监控项的数据采集，其整体流程较为相似，此处以CPU利用率采集过程为例，其流程图如图5-2。

图5-2 CPU利用率采集流程图

其中触发事件部分用到了Golang提供的select语句，该语句可以阻塞等待多个条件中的某一个被触发后执行对应的分支，程序中使用select阻塞等待结束信号和计时器信号，按计时器设定的触发间隔采集监控信息，若接收到结束信号则主动结束当前goroutine并退出，避免主程序退出后还有goroutine尚未结束。  
数据采集goroutine中并不执行具体的发送操作，而是将采集到的监控数据写入channel，程序中分配了一个容量为64的channel作为数据缓冲区，由单独的goroutine负责从缓冲区中读取数据进行发送，避免并发写入同一WebSocket连接触发panic。缓冲区满时会导致向channel中写入数据超时，将触发数据采集模块的panic，配合systemd机制数据采集模块将自动重启，重新建立与数据存储模块的WebSocket连接。  
负责发送数据的goroutine会将原数据封装为metadata，添加unix时间戳，并利用Golang的反射机制reflect.TypeOf(interface).Name()获取监控项数据类型后为数据包加上category，最后转换为JSON通过WebSocket发送出去。  
除了类似的数据采集goroutine之外，还会有一个插件goroutine监听本地端口，提供HTTP接口供插件上报信息。插件通过POST方式提交事件，事件原始数据使用JSON进行编码，放在POST请求的body中，会被插件goroutine封装为metadata，插件goroutine从form value中读取category并加上时间戳，最后写入缓冲区。  
SSH远程登录事件插件利用的是Linux的PAM机制，PAM模块的类型有四种：account，auth，password和session，其中account是根据帐号管理设置进行检查，如当前时间段是否限制用户登录，是否达到系统允许同时登录的用户数量限制等；auth是常规的用户认证，检查用户名和密码是否匹配；password用于更新用户认证信息；session在用户登录前和退出后执行相关操作。插件目的是在SSH登录成功后进行事件上报，因此选用session即可，根据GitHub上的一个同类项目uber/pam-ussh及官方教程，程序中需要用Golang实现pam_sm_open_session()和pam_sm_close_session()两个入口函数，并利用cgo的export导出，获取监控信息的部分需要使用C语言封装完成后给Golang调用，pam_get_user()用来获取登录用户，pam_getenv()从SSH_AUTH_INFO_0中读取登录方式，pam_get_item()可以获取登录用户的IP地址。最终使用go build -o pam_sshd_log.so -buildmode=c-shared ./plugin/pam_sshd_log进行编译，上报地址默认为本机的8001端口，也可从命令行参数中动态读取，命令行参数在/etc/pam.d/sshd配置文件中进行设置。除此之外，程序中函数返回值为PAM_IGNORE，在/etc/pam.d/sshd文件中该模块被设置为optional，保证数据采集模块未启动时的事件上报失败不影响用户正常登录。  
文件修改删除事件监控插件用到的是Linux内核提供的inotify API，为完整监听到目标文件的所有操作，程序需要一同监听上层目录发生的inotify事件，并动态更新当前的监听列表，该插件流程图如图5-3。

图5-3 文件修改删除事件监控插件流程图

### 5.2 数据存储与处理模块
数据存储与处理模块是本监控告警系统的核心部分，它需要连接TimescaleDB和redis数据库，为数据采集模块提供WebSocket接口，为可视化模块提供HTTP接口，同时还要负责监控与告警，该模块启动时的初始化流程如图5-4。

5-4 数据存储与处理模块初始化流程

初始化过程中的任意步骤失败都会导致程序抛出异常，供运维人员排查问题。程序中通过async/await语法处理异步函数的先后关系，相比层层嵌套的回调函数使得程序逻辑更加清晰，增强了程序的可维护性。同时遵循模块化原则，自定义的config模块可从默认位置，环境变量，以及命令行参数中读取指定配置文件，方便灵活部署；自定义的pid模块在指定位置保存程序运行pid，并在程序结束时主动删除，方便运行过程中监控服务进程状态；还自定义了logger模块格式化程序日志，提高开发时的debug效率。  
WebSocket服务相对独立，HTTP服务器收到的所有upgrade请求都会交给WebSocket服务进行处理，握手阶段会进行一系列校验，校验成功后才正式建立连接，等待接收数据采集模块发送过来的数据包，对于每个数据包都会返回pong响应，供数据采集模块判断网络状况，若数据采集模块连续发送的多个数据包都未收到响应，则主动进行重启，等待Nginx重新分配数据存储与处理模块。WebSocket服务的具体响应流程如图5-5。

图5-5 WebSocket服务响应流程

WebSocket握手过程中使用的token是数据采集模块在首次运行时访问注册接口得到的。数据采集模块首先通过注册接口上报machine-id，machine-id完整长度为128比特位，表示成小写十六进制后长度为32个字符，Node.js随机生成16字节的buffer作为iv，配置文件中指明32字节的key，使用AES-256-CFB算法进行加密，加密后将16字节的iv拼接加密结果进行base64编码后得到token，最终token长度为固定44字符。逆向解析token时先base64解码，取前16字节作为buffer，然后使用AES-256-CFB配合配置文件中的key解出machine-id。  
数据存储与处理模块对外提供一个/status接口用于查看服务状态，其中包括5分钟内处理请求数，当前活跃WebSocket连接数，以及该模块当前所运行的版本及监听端口。5分钟内请求数通过5个空位的对象数组进行统计，每个位置上的对象记录有unix时间戳计算出的分钟及该分钟内处理的请求数，初始状态下一个指针指向数组开头对象，若新请求的分钟时间戳与指针指向对象相同，则指针指向对象的请求数加一，否则指针后移一位，指针指向对象的请求数置为1，同时更新该对象的时间。指针移动到数组末尾时下一次将回到开头，查询时将数组中时间戳在5分钟内的对象的请求数相加得到5分钟内处理请求数。  
提供给前端可视化模块的API保证返回JSON结果，例如对于请求数据的API返回JSON格式具体数据，对于请求成功的API返回{"result":"success"}，对于请求失败的API返回{"error_type":"错误原因"}。API请求方法上将GET用于获取数据，POST用于新建数据，PUT用于修改已有数据，DELETE用于删除数据。前端可视化模块通常按时间范围批量请求监控数据，当时间范围较大时，数据点的数量将变得过于庞大，因此数据存储模块用到了时序数据库TimescaleDB提供的time_bucket()聚合函数对数据点进行压缩，在不影响整体趋势的前提下减少数据点数量。例如查询最近12小时内1号主机的CPU利用率，假设限定单次请求最多返回200个数据点，则收到请求后首先要根据请求中的时间范围计算出总时长12小时，除以200得到区间长度216秒，然后查询时利用time_bucket('216 seconds', time)压缩数据，最终查询使用的SQL为：
```sql
SELECT
  time_bucket('216 seconds', time) as bucket_time,
  floor(avg(used_percent)) as used_percent
FROM cpuinfos
WHERE agent_id = 1 AND time BETWEEN '2020-05-20 10:00' AND '2020-05-20 22:00'
GROUP BY bucket_time
ORDER BY bucket_time
```

### 5.3 前端可视化模块
前端可视化模块使用React前端框架进行编写，React自16.8版本开始支持Hooks，其改进了原本组件编写的一些不足之处，如难以复用状态逻辑，复杂组件中生命周期方法混乱等，官方也更加推荐使用新的Hooks编写组件，因此学习过程中主要了解的就是Hooks，程序实现时也使用的Hooks。  
前端代码中实际的业务流程并不多，更多的是对用户使用体验的优化，例如注册登录功能这里，程序中将登录注册合并为一个页面，通过URL中的hash区分不同标签页，用户点击标签栏可以进行切换，起初每次切换都是通过修改window.location.hash实现，导致浏览器的window.history中不停增加历史记录，用户在浏览器中点击返回按钮就会一直停留在这一个页面上，后来更加深入地了解了window.history后，将其修改为window.history.replaceState()，此时在登录注册页面切换标签就不会再影响浏览历史，点击返回按钮就可以返回真正的前一页面。还有在用户点击登录或注册按钮后，为防止重复提交表单，前端会将按钮修改为不可操作状态，同时在旁边显示加载动画，给出合理的视觉反馈，前端收到服务器响应后再恢复按钮为可点击状态，加载动画变为成功或失败标识，并在失败时给出具体原因。对邮箱和密码的格式除了在后端进行校验外，前端也增加一次校验，点击提交按钮后格式校验通过再向后端发请求，格式错误会在前端立即返回，减少用户等待时间。  
主机列表页面会显示各主机所用发行版的logo来提高辨识度，像logo这样几乎不会再发生修改的静态资源，如果按照官方说明使用Webpack的file-loader进行处理，那么每次编译都会重复处理一次，因此程序中将提前处理好的logo放入public目录下，然后封装为绝对路径的img标签组件供页面调用，编译过程中只需原样复制，可以节省每次Webpack编译的处理时间。除了常见的发行版logo之外，public目录下还存放有favicon.png作为网站标识，robots.txt禁止搜索引擎爬虫爬取所有页面，也通过CopyPlugin来跳过Webpack的进一步处理。  
常用页面使用Material-UI提供的Grid栅格布局实现了自适应调整，页面宽度足够时在一行里显示多组内容，随着宽度的减小会将部分内容移至下一行，避免内容过于拥挤。控制台入口的概览页面在不同宽度下的显示效果如图5-6和5-7。

图5-6 概览页面完整宽度效果

图5-7 概览页面窄屏幕下效果

主机详情页是前端可视化模块中数据最为复杂的页面，最上方是主机的所有信息项，同时给出主机相关监控规则及主机相关警报入口，允许对主机执行删除恢复操作，被删除的主机依旧会记录监控数据，但不再展示在主机列表中，管理员可在回收站中找到所有被删除的主机。主机信息项之下是时间范围切换栏，默认选择1小时，提供6小时，12小时，1天，3天，7天，14天的快速切换选项，同时支持自定义时间范围，切换时间范围后会触发监控数据的刷新。各监控项数据图表位于时间范围切换栏之下，通过Chart.js的折线图展示其变化趋势，包含CPU使用率，内存利用率，系统平均负载，网络传输速率，网络传输数据包数，磁盘读写速率，磁盘读写请求数。最下方是挂载点统计，SSH登录事件，和文件修改删除事件。Chart.js的默认折线图在鼠标指向数据点之后才显示其详细信息，当数据点较密集时用户难以选中关注的数据点，因此在原折线图的基础上添加了一条垂直于X轴的跟踪线，跟随鼠标移动，并始终显示跟踪线与折线交点的详细信息。主机详情页实际效果如图5-8。

图5-8 主机详情页实际效果

### 5.4 本章小结
按照上一章系统设计的功能模块划分，本章依次介绍了数据采集模块，数据存储与处理模块，和前端可视化模块的具体实现，搭配流程图介绍了关键部分的代码逻辑，并对一些重要流程进行了说明，如数据采集模块的插件上报，数据存储与处理模块的token生成，前端可视化模块的静态资源优化等，接下来将对系统实现的成果进行测试。

## 6. 系统测试
为模拟监控告警系统真实运行状况，尝试将本系统部署到了阿里云的一台单核2G内存的服务器上，使用Docker提供TimescaleDB和redis数据库，Nginx作为负载均衡，此服务器上共启动一个数据采集模块，两个数据存储与处理模块，和一个前端可视化模块。另外在两台其它云服务商的服务器上也部署了数据采集模块，7x24小时不间断上报数据，本地配置了三个虚拟机用于测试数据采集模块在常见的CentOS 7，CentOS 8，Ubuntu 18.04 LTS系统上的兼容性，数据采集模块上报间隔均设置为10秒。

### 6.1 数据量测试
Docker中TimescaleDB时序数据库的数据存储目录为/var/lib/postgresql，导入完数据表结构后该目录大小为50M，2020.5.13到2020.5.26运行约两周，数据存储目录大小增长到561M，查询CPU占用统计表，数据共342868条，所以按监控数据保留3个月的话，单台主机需要约1G的硬盘空间，完全可以接受。  
除磁盘空间外，从监控系统中可以看到阿里云服务器的内存占用一直在增长，从起始的40%增长到了75%，重启数据库后恢复到了58%，然后继续缓慢增长。查看官方TimescaleDB的Docker仓库，发现其对配置文件的初始化有相关说明：首次利用Docker启动TimescaleDB时，会使用timescaledb-tune命令获取主机内存和CPU核数自动调整数据库配置，其默认主机上只运行了该数据库服务，因此会尽量分配所有资源。而实际当前服务器上有多个服务共存，所以使用timescaledb-tune生成推荐配置时需要限制其最大可用内存。修改后重启数据库，内存占用恢复到了47%，最终稳定到52%，内存占用变化过程如图6-1。

图6-1 内存占用变化图

### 6.2 负载均衡测试
服务器上同时启动有两个数据存储与处理模块，分别监听本地3000和3001端口，使用Nginx进行负载均衡，当新请求到达Nginx时，Nginx会根据已有连接数均衡分配任务，防止Node.js单进程的处理方式浪费CPU多核资源。数据存储与处理模块留有/status接口，可以通过该接口判断对应进程压力。共启动四个数据采集模块，重复多次浏览主机历史数据后，使用curl命令访问本地127.0.0.1:3000/status和127.0.0.1:3001/status查看各自进程处理压力，结果如图6-2。

图6-2 双数据存储与处理模块负载均衡

可以看到两者各自承担了两个数据采集模块的上报任务，近5分钟内处理请求数分别为291和283，Nginx的负载均衡效果十分显著。接下来关闭3001端口上的数据存储与处理模块，查看其中一个模块宕机后Nginx能否将新的请求转发给正常运行的模块，结果如图6-3。

图6-3 双数据存储与处理模块宕机切换

原本与3001端口上存储模块连接的两个数据采集模块异常断开WebSocket连接后，重新发起的连接都被正常运行的3000模块所接受，在前端浏览主机历史数据，各查询请求均收到正常响应，因此数据存储与处理模块利用Nginx负载均衡实现的宕机切换无问题。

### 6.3 告警测试
新建告警测试规则组，添加CPU告警规则和系统负载告警规则，调小阈值便于触发，CPU阈值设置为20%，系统一分钟内平均负载阈值设置为1.00，静默时间设置为30秒，添加邮件推送，企业微信推送，钉钉推送和飞书推送，将本地开发机加入该规则组，最后在本地开发机上启动数据采集模块，并运行简单的编译任务触发告警。  
数据采集模块首次上传监控数据后立即触发告警，邮箱及各企业IM收到推送，随后30秒内的监控数据不再触发告警，30秒后触发第二次告警，移动端收到的告警信息如图6-4，各渠道均正常推送。

图6-4 各渠道告警通知

### 6.4 历史数据查询测试
在主机列表页面选择主服务器查看主机详情，历史监控数据时间范围调整为近14天，在浏览器开发者工具中得到网络请求基本信息如图6-5。

图6-5 近14天历史数据相关网络请求

以CPU使用率为例，在数据库中查询得近14天内该主机共约11万条CPU监控数据，后端以201分钟为单位区间进行聚合后得到95个数据点，响应大小共5.36KB，经过Nginx的gzip压缩后实际通过网络传输的数据量仅为0.98KB，网络传输时间加上请求处理时间共482毫秒，且请求过程中多个数据项查询均为并行处理，历史数据查询效率较高。

### 6.5 本章小结
本章系统测试选取了该监控告警系统最为关键的几个方面进行测试，包括数据增长速度，多存储模块负载均衡及宕机切换，告警信息多渠道推送，较大时间范围内的历史数据聚合，通过这些测试检验了系统实现的成果，确保了该监控告警系统能够在真实的运行环境下正常运作。

## 7. 总结与展望

## 致谢