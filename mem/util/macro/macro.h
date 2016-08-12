/******************************************************************
** �ļ���:      macro.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2006.3.2
** �� ��:       Ӧ�������ļ������������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/macro/macro.h,v 1.1 2012/03/29 01:20:15 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:15 $
 */

#ifndef _COMMON_MACRO_H
#define _COMMON_MACRO_H


/*
 * ����BOOL����
 */
#define     MACRO_APP_HOME         	"${APP_HOME}"			/* ���ò����꣬Ӧ����Ŀ¼ */
#define     MACRO_SYSTEM_NAME  		"${SYSTEM_NAME}"		/* ���ò����꣬ϵͳ���� */


/*
 * �ⲿ��������
 */
extern char*	GetHome();									/* ����Ӧ����Ŀ¼·�� */


/*
 * ��������
 */
char*			ReplaceMacroAppHome(const char*);			/* �滻${APP_HOME}�� */
char*			ReplaceMacroSystemName(const char *target, 
				const char *value);							/* �滻${SYSTEM_NAME}�� */


#endif  /* _COMMON_MACRO_H */
