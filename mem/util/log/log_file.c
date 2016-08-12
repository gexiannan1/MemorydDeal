/******************************************************************
** 文件名:      log_file.c
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2004.4.29
** 描 述:       文件日志处理程序
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_file.c,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <fcntl.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    "log.h"
#include    "log_file.h"
#include    "log_level.h"
#include    "log_mode.h"
#include    "../symbol.h"
#include    "../lib/common_lib.h"
#include    "../time/common_time.h"
#include    "../file/common_file.h"
#include    "../string/common_str.h"


/*****************************************************************
** 函数名:      TestFileLogConfig
** 输 入:       无
** 输 出:       BOOL
** 功能描述:    测试文件日志系统的配置是否有效
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2004.4.27
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
TestFileLogConfig(LOG_APPENDER *logConfig) {
    int     	fp  = -1;
    char		testLogFile[256];

	if (IsEmptyString(logConfig->logFile)) {
        return FALSE;
    }
    
	sprintf(testLogFile, "%s.tmp", logConfig->logFile);

    /*
     * 测试日志文件
     */
    fp = open(testLogFile, O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fp > 0) {
        close(fp);
        if (!remove(testLogFile)) {
        	return TRUE;
        }
    }
    return FALSE;
}


/*****************************************************************
** 函数名:      FileLogDate
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              msg:      日志内容
** 输 出:       无
** 功能描述:    日志模式为 DATE 的文件日志登记函数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
FileLogDate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg) {
    int         fp                  = 0;
    long        length              = 0;
    char        buf[MAX_LOG_INFO_SIZE]   = "";
    char        logFile[128]        = "";
    char        bakLogFile[128]     = "";

    sprintf(buf, "%s %-5s [%5d] (%s#%d) <%s> - %s\n",
            GetFormattedTimestamp(), level->name, getpid(), srcFile, srcLine,
            module, msg);

    sprintf(logFile, "%s.%s", logConfig->logFile, GetDate());

    if ((fp = open(logFile, O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        return;
    }

    write(fp, buf, strlen(buf));
    length = lseek(fp, 0, SEEK_END);
    close(fp);

    if (logConfig->maxFileLength > 0 && length > logConfig->maxFileLength * 1048576) {
        sprintf(bakLogFile, "%s.%s.%s", logConfig->logFile, GetDate(), GetTime());
        rename(logFile, bakLogFile);
    }
}


/*****************************************************************
** 函数名:      FileLogAlternate
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              msg:      日志内容
** 输 出:       无
** 功能描述:    日志模式为 ALTERNATE 的文件日志登记函数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
FileLogAlternate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg) {
    int         fp                  = 0;
    long        length              = 0;
    char        buf[MAX_LOG_INFO_SIZE]   = "";
    char        logFile[128]        = "";
    char        bakLogFile[128]     = "";
    int         i                   = 0;

    sprintf(buf, "%s %-5s [%5d] (%s#%d) <%s> - %s\n",
            GetFormattedTimestamp(), level->name, getpid(), srcFile, srcLine,
            module, msg);

    if ((fp = open(logConfig->logFile, O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        return;
    }

    write(fp, buf, strlen(buf));
    length = lseek(fp, 0, SEEK_END);
    close(fp);

    if (logConfig->maxFileLength > 0 && length > logConfig->maxFileLength * 1048576) {
        if (logConfig->maxBackupIndex > 0) {
            sprintf(logFile, "%s.%d", logConfig->logFile, logConfig->maxBackupIndex);
            if (IsFileExist(logFile)) {
                remove(logFile);
            }

            for (i = logConfig->maxBackupIndex - 1; i > 0; i--) {
                sprintf(logFile, "%s.%d", logConfig->logFile, i);
                sprintf(bakLogFile, "%s.%d", logConfig->logFile, i + 1);

                if (IsFileExist(logFile)) {
                    rename(logFile, bakLogFile);
                }
            }
        }

        sprintf(logFile, "%s.%d", logConfig->logFile, 1);
        if (IsFileExist(logFile)) {
            remove(logFile);
        }

        rename(logConfig->logFile, logFile);
    }
}


/*****************************************************************
** 函数名:      FileLogDateSingle
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              msg:      日志内容
** 输 出:       无
** 功能描述:    日志模式为 DATE_SINGLE 的文件日志登记函数
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
FileLogDateSingle(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg) {
    int         fp                  = 0;
    char        buf[MAX_LOG_INFO_SIZE]   = "";
    char        logFile[128]        = "";

    sprintf(buf, "%s %-5s [%5d] (%s#%d) <%s> - %s\n",
            GetFormattedTimestamp(), level->name, getpid(), srcFile, srcLine,
            module, msg);

    sprintf(logFile, "%s.%s", logConfig->logFile, GetDate());

    if ((fp = open(logFile, O_WRONLY | O_APPEND | O_CREAT,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
        return;
    }

    write(fp, buf, strlen(buf));
    close(fp);
}

