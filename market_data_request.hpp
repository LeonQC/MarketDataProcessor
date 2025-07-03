#pragma once

#include <string>

struct Task {
    std::string exchange;
    std::string base;
    std::string quote;
};

// 核心功能
void fetch_and_produce_price(const Task& t);

// Kafka producer 生命周期
void init_kafka_producer();
void close_kafka_producer();
