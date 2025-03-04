#include "debug.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "voice_record.h"
#include "command.h"

// 一般情况下，除非添加 usermod，否则需要sudo权限。
#define VOICE_RECORD_CMD "arecord -r 16000 -f S16_LE -t wav -V stereo -c 1 -d "
#define SPLICING_AUDIO_CMD "sox "

/**
 * 拼接多个音频文件。
 *
 * @param file_prefix 语音文件的前缀（拼接多个文件时使用相同前缀）。
 * @param voice_count 需要拼接的音频文件数量。
 * @param out 输出拼接后的音频文件路径。
 * @return 成功返回 0，失败返回 -1。
 */
int splicing_audio(const char* file_prefix, size_t voice_count, const char* out)
{
    std::string splicing = "";
    for (size_t i = 0; i < voice_count; ++i) {
        splicing += file_prefix + std::to_string(i) + ".wav ";
    }
    std::string output;
    std::string cmd = SPLICING_AUDIO_CMD;
    cmd += splicing;
    cmd += out;
    cmd += " 2>&1";

    return execute_command(cmd.c_str(), output);
}

/**
 * 录制语音并保存到文件。
 *
 * @param save_file 录音保存的文件路径。
 * @param record_time 录音时长（单位：秒）。
 * @return 成功返回 0，失败返回 -1。
 */
int voice_record(const char* save_file, size_t record_time)
{
    std::string output;
    std::string cmd = VOICE_RECORD_CMD;
    cmd += std::to_string(record_time) + " ";
    cmd += save_file;
    cmd += " 2>&1";

    return execute_command(cmd.c_str(), output);
}

// date 3h26min