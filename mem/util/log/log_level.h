/******************************************************************
** 文件名:      log_level.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 描 述:       日志级别定义头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_level.h,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#ifndef _LOG_LEVEL_H
#define _LOG_LEVEL_H


/*
 * 日志级别结构体定义
 */
typedef struct {
    int     level;			/* 日志级别代码 */
    char    name[32];		/* 日志级别名称 */
} LOG_LEVEL;


/*
 * 日志级别定义表
 */
extern      LOG_LEVEL       _logLevels[];


/*
 * 日志级别常量定义
 */
#define     LOG_TRACE       (&_logLevels[0])        /* 日志级别 TRACE */
#define     LOG_DEBUG       (&_logLevels[1])        /* 日志级别 DEBUG */
#define     LOG_INFO        (&_logLevels[2])        /* 日志级别 INFO  */
#define     LOG_WARN        (&_logLevels[3])        /* 日志级别 WARN */
#define     LOG_ERROR       (&_logLevels[4])        /* 日志级别 ERROR  */
#define     LOG_FATAL       (&_logLevels[5])        /* 日志级别 FATAL */
#define     LOG_NONE        (&_logLevels[6])        /* 日志级别 NONE  */



/*
 * 函数定义
 */
LOG_LEVEL*	GetLogLevel(const char *levelName);		/* 返回名称对应的日志级别 */



#endif  /* _LOG_LEVEL_H */
