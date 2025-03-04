#include "debug.h"
#include "whisper.h"
#include <iostream>
#include <thread>
#include <vector>


/**
 * Whisper 回调函数类型定义。
 *
 * @param text 识别到的文本代码。
 * @param code 处理后的文本代码。
 * @param userdata 用户自定义数据指针。
 */
void whisper_callback(const char *text, const char* code, void* userdata)
{
    size_t *count = (size_t *)userdata;
    // LOG_DBG("[%zu] get code: %s", *count, code);
    std::cout << "[" << *count << "] " << "get text: '" << text << "' code: " << code << std::endl;
}

// 一个简单的任务函数，模拟你要执行的程序
void whisper_task(void* userdata)
{
    whisper_t* w = (whisper_t*)userdata;
    std::cout << "Task whisper is running on thread " << std::this_thread::get_id() << std::endl;
    size_t count = 0;

    while (1) {
        int ret = whisper(w, whisper_callback, &count);
        LOG_DBG("whisper ret: %d", ret);
        ++count;
    }

    std::cout << "Task whisper completed" << std::endl;
}

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        LOG_ERR("use: %s config.json", argv[0]);
        return -1;
    }
    const int numThreads = 1; // 假设我们要开启1个线程
    std::vector<std::thread> threads;

    whisper_t* w = whisper_init(argv[1]);
    if (!w) {
        LOG_ERR("fail to init whisper");
        return -1;
    }

    // 启动多个线程，每个线程运行 task 函数
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(whisper_task, w)); // 启动线程并传递参数
    }

    // 等待所有线程完成
    for (auto& th : threads) {
        th.join(); // 等待每个线程完成
    }
    whisper_exit(w);

    std::cout << "All tasks are completed." << std::endl;

    return 0;
}
