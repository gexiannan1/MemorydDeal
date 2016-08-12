/******************************************************************
** �ļ���:      log_file.h
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2004.4.29
** �� ��:       �ļ���־��������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��������
 */

/* �����ļ���־ϵͳ�������Ƿ���Ч */
BOOL    TestFileLogConfig(LOG_APPENDER *logConfig);

/* ��־ģʽΪ DATE ���ļ���־�ǼǺ��� */
void    FileLogDate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);

/* ��־ģʽΪ ALTERNATE ���ļ���־�ǼǺ��� */
void    FileLogAlternate(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);

/* ��־ģʽΪ DATE_SINGLE ���ļ���־�ǼǺ��� */
void    FileLogDateSingle(LOG_APPENDER *logconfig, const char *srcfile, int srcline,
        LOG_LEVEL *level, const char *module, const char *msg);


#endif  /* _LOG_FILE_H */
