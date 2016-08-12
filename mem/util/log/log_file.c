/******************************************************************
** �ļ���:      log_file.c
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2004.4.29
** �� ��:       �ļ���־�������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
** ������:      TestFileLogConfig
** �� ��:       ��
** �� ��:       BOOL
** ��������:    �����ļ���־ϵͳ�������Ƿ���Ч
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2004.4.27
** �� ��:
** �� ��:
** �汾:        0.1
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
     * ������־�ļ�
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
** ������:      FileLogDate
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              msg:      ��־����
** �� ��:       ��
** ��������:    ��־ģʽΪ DATE ���ļ���־�ǼǺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.4.26
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      FileLogAlternate
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              msg:      ��־����
** �� ��:       ��
** ��������:    ��־ģʽΪ ALTERNATE ���ļ���־�ǼǺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.4.26
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      FileLogDateSingle
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              msg:      ��־����
** �� ��:       ��
** ��������:    ��־ģʽΪ DATE_SINGLE ���ļ���־�ǼǺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.4.26
** �� ��:
** �� ��:
** �汾:        0.1
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

