#include "market_data_request.hpp"
#include "scheduler.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

using namespace std;

int main() {
    // 初始化Kafka生产者
    init_kafka_producer();
    
    // 定义最热门的五种加密货币
    vector<string> popular_cryptos = {"BTC", "ETH", "XRP", "SOL"};
    
    // 定义交易市场
    vector<string> exchanges = {"bitmart", "cryptocom"};
    
    // 创建调度器
    Scheduler scheduler{};
    
    
    std::cout << "=== 市场数据处理器启动 ===" << std::endl;
    std::cout << "支持的交易市场: ";
    for (const auto& exchange : exchanges) {
        std::cout << exchange << " ";
    }
    std::cout << std::endl;
    
    std::cout << "监控的加密货币: ";
    for (const auto& crypto : popular_cryptos) {
        std::cout << crypto << " ";
    }
    std::cout << std::endl;
    
    // 为每种加密货币创建任务，同时获取所有交易所的数据，但不同货币错开时间

    for (const auto& crypto : popular_cryptos) {

        this_thread::sleep_for(chrono::milliseconds(2000)); // 每个货币延迟1秒
        scheduler.addTask([crypto, exchanges]() {
            // 第一次执行时延迟启动，错开不同货币的执行时间

            
            std::cout << "=== 同时获取 " << crypto << "/USDT 在所有交易所的价格 ===" << std::endl;
            
            // 为每个交易所创建任务
            for (const auto& exchange : exchanges) {
                Task task;
                task.exchange = exchange;
                task.base = crypto;
                task.quote = "USDT";
                
                std::cout << "获取 " << task.exchange << " 市场 " 
                         << task.base << "/" << task.quote << " 价格数据" << std::endl;
                fetch_and_produce_price(task);
            }
            
            std::cout << "=== " << crypto << "/USDT 价格获取完成 ===" << std::endl;
        }, 10000);
        
        std::cout << "已添加任务: 同时获取 " << crypto << "/USDT 在所有交易所" << std::endl;
    }
    scheduler.start();
    
    std::cout << "总共创建了 " << popular_cryptos.size() 
              << " 个数据获取任务" << std::endl;
    std::cout << "数据获取间隔: 5秒" << std::endl;
    std::cout << "按 Ctrl+C 停止程序" << std::endl;
    
    // 主程序运行，等待用户中断
    try {
        while (true) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cout << "程序异常: " << e.what() << std::endl;
    }
    
    std::cout << "=== 正在关闭程序 ===" << std::endl;
    
    // 停止调度器
    scheduler.stop();
    
    // 清理Kafka资源
    close_kafka_producer();
    
    std::cout << "=== 程序已安全关闭 ===" << std::endl;
    
    return 0;
} 