#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <string>
#include "voice_max_loudness.h"
#include "debug.h"

/**
 * WAV 文件头结构体，存储 WAV 文件的基本信息。
 */
struct WAVHeader {
    char      riff[4];         ///< 标识文件类型，必须为 "RIFF"。
    uint32_t  chunkSize;       ///< 文件总大小（不包括 "RIFF" 和自身）。
    char      wave[4];         ///< 文件格式，必须为 "WAVE"。
    char      fmt[4];          ///< 子块标识，必须为 "fmt "。
    uint32_t  subchunk1Size;   ///< 格式子块大小（PCM 为 16）。
    uint16_t  audioFormat;     ///< 音频格式（1 = PCM，其他值表示压缩格式）。
    uint16_t  numChannels;     ///< 通道数（1 = 单声道，2 = 立体声）。
    uint32_t  sampleRate;      ///< 采样率（如 44100Hz, 48000Hz）。
    uint32_t  byteRate;        ///< 每秒字节数（sampleRate * numChannels * bitsPerSample / 8）。
    uint16_t  blockAlign;      ///< 每个采样帧的字节数（numChannels * bitsPerSample / 8）。
    uint16_t  bitsPerSample;   ///< 每个样本的位数（如 16 位、24 位）。
    char      data[4];         ///< 数据块标识，必须为 "data"。
    uint32_t  subchunk2Size;   ///< 数据块大小（音频数据的字节数）。
};

/**
 * 计算 WAV 文件的最大响度（峰值响度）。
 *
 * @param filename 要分析的 WAV 文件路径。
 * @return 返回最大响度值（单位：dB），例如 -6.21521 dB。发生错误时返回 size_t(-1)。
 */
double get_max_loudness(const char * filename) 
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        LOG_ERR("Failed to open WAV file.");
        return size_t(-1);
    }

    // 读取 WAV 文件头部
    WAVHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.audioFormat != 1 || header.bitsPerSample != 16) {
        LOG_ERR("Only 16-bit PCM WAV files are supported.");
        return size_t(-1);
    }

    // 计算数据区的样本数量
    size_t numSamples = header.subchunk2Size / sizeof(int16_t);

    // 分配内存来存储样本
    int16_t* samples = new int16_t[numSamples];

    // 读取样本数据
    file.read(reinterpret_cast<char*>(samples), header.subchunk2Size);
    file.close();

    // 找到最大的样本值（即峰值）
    int16_t maxSample = 0;
    for (size_t i = 0; i < numSamples; ++i) {
        if (std::abs(samples[i]) > std::abs(maxSample)) {
            maxSample = samples[i];
        }
    }

    // 计算最大响度的比例值（可以转化为 dB）
    double peakLoudness = maxSample / 32768.0 * 100.0;  // 最大幅度转换为百分比

    // 可选：输出最大响度的 dB 值
    double peakLoudnessDB = 20 * std::log10(std::abs(maxSample) / 32768.0);
    LOG_DBG("Maximum peak loudness (dB): %f dB", peakLoudnessDB);

    delete[] samples;
    
    return peakLoudnessDB;
}

#ifdef _XTEST

int main() {
    // 调用函数计算 WAV 文件的最大响度
    std::string filePath = "example.wav";
    double maxLoudness = get_max_loudness(filePath.c_str());

    if (maxLoudness != -1) {
        std::cout << "Maximum peak loudness (percentage): " << maxLoudness << "%" << std::endl;
    }

    return 0;
}
#endif//_XTEST