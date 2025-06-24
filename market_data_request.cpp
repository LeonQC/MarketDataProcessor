#include "market_data_request.hpp"
#include <iostream>
#include <curl/curl.h>
#include <librdkafka/rdkafkacpp.h>

// Kafka producer 全局指针（可在主线程初始化一次）
static RdKafka::Producer* producer = nullptr;
static std::string topic_name = "price_topic";

// 回调：写入 libcurl 返回的 HTTP body 数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

// 发送消息到 Kafka
void produce_to_kafka(const std::string& topic, const std::string& message) {
    if (!producer) {
        std::cerr << "Kafka producer not initialized.\n";
        return;
    }

    RdKafka::ErrorCode err = producer->produce(
        topic,
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char*>(message.c_str()),
        message.size(),
        nullptr, 0,
        0, nullptr
    );

    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Kafka produce failed: " << RdKafka::err2str(err) << std::endl;
    }

    producer->poll(0);
}

// 主函数：构造 URL、请求、发送 Kafka
void fetch_and_produce_price(const Task& t) {
    // 构造 URL
    std::string url = "http://localhost:8000/price/" + t.base + "/" + t.quote +
                      "?exchange=" + t.exchange;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to init CURL.\n";
        return;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);  // 最多等 5 秒

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        std::cout << "[✔] Got response from " << t.exchange << " " << t.base << "/" << t.quote << "\n";
        produce_to_kafka(topic_name, response);
    } else {
        std::cerr << "[✘] CURL error: " << curl_easy_strerror(res) << "\n";
    }

    curl_easy_cleanup(curl);
}

// Kafka 初始化函数（main.cpp 中调用一次即可）
void init_kafka_producer() {
    std::string errstr;
    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    conf->set("bootstrap.servers", "localhost:9092", errstr);
    producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create Kafka producer: " << errstr << "\n";
        exit(1);
    }

    delete conf;
}

// Kafka 清理函数（main.cpp 结束时调用）
void close_kafka_producer() {
    if (producer) {
        producer->flush(3000);  // 等待最多3秒
        delete producer;
        producer = nullptr;
    }
}
