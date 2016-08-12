/******************************************************************
** �ļ���:      env_base.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.12
** �� ��:       ȫ�ֻ���������������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/env_base/env_base.h,v 1.1 2012/03/29 01:20:10 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:10 $
 */

#ifndef _ENV_BASE_H
#define _ENV_BASE_H


/*
 * ��������
 */
int     InitEnv(const char*, const char *);         /* ��ʼ��ȫ�ֻ��������������Ļ��� */
int     ResetEnv(const char*, const char *);     	/* ����ȫ�ֻ������� */

void    SetRootEnvStorage(const char*);             /* ����ϵͳȫ�ֻ��������ĸ��洢·�� */
char*   GetRootEnvStorage();                        /* ����ϵͳȫ�ֻ��������ĸ��洢·�� */

void    SetEnvStorage(const char*);                 /* ���÷���ϵͳȫ�ֻ��������洢·�� */
char*   GetEnvStorage();                            /* ����ϵͳȫ�ֻ��������洢·�� */

int     SetEnv(const char*, const char*, int);      /* ����ȫ�ֻ������� */
char*   GetEnv(const char*);                        /* ����ȫ�ֻ������� */

/* ����ȫ�ֻ�����������ָ��ϵͳ���ƣ� */
int     SetEnvFully(const char*, const char*, const char*, int);

/* ����ȫ�ֻ�����������ָ��ϵͳ���ƣ� */
char*   GetEnvFully(const char*, const char*);


#endif  /* _ENV_BASE_H */
