#ifndef __WHISPER_H__
#define __WHISPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RECODE_TIMEOUT (5) // 有声录制最大时长 单位s
#define SILENCE_THRESHOLD (-25) // 环境音响度(db) 用于区分是否有人说话, 值越大声音越大
#define WHISPER_SERVER_PORT (8090) // 语音识别服务端口

/**
 * Whisper 结构体的前向声明。
 */
struct whisper_t;

/**
 * Whisper 回调函数类型定义。
 *
 * @param text 识别到的文本代码。
 * @param code 处理后的文本代码。
 * @param userdata 用户自定义数据指针。
 */
typedef void (*whisper_callback_t)(const char *text, const char* code, void* userdata);

/**
 * 初始化 Whisper 组件。
 *
 * @param config 配置文件路径或配置字符串。
 * @return 成功返回指向 whisper_t 结构的指针，失败返回 NULL。
 */
whisper_t* whisper_init(const char* config);

/**
 * 释放 Whisper 组件并清理资源。
 *
 * @param w 指向要释放的 whisper_t 结构体指针。
 */
void whisper_exit(whisper_t* w);

/**
 * 处理 Whisper 任务。
 *
 * @param w 指向 whisper_t 结构体的指针。
 * @param callback 处理完成后的回调函数。
 * @param userdata 传递给回调函数的用户数据。
 * @return 成功返回 0，失败返回 -1。
 */
int whisper(whisper_t* w, whisper_callback_t callback, void* userdata);

#ifdef __cplusplus
}
#endif

#endif //__WHISPER_H__