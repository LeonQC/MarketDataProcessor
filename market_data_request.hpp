#pragma once

#include <string>

// 用于发起请求和发送 Kafka 的主逻辑函数
struct Task {
    std::string exchange;
    std::string base;
    std::string quote;
};

void fetch_and_produce_price(const Task& t);
