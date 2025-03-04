#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

/**
 * 执行系统命令并获取输出结果。
 *
 * @param command 要执行的系统命令字符串。
 * @param output  存储命令执行后的输出结果。
 * @return 成功返回 0，失败返回非 0 值。
 */
int execute_command(const char* command, std::string& output);

#endif//__COMMAND_H__