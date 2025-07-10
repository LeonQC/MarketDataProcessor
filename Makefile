CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lrdkafka++ -lrdkafka -lpthread -lcurl

# 源文件
SOURCES = main.cpp market_data_request.cpp scheduler.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 目标文件
TARGET = market_data_processor

# 默认目标
all: $(TARGET)

# 编译主程序
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 编译规则
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 运行
run: $(TARGET)
	./$(TARGET)

# 测试编译
test: clean all

.PHONY: all clean run test 