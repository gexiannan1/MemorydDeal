/******************************************************************
** 文件名:      log_mode.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.12.23
** 描 述:       日志模式定义头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_mode.h,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#ifndef _LOG_MODE_H
#define _LOG_MODE_H


#include    "../symbol.h"
#include   "log_level.h"
#define     APPENDER_TYPE_FILE          0               /* 日志记录器类型 - 文件 */
#define     APPENDER_TYPE_CONSOLE       1               /* 日志记录器类型 - 控制台 */

typedef struct log_appender LOG_APPENDER;
/*
 * 日志模式结构体定义
 */
typedef struct {
    int     mode;                                       /* 日志模式代码 */
    char    name[32];                                   /* 日志模式名称 */
    int     appenderType;                               /* 日志记录器类型 */
 //   void    (*fileLog  )();
   void    (*fileLog)(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
     LOG_LEVEL *level, const char *module, const char *msg);                               /* 日志实现函数 */
} LOG_MODE;


/*
 * 日志级别定义表
 */
extern      LOG_MODE       _logModes[];


/*
 * 日志级别常量定义
 */
#define     LOG_MODE_FILE_DATE          (&_logModes[0]) /* 日志模式 - 文件日志 - 轮换, 不区分具体日期 */
#define     LOG_MODE_FILE_ALTERNATE     (&_logModes[1]) /* 日志模式 - 文件日志 - 每天N个日志文件(N >= 1) */
#define     LOG_MODE_FILE_DATE_SINGLE   (&_logModes[2]) /* 日志模式 - 文件日志 - 每天一个日志文件 */
#define     LOG_MODE_CONSOLE            (&_logModes[3]) /* 日志模式 - 控制台日志 */



/*
 * 函数定义
 */
LOG_MODE*   GetLogMode(const char *modeName);           /* 返回名称对应的日志模式 */
BOOL        IsFileLogAppender(LOG_MODE*);               /* 返回日志模式对应的日志记录器类型是否是文件记录器 */


#endif  /* _LOG_MODE_H */
