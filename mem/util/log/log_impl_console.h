/******************************************************************
** 文件名:      log_impl_console.h
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2006.3.1
** 描 述:       控制台日志处理程序
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_impl_console.h,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#ifndef _LOG_IMPL_CONSOLE_H
#define _LOG_IMPL_CONSOLE_H


#include    "log.h"


/*
 * 函数声明
 */

/* 日志模式为 CONSOLE 的控制台日志登记函数 */
void    ConsoleLog(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);


#endif  /* _LOG_IMPL_CONSOLE_H */
