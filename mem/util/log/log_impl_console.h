/******************************************************************
** �ļ���:      log_impl_console.h
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2006.3.1
** �� ��:       ����̨��־�������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_impl_console.h,v 1.1 2012/03/29 01:20:14 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:14 $
 */

#ifndef _LOG_IMPL_CONSOLE_H
#define _LOG_IMPL_CONSOLE_H


#include    "log.h"


/*
 * ��������
 */

/* ��־ģʽΪ CONSOLE �Ŀ���̨��־�ǼǺ��� */
void    ConsoleLog(LOG_APPENDER *logConfig, const char *srcFile, int srcLine,
        LOG_LEVEL *level, const char *module, const char *msg);


#endif  /* _LOG_IMPL_CONSOLE_H */
