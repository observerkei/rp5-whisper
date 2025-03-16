#ifndef __WHISPER_STREAM_H__
#define __WHISPER_STREAM_H__

#include <thread>
#include <string>

#include "stdint.h"
#include "whisper_fuzzy.h"


/**
 * 结构体：whisper_params_t
 * 用于存储命令行参数配置，影响 Whisper 语音处理的行为。
 */
struct whisper_params_t {
    // 线程数量，默认为 4，或不超过硬件支持的最大线程数。
    int32_t n_threads  = std::min(4, (int32_t) std::thread::hardware_concurrency()); 
    
    int32_t step_ms    = 1000;  // 每次音频处理的步长（毫秒）。
    int32_t length_ms  = 3000;  // 每次音频片段的长度（毫秒）。
    int32_t keep_ms    = 100;   // 处理时保留的音频长度（毫秒）。
    int32_t capture_id = -1;    // 录音设备 ID，默认为 -1（未指定）。
    int32_t max_tokens = 8;     // 每个音频片段允许的最大 Token 数量。
    int32_t audio_ctx  = 0;     // 音频上下文大小，0 表示全部。

    float vad_thold    = 0.6f;  // 语音活动检测（VAD）的阈值。
    float freq_thold   = 100.0f;// 高通滤波的截止频率（Hz）。

    bool translate     = false; // 是否将源语言翻译为英语。
    bool no_fallback   = false; // 是否禁用温度回退机制。
    bool print_special = false; // 是否输出特殊标记。
    bool no_context    = true;  // 是否在音频块之间保留上下文（默认不保留）。
    bool no_timestamps = false; // 是否去除时间戳。
    bool tinydiarize   = false; // 是否启用 TinyDiarize（需要 tdrz 模型）。
    bool save_audio    = false; // 是否将录制的音频保存到文件。
    bool use_gpu       = true;  // 是否启用 GPU 加速。
    bool flash_attn    = false; // 是否在推理时使用 Flash Attention。

    // 语音的语言，默认为英语。
    std::string language  = "en"; 
    // 模型文件路径。
    std::string model     = "models/ggml-base.en.bin"; 
    std::string user      = ""; // 用户配置文件路径。
    std::string fname_out;      // 输出文件名。
    const char *program_name;   // 程序名称。
};

/**
 * 打印命令行参数的使用说明。
 *
 * @param params 传入的 whisper_params_t 结构体，包含所有可用的命令行参数及其默认值。
 * 
 * 该函数用于向用户展示可用的命令行选项及其默认值，帮助用户正确配置 Whisper 运行参数。
 */
void whisper_print_usage(const whisper_params_t & params);

/**
 * 运行 Whisper 语音流处理的主函数。
 *
 * @param whisper_fuzzy_ctx 指向 whisper_fuzzy_t 结构体的指针，包含语音处理的上下文信息。
 * @return 成功返回 0，失败返回 -1。
 *
 * 该函数负责管理 Whisper 的音频流处理，调用相关的语音识别和匹配功能，
 * 以便实时处理输入音频数据并执行模糊匹配任务。
 */
int whisper_stream_main(whisper_fuzzy_t *whisper_fuzzy_ctx);

#endif //__WHISPER_STREAM_H__