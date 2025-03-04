#ifndef __VOICE_RECORD_H__
#define __VOICE_RECORD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/**
 * 录制语音并保存到文件。
 *
 * @param save_file 录音保存的文件路径。
 * @param record_time 录音时长（单位：秒）。
 * @return 成功返回 0，失败返回 -1。
 */
int voice_record(const char* save_file, size_t record_time);

/**
 * 拼接多个音频文件。
 *
 * @param file_prefix 语音文件的前缀（拼接多个文件时使用相同前缀）。
 * @param voice_count 需要拼接的音频文件数量。
 * @param out 输出拼接后的音频文件路径。
 * @return 成功返回 0，失败返回 -1。
 */
int splicing_audio(const char* file_prefix, size_t voice_count, const char* out);

#ifdef __cplusplus
}
#endif

#endif //__VOICE_RECORD_H__