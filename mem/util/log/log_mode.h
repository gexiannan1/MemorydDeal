/******************************************************************
** �ļ���:      log_mode.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.12.23
** �� ��:       ��־ģʽ����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_mode.h,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#ifndef _LOG_MODE_H
#define _LOG_MODE_H


#include    "../symbol.h"
#include   "log_level.h"
#define     APPENDER_TYPE_FILE          0               /* ��־��¼������ - �ļ� */
#define     APPENDER_TYPE_CONSOLE       1               /* ��־��¼������ - ����̨ */

typedef struct log_appender LOG_APPENDER;
/*
 * ��־ģʽ�ṹ�嶨��
 */
typedef struct {
    int     mode;                                       /* ��־ģʽ���� */
    char    name[32];                                   /* ��־ģʽ���� */
    int     appenderType;                               /* ��־��¼������ */
 //   void    (*fileLog  )();
   void    (*fileLog)(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
     LOG_LEVEL *level, const char *module, const char *msg);                               /* ��־ʵ�ֺ��� */
} LOG_MODE;


/*
 * ��־�������
 */
extern      LOG_MODE       _logModes[];


/*
 * ��־����������
 */
#define     LOG_MODE_FILE_DATE          (&_logModes[0]) /* ��־ģʽ - �ļ���־ - �ֻ�, �����־������� */
#define     LOG_MODE_FILE_ALTERNATE     (&_logModes[1]) /* ��־ģʽ - �ļ���־ - ÿ��N����־�ļ�(N >= 1) */
#define     LOG_MODE_FILE_DATE_SINGLE   (&_logModes[2]) /* ��־ģʽ - �ļ���־ - ÿ��һ����־�ļ� */
#define     LOG_MODE_CONSOLE            (&_logModes[3]) /* ��־ģʽ - ����̨��־ */



/*
 * ��������
 */
LOG_MODE*   GetLogMode(const char *modeName);           /* �������ƶ�Ӧ����־ģʽ */
BOOL        IsFileLogAppender(LOG_MODE*);               /* ������־ģʽ��Ӧ����־��¼�������Ƿ����ļ���¼�� */


#endif  /* _LOG_MODE_H */
