#include "debug.h"

log_dbg_flag_t g_dbg_enable = log_dbg_flag_t(LOG_ERR_FLAG | LOG_INFO_FLAG | LOG_DBG_FLAG);

/**
 * 设置调试日志的启用标志。
 *
 * @param flag 指定的调试日志标志，决定是否启用调试模式。
 */
void set_dbg_enable(log_dbg_flag_t flag)
{
    g_dbg_enable = flag;
}

/**
 * 获取当前的调试日志启用标志。
 *
 * @return 当前的调试日志标志。
 */
log_dbg_flag_t get_dbg_enable()
{
    return g_dbg_enable;
}
