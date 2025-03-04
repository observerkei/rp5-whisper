#include "debug.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define LINE_LEN (1024)


/**
 * 执行系统命令并获取输出结果。
 *
 * @param command 要执行的系统命令字符串。
 * @param output  存储命令执行后的输出结果。
 * @return 成功返回 0，失败返回非 0 值。
 */
int execute_command(const char* command, std::string& output)
{
    FILE* pipe = popen(command, "r");
    if (!pipe) {
        LOG_ERR("Failed to execute command: %s", command);
        return -1;
    }

    char buffer[LINE_LEN];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        LOG_DBG("%s", buffer); // 输出命令执行过程中的信息
        output += buffer;
        output += "\n";
    }

    int status = pclose(pipe);
    if (status == -1) {
        LOG_ERR("Error closing pipe.");
        return -1;
    }

    int code = WEXITSTATUS(status); // 获取命令返回值
    if (code != 0) {
        LOG_ERR("run cmd: %s, code: %d", command, code);
        return -1;
    }
    return 0;
}