/******************************************************************
** �ļ���:      log_level.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.4.26
** �� ��:       ��־������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��־����ṹ�嶨��
 */
typedef struct {
    int     level;			/* ��־������� */
    char    name[32];		/* ��־�������� */
} LOG_LEVEL;


/*
 * ��־�������
 */
extern      LOG_LEVEL       _logLevels[];


/*
 * ��־����������
 */
#define     LOG_TRACE       (&_logLevels[0])        /* ��־���� TRACE */
#define     LOG_DEBUG       (&_logLevels[1])        /* ��־���� DEBUG */
#define     LOG_INFO        (&_logLevels[2])        /* ��־���� INFO  */
#define     LOG_WARN        (&_logLevels[3])        /* ��־���� WARN */
#define     LOG_ERROR       (&_logLevels[4])        /* ��־���� ERROR  */
#define     LOG_FATAL       (&_logLevels[5])        /* ��־���� FATAL */
#define     LOG_NONE        (&_logLevels[6])        /* ��־���� NONE  */



/*
 * ��������
 */
LOG_LEVEL*	GetLogLevel(const char *levelName);		/* �������ƶ�Ӧ����־���� */



#endif  /* _LOG_LEVEL_H */
