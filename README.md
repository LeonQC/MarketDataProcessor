# MarketDataProcessor
Cryptocurrency Trading Market Data Processor

## 项目简介
这是一个加密货币市场数据处理器，支持从多个交易市场获取实时价格数据并发送到Kafka消息队列。

## 功能特性
- 支持多个交易市场：Bitmart、Crypto.com
- 监控热门加密货币：BTC、ETH、BNB、ADA、SOL
- 实时数据获取和Kafka消息发送
- 可配置的调度系统
- 线程安全的任务管理

## 编译和运行

### 编译
```bash
make
```

### 运行主程序
```bash
make run
```

或者直接运行：
```bash
./market_data_processor
```

### 清理编译文件
```bash
make clean
```

## 程序功能

### 主程序 (main.cpp)
- 自动为两个交易市场（bitmart、cryptocom）的5种热门加密货币创建数据获取任务
- 每个任务每5秒执行一次
- 总共创建10个并发任务（2个市场 × 5种加密货币）
- 支持优雅的程序关闭

### 监控的加密货币
- BTC (Bitcoin)
- ETH (Ethereum)  
- BNB (Binance Coin)
- ADA (Cardano)
- SOL (Solana)

### 支持的交易市场
- Bitmart
- Crypto.com

## 测试程序

### Kafka测试
```bash
cd test
g++ -o kafka_test kafka_test.cpp ../market_data_request.cpp ../scheduler.cpp -lrdkafka++ -lrdkafka -lpthread
./kafka_test
```

### 调度器测试
```bash
cd test
g++ -o scheduler_test scheduler_test.cpp ../scheduler.cpp -lpthread
./scheduler_test
```

### 数据获取测试
```bash
cd test
g++ -o fetch_test fetch_test.cpp ../market_data_request.cpp -lrdkafka++ -lrdkafka
./fetch_test
```

## 依赖项
- librdkafka (Kafka客户端库)
- pthread (线程库)
- C++17 编译器

## 项目结构
```
MarketDataProcessor/
├── main.cpp                 # 主程序入口
├── market_data_request.hpp  # 市场数据请求接口
├── market_data_request.cpp  # 市场数据请求实现
├── scheduler.hpp           # 调度器接口
├── scheduler.cpp           # 调度器实现
├── Makefile               # 编译配置
├── README.md              # 项目说明
└── test/                  # 测试程序
    ├── kafka_test.cpp
    ├── scheduler_test.cpp
    └── fetch_test.cpp
```

## 使用说明
1. 确保已安装librdkafka库
2. 配置Kafka连接参数（在market_data_request.cpp中）
3. 编译程序：`make`
4. 运行程序：`make run`
5. 程序将自动开始获取市场数据并发送到Kafka

## 注意事项
- 程序需要稳定的网络连接
- 确保Kafka服务器正在运行
- 程序可以通过Ctrl+C优雅关闭
