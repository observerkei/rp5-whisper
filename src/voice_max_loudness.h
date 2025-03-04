#ifndef __VOICE_MAX_LOUDNESS_H__
#define __VOICE_MAX_LOUDNESS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 计算 WAV 文件的最大响度（峰值响度）。
 *
 * @param filename 要分析的 WAV 文件路径。
 * @return 返回最大响度值（单位：dB），例如 -6.21521 dB。发生错误时返回 size_t(-1) 
 */
double get_max_loudness(const char* filename);

#ifdef __cplusplus
}
#endif

#endif //__VOICE_MAX_LOUDNESS_H__