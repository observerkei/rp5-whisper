#include "whisper.h"
#include "curl_request.h"
#include "debug.h"
#include "json.hpp"
#include "voice_max_loudness.h"
#include "voice_record.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using json = nlohmann::json;


/**
 * Whisper 结构体，包含语音计数和映射数据。
 */
typedef struct whisper_t {
    size_t voice_count;                                 ///< 语音数量计数。
    std::unordered_map<std::string, std::string>* map;  ///< 指向存储文本转换映射的哈希表指针。
} whisper_t;


/**
 * 将字符串转换为小写。
 *
 * @param str 需要转换的小写字符串，转换结果直接作用于输入参数。
 */
void to_lower(std::string& str)
{
    for (char& c : str) {
        c = std::tolower(c);
    }
}

/**
 * 读取配置文件并存储到映射表中。
 *
 * @param filename 配置文件的路径。
 * @param map 存储键值对的哈希表。
 * @return 成功返回 0，失败返回 -1。
 */
int read_config(const std::string& filename, std::unordered_map<std::string, std::string>& map)
{
    std::ifstream file(filename);
    if (!file) {
        LOG_ERR("fail to open %s", filename.c_str());
        return -1;
    }

    json config;
    file >> config;

    for (const auto& item : config) {
        for (const auto& text : item["text"]) {
            std::string text_lower = text.get<std::string>();
            to_lower(text_lower);

            std::string code = item["code"].get<std::string>(); 

            map[text_lower] = code;
            LOG_DBG("read %s -> %s", text_lower.c_str(), code.c_str());
        }
    }
    return 0;
}

/**
 * 去除字符串首尾空白字符。
 *
 * @param str 需要去除空白字符的字符串。
 * @return 返回去除空白后的字符串副本。
 */
std::string trim(const std::string& str)
{
    // 找到第一个非空格字符
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    // 找到最后一个非空格字符
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    // 如果字符串全为空格
    if (start == std::string::npos || end == std::string::npos) {
        return ""; // 返回空字符串
    }

    // 截取去掉首尾空格的部分
    return str.substr(start, end - start + 1);
}

/**
 * 将语音文件转换为文本。
 *
 * @param voice_file 语音文件的路径。
 * @param text 输出转换后的文本。
 * @return 成功返回 0，失败返回 -1。
 */
int voice_to_text(const char* voice_file, std::string& text)
{
    std::string url = "http://127.0.0.1:";
    url += std::to_string(WHISPER_SERVER_PORT);
    url += "/inference";

    std::string response = curl_request(url, voice_file);
    // 打印服务器响应
    if (response.empty()) {
        LOG_ERR("server no response");
        return -1;
    }
    // 解析 JSON 字符串
    try {
        // 使用 nlohmann::json 解析 JSON 字符串
        json parsed_json = json::parse(response);

        // 检查 "text" 字段是否存在且不为 null
        if (parsed_json.contains("text") && !parsed_json["text"].is_null()) {
            text = parsed_json["text"];
            text = trim(text);  // 调用 trim 函数去除首尾空格
            // 打印 text 字段
            LOG_DBG("Text: %s", text.c_str());
        } else {
            std::cout << "text 字段为空或不存在, response: " << response << std::endl;
        }
    } catch (const json::parse_error& e) {
        std::string err = e.what();
        LOG_ERR("JSON Parse Error: %s", err.c_str());
    }

    return 0;
}

/**
 * 将文本转换为代码。
 *
 * @param map 存储文本与代码映射的哈希表。
 * @param text 需要转换的文本。
 * @return 返回转换后的代码字符串指针，失败返回 NULL。
 */
const char* text_to_code(std::unordered_map<std::string, std::string>& map, const char* text)
{
    std::string text_lower = text;
    to_lower(text_lower);

    if (map.find(text_lower) == map.end()) {
        LOG_ERR("unknow %s ", text_lower.c_str());
        return "0x00";
    }
    return map[text_lower].c_str();
}

/**
 * 初始化 Whisper 组件。
 *
 * @param config 配置文件路径或配置字符串。
 * @return 成功返回指向 whisper_t 结构的指针，失败返回 NULL。
 */
whisper_t* whisper_init(const char* config)
{
    whisper_t* w = (whisper_t*)malloc(sizeof(whisper_t));
    if (!w) {
        LOG_ERR("fail to malloc whisper");
        return nullptr;
    }
    memset(w, 0, sizeof(whisper_t));

    w->map = new std::unordered_map<std::string, std::string>;
    if (!w->map) {
        free(w);
        return nullptr;
    }

    int ret = read_config(config, *w->map);
    if (ret < 0) {
        LOG_DBG("fail to read_config");
        whisper_exit(w);
        return nullptr;
    }
    return w;
}

/**
 * 释放 Whisper 组件并清理资源。
 *
 * @param w 指向要释放的 whisper_t 结构体指针。
 */
void whisper_exit(whisper_t* w)
{
    if (!w)
        return;

    if (w->map) {
        delete w->map;
        w->map = nullptr;
    }
    free(w);
}

/**
 * 处理 Whisper 任务。
 *
 * @param w 指向 whisper_t 结构体的指针。
 * @param callback 处理完成后的回调函数。
 * @param userdata 传递给回调函数的用户数据。
 * @return 成功返回 0，失败返回 -1。
 */
int whisper(whisper_t* w, whisper_callback_t callback, void* userdata)
{
    int ret = 0;
    const char* record_prefix = "/tmp/record_";

    std::string save_file = record_prefix;
    save_file += std::to_string(w->voice_count) + ".wav";

    ret = voice_record(save_file.c_str(), 1);
    if (ret < 0) {
        LOG_ERR("fail to voice_record");
        w->voice_count = 0;
        return -1;
    }

    double max_loudness = get_max_loudness(save_file.c_str());
    if (max_loudness <= SILENCE_THRESHOLD && w->voice_count == 0) {
        LOG_DBG("max_loudness(%f) <= SILENCE_THRESHOLD(%d), no voice",
            max_loudness, SILENCE_THRESHOLD);
        return -2;
    }

    w->voice_count += 1;
    if (max_loudness > SILENCE_THRESHOLD
        && w->voice_count <= RECODE_TIMEOUT) {
        LOG_DBG("max_loudness(%f) > SILENCE_THRESHOLD(%d), continue record",
            max_loudness, SILENCE_THRESHOLD);
        return 1;
    }


    std::string save_record = record_prefix;
    save_record += "all.wav";

    ret = splicing_audio(record_prefix, w->voice_count, save_record.c_str());
    if (ret < 0) {
        LOG_ERR("fail to splicing_audio");
        w->voice_count = 0;
        return -1;
    }

    std::string text = "";
    ret = voice_to_text(save_record.c_str(), text);
    if (ret < 0) {
        LOG_ERR("fail to voice_to_text");
        w->voice_count = 0;
        return -1;
    }
    const char *code = text_to_code(*w->map, text.c_str());

    callback(text.c_str(), code, userdata);
    w->voice_count = 0;

    return 0;
}
