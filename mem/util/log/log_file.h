/******************************************************************
** 文件名:      log_file.h
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2004.4.29
** 描 述:       文件日志处理程序的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_file.h,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#ifndef _LOG_FILE_H
#define _LOG_FILE_H


#include    "log.h"


/*
 * 函数声明
 */

/* 测试文件日志系统的配置是否有效 */
BOOL    TestFileLogConfig(LOG_APPENDER *logConfig);

/* 日志模式为 DATE 的文件日志登记函数 */
void    FileLogDate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);

/* 日志模式为 ALTERNATE 的文件日志登记函数 */
void    FileLogAlternate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);

/* 日志模式为 DATE_SINGLE 的文件日志登记函数 */
void    FileLogDateSingle(LOG_APPENDER *logconfig, const char *srcfile, int srcline,
        LOG_LEVEL *level, const char *module, const char *msg);


#endif  /* _LOG_FILE_H */
