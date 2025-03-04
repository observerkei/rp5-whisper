#ifndef __CURL_REQUEST_H__
#define __CURL_REQUEST_H__

#include <string>

/**
 * 发送 HTTP 请求并下载文件或获取响应数据。
 *
 * @param url             请求的 URL 地址。
 * @param filePath        要保存下载内容的文件路径（为空字符串时不保存）。
 * @param responseFormat  响应格式（默认为 "json"）。
 * @return 返回服务器的响应数据（字符串格式），请求失败时返回空字符串。
 */
std::string curl_request(const std::string& url, const std::string& filePath, const std::string& responseFormat = "json");

#endif//__CURL_REQUEST_H__