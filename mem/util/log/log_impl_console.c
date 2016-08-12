/******************************************************************
** �ļ���:      log_impl_console.c
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2006.3.1
** �� ��:       ����̨��־�������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
** ������:      ConsoleLog
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              msg:      ��־����
** �� ��:       ��
** ��������:    ��־ģʽΪ CONSOLE �Ŀ���̨��־�ǼǺ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2006.3.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
ConsoleLog(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg) {
    fprintf(stdout, "%s %-5s [%5d] (%s#%d) <%s> - %s\n",
            GetFormattedTimestamp(), level->name, getpid(), srcFile, srcLine,
            module, msg);
}
