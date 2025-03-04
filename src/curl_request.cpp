#include <curl/curl.h>
#include <iostream>
#include <string>


/**
 * CURL 数据写入回调函数。
 *
 * @param ptr   指向接收数据的缓冲区。
 * @param size  每个数据单元的大小（通常为 1）。
 * @param nmemb 数据单元的数量。
 * @param data  指向存储接收数据的字符串对象。
 * @return 实际写入的数据大小（size * nmemb）。
 */
size_t write_callback(void* ptr, size_t size, size_t nmemb, std::string* data)
{
    size_t totalSize = size * nmemb;
    data->append(static_cast<char*>(ptr), totalSize); // 将接收到的数据追加到字符串
    return totalSize;
}

/**
 * 发送 HTTP 请求并下载文件或获取响应数据。
 *
 * @param url             请求的 URL 地址。
 * @param filePath        要保存下载内容的文件路径（为空字符串时不保存）。
 * @param responseFormat  响应格式（默认为 "json"）。
 * @return 返回服务器的响应数据（字符串格式），请求失败时返回空字符串。
 */
std::string curl_request(const std::string& url, const std::string& filePath, const std::string& responseFormat)
{
    CURL* curl;
    CURLcode res;
    std::string response; // 用于存储服务器的响应

    // 初始化 curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // 设置 URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // 设置 POST 请求的 HTTP header
        struct curl_httppost* form = NULL;
        struct curl_httppost* lastptr = NULL;

        // 添加文件字段
        curl_formadd(&form, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, filePath.c_str(), CURLFORM_END);

        // 添加 response_format 字段
        curl_formadd(&form, &lastptr, CURLFORM_COPYNAME, "response_format", CURLFORM_COPYCONTENTS, responseFormat.c_str(), CURLFORM_END);

        // 设置 CURLOPT_HTTPPOST 以启用 multipart/form-data
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);

        // 设置回调函数来处理服务器响应
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 执行请求
        res = curl_easy_perform(curl);

        // 检查请求是否成功
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_formfree(form);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return ""; // 返回空字符串表示失败
        }

        // 清理表单
        curl_formfree(form);

        // 清理 curl
        curl_easy_cleanup(curl);
    }

    // 释放全局 curl 环境
    curl_global_cleanup();

    return response; // 返回服务器响应的字符串
}

#ifdef _XTEST

#include "json.hpp"

using json = nlohmann::json;

int main()
{
    // 调用函数并传递参数
    std::string url = "http://127.0.0.1:8090/inference";
    std::string filePath = "./whisper.cpp-1.7.4/samples/jfk.wav";
    std::string responseFormat = "json";

    // 调用 sendInferenceRequest 函数并获取响应
    std::string response = curl_request(url, filePath, responseFormat);

    // 打印服务器响应
    if (!response.empty()) {
        std::cout << "Server response: " << std::endl;
        std::cout << response << std::endl;
        // 解析 JSON 字符串
        try {
            // 使用 nlohmann::json 解析 JSON 字符串
            json parsed_json = json::parse(response);

            // 提取 text 字段
            std::string text = parsed_json["text"];

            // 打印 text 字段
            std::cout << "Text: " << text << std::endl;
        } catch (const json::parse_error& e) {
            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Request failed." << std::endl;
    }

    return 0;
}

#endif //_XTEST