/******************************************************************
** 文件名:      log_mode.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 描 述:       日志模式定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_mode.c,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */
#include	<strings.h>
#include    "log_mode.h"
#include    "log_file.h"
#include    "log_impl_console.h"
#include    "../symbol.h"
#include    "../lib/common_lib.h"

/*
 * 日志模式定义表
 */
LOG_MODE    _logModes[] = {
 {0,  "FILE_ALTERNATE",   0,  FileLogAlternate},       /* 日志模式 - 文件日志 - 轮换, 不区分具体日期 */
{1,  "FILE_DATE",        0,  FileLogDate},            /* 日志模式 - 文件日志 - 每天N个日志文件(N >= 1) */
  {2,  "FILE_DATE_SINGLE", 0,  FileLogDateSingle},      /* 日志模式 - 文件日志 - 每天一个日志文件 */
   {3,  "CONSOLE",          1,  ConsoleLog },             /* 日志模式 - 控制台日志 */
};
/* -------------------------           */



/*****************************************************************
** 函数名:      GetLogMode
** 输 入:
** 输 出:       LOG_MODE*
** 功能描述:    返回名称对应的日志模式
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_MODE*
GetLogMode(const char *modeName) {
    int i = 0;

    for (i = 0; i < Dim(_logModes); i++) {
        if (! strncasecmp(_logModes[i].name, modeName, sizeof(_logModes[i].name))) {
            return &_logModes[i];
        }
    }
    return NULL;
}


/*****************************************************************
** 函数名:      IsFileLogAppender
** 输 入:
** 输 出:       BOOL
** 功能描述:    返回日志模式对应的日志记录器类型是否是文件记录器
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2006.3.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsFileLogAppender(LOG_MODE *logMode) {
    return logMode->appenderType == APPENDER_TYPE_FILE;
}
