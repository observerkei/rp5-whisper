#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_DBG_FLAG  (1 << 2)
#define LOG_INFO_FLAG (1 << 1)
#define LOG_ERR_FLAG  (1 << 0)

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/')) 

#define LOG_DBG(fmt, ...)  if (g_dbg_enable & LOG_DBG_FLAG)  fprintf(stdout, "[%s:%s:%d] " fmt "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) if (g_dbg_enable & LOG_INFO_FLAG) fprintf(stdout, "[%s:%s:%d] " fmt "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)  if (g_dbg_enable & LOG_ERR_FLAG)  fprintf(stderr, "[%s:%s:%d] " fmt "\n", __FILENAME__, __func__, __LINE__, ##__VA_ARGS__)


extern char g_dbg_enable;

#ifdef __cplusplus
}
#endif

#endif//__DEBUG_H__