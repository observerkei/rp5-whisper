#ifndef __WHISPER_FUZZY_H__
#define __WHISPER_FUZZY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>
#include <cstddef>

/**
 * Whisper 结构体的前向声明。
 */
struct whisper_fuzzy_t;
struct whisper_params_t;

/**
 * Whisper 回调函数类型定义。
 *
 * @param leat_count 识别到的文本剩余数量, 0表示最后一个
 * @param text 识别到的文本代码。
 * @param code 处理后的文本代码。
 * @param userdata 用户自定义数据指针。
 */
typedef int (*whisper_callback_t)(size_t leat_count, const char *text, const char* code, void* userdata);

/**
 * 获取解析参数
 * @param w 指向 whisper_fuzzy_t 结构体的指针。
 * @return 成功返回指向 whisper_params_t 结构的指针
 */
whisper_params_t *whisper_fuzzy_get_params(whisper_fuzzy_t *w);

/**
 * 初始化 Whisper 组件。
 *
 * @param argc 参数个数
 * @param argv 参数列表
 * @return 成功返回指向 whisper_fuzzy_t 结构的指针，失败返回 NULL。
 */
whisper_fuzzy_t* whisper_fuzzy_init(int argc, char const* argv[]);

/**
 * 释放 Whisper 组件并清理资源。
 *
 * @param w 指向要释放的 whisper_fuzzy_t 结构体指针。
 */
void whisper_fuzzy_exit(whisper_fuzzy_t* w);

/**
 * 处理 Whisper 任务。
 *
 * @param w 指向 whisper_fuzzy_t 结构体的指针。
 * @param callback 处理完成后的回调函数。
 * @param userdata 传递给回调函数的用户数据。
 * @return 成功返回 0，失败返回 -1。
 */
int whisper_fuzzy(whisper_fuzzy_t* w, whisper_callback_t callback, void* userdata);

/**
 * 进行模糊匹配流程。
 *
 * @param w 指向 whisper_fuzzy_t 结构体的指针。
 * @param leat_count 允许的最小匹配字符数。
 * @param text 需要匹配的文本字符串。
 * @return 成功返回匹配的评分值，失败返回 -1。
 */
int whisper_fuzzy_match(whisper_fuzzy_t* w, size_t leat_count, const char *text);


#ifdef __cplusplus
}
#endif

#endif //__WHISPER_FUZZY_H__