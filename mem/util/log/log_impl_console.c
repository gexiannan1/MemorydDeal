/******************************************************************
** 文件名:      log_impl_console.c
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2006.3.1
** 描 述:       控制台日志处理程序
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_impl_console.c,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <fcntl.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    "log.h"
#include    "log_level.h"
#include    "log_mode.h"
#include    "log_impl_console.h"
#include    "../symbol.h"
#include    "../lib/common_lib.h"
#include    "../time/common_time.h"


/*****************************************************************
** 函数名:      ConsoleLog
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              msg:      日志内容
** 输 出:       无
** 功能描述:    日志模式为 CONSOLE 的控制台日志登记函数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2006.3.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
ConsoleLog(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg) {
    fprintf(stdout, "%s %-5s [%5d] (%s#%d) <%s> - %s\n",
            GetFormattedTimestamp(), level->name, getpid(), srcFile, srcLine,
            module, msg);
}
