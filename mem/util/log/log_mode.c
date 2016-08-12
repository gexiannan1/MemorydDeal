/******************************************************************
** �ļ���:      log_mode.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:       ��־ģʽ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��־ģʽ�����
 */
LOG_MODE    _logModes[] = {
 {0,  "FILE_ALTERNATE",   0,  FileLogAlternate},       /* ��־ģʽ - �ļ���־ - �ֻ�, �����־������� */
{1,  "FILE_DATE",        0,  FileLogDate},            /* ��־ģʽ - �ļ���־ - ÿ��N����־�ļ�(N >= 1) */
  {2,  "FILE_DATE_SINGLE", 0,  FileLogDateSingle},      /* ��־ģʽ - �ļ���־ - ÿ��һ����־�ļ� */
   {3,  "CONSOLE",          1,  ConsoleLog },             /* ��־ģʽ - ����̨��־ */
};
/* -------------------------           */



/*****************************************************************
** ������:      GetLogMode
** �� ��:
** �� ��:       LOG_MODE*
** ��������:    �������ƶ�Ӧ����־ģʽ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsFileLogAppender
** �� ��:
** �� ��:       BOOL
** ��������:    ������־ģʽ��Ӧ����־��¼�������Ƿ����ļ���¼��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2006.3.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsFileLogAppender(LOG_MODE *logMode) {
    return logMode->appenderType == APPENDER_TYPE_FILE;
}
