#include <iostream>
#include <thread>
#include <vector>

#include "whisper_fuzzy.h"
#include "debug.h"

/**
 * Whisper 回调函数类型定义。
 *
 * @param leat_count 识别到的文本剩余数量
 * @param text 识别到的文本代码。
 * @param code 处理后的文本代码。
 * @param userdata 用户自定义数据指针。
 */
static int whisper_user_callback(size_t leat_count, const char* text, const char* code, void* userdata)
{
    // skip.
    if (leat_count) {
        return 1;
    }
    if (!text || !code || !userdata) {
        LOG_ERR("args fail! text(%p), code(%p), userdata(%p)", text, code, userdata);
        return -1;
    }
    size_t &count = *(size_t *)userdata;
    ++count;

    LOG_INFO("[%zu] get text: %s, code: %s", count, text, code);

    return 0;
}

// 一个简单的任务函数，模拟你要执行的程序
void whisper_fuzzy_task(void* userdata)
{
    whisper_fuzzy_t* w = (whisper_fuzzy_t*)userdata;
    std::cout << "Task whisper is running on thread " << std::this_thread::get_id() << std::endl;
    size_t count = 0;

    int ret = whisper_fuzzy(w, whisper_user_callback, &count);
    LOG_INFO("whisper ret: %d", ret);

    std::cout << "Task whisper completed" << std::endl;
}

int main(int argc, char const* argv[])
{
    const int numThreads = 1; // 假设我们要开启1个线程
    std::vector<std::thread> threads;

    whisper_fuzzy_t* w = whisper_fuzzy_init(argc, argv);
    if (!w) {
        LOG_ERR("fail to init whisper");
        return -1;
    }

    // 启动多个线程，每个线程运行 task 函数
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(whisper_fuzzy_task, w)); // 启动线程并传递参数
    }

    // 等待所有线程完成
    for (auto& th : threads) {
        th.join(); // 等待每个线程完成
    }

    std::cout << "clear fuzzy." << std::endl;

    whisper_fuzzy_exit(w);

    std::cout << "All tasks are completed." << std::endl;

    return 0;
}
