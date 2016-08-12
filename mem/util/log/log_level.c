/******************************************************************
** 文件名:      log_level.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 描 述:       日志模式定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_level.c,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */
#include	<strings.h>
#include    "log_level.h"
#include    "../symbol.h"
#include    "../lib/common_lib.h"
#include    "log_file.h"

/*
 * 日志级别定义表
 */
LOG_LEVEL   _logLevels[] = {
    {0,      "TRACE"},                                    /* 跟踪信息       */
    {1,      "DEBUG"},                                    /* 调试信息       */
    {2,      "INFO"},                                     /* 应用信息       */
    {3,      "WARN"},                                     /* 警告信息       */
    {4,      "ERROR"},                                    /* 错误信息       */
    {5,      "FATAL"},                                    /* 致命错误信息   */
    {6,      "NONE"},                                     /* 屏蔽所有日志   */
};
/* -------------------------           */



/*****************************************************************
** 函数名:      GetLogLevel
** 输 入:
** 输 出:       LOG_LEVEL*
** 功能描述:    返回名称对应的日志级别
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_LEVEL*
GetLogLevel(const char *levelName) {
	int i = 0;
	
    for (i = 0; i < Dim(_logLevels); i++) {
        if (! strncasecmp(_logLevels[i].name, levelName, sizeof(_logLevels[i].name))) {
            return &_logLevels[i];
        }
    }
    return NULL;
}
