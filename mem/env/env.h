/******************************************************************
** �ļ���:      env.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.12
** �� ��:       ϵͳȫ�ֻ���������������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���          SIR         �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2011-08-18	Wu	Zheng		00100		�޸�GW-QE֮���CS�ṹ
******************************************************************/


/*
 * $Header: /RepLevel2/v3/src/qe/src/public/env/env.h,v 1.1 2012/03/29 01:19:48 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:48 $
 */

#ifndef _SYS_ENV_H
#define _SYS_ENV_H


#include    "../util/symbol.h"
#include    "../util/env_base/env_base.h"


/*
 * ����Ӧ��˽�д洢�����־
 */
#define     _ENV_PRIVATE_ABLE       1


/*
 * ϵͳ��Ŀ¼������������
 */
#define     APPLICATION_HOME        "L2_QE_HOME"


/*
 * ����ʱ������������
 */
#define     SYSTEM_NAME             "SYSTEM_NAME"           /* Ӧ��ϵͳ���ƵĻ����������� */
#define     MAIN_PROCESS_ID         "MAIN_PID"              /* Ӧ��ϵͳ�����̺ŵĻ����������� */
#define     SHUTDOWN_SIGN           "SHUTDOWN_SIGN"         /* Ӧ��ϵͳShutdown��־�Ļ����������� */


/*
 * ϵͳ״̬����
 */
#define     SYSTEM_STATUS_STOP      0                       /* ϵͳ״̬.��ֹͣ */
#define     SYSTEM_STATUS_RUNNING   1                       /* ϵͳ״̬.�������� */
/*** SIR 00100 BEGIN */
#define     SYSTEM_STATUS_ABNORMAL  2                       /* ϵͳ״̬���쳣 */
/*** SIR 00100 END */

/*
 * ��ϵͳ���Ƴ�������
 */
#define     SUBSYSTEM_STUB_TE           "stub_te"           /* ���ݽ�����ӹ���ϵͳ���� */
#define     SUBSYSTEM_STUB_MONITOR      "stub_mon"          /* ��ط�����ϵͳ���� */
#define     SUBSYSTEM_DATA_VENDER       "data_vender"       /* ���ݷ�����ϵͳ���� */
#define     SUBSYSTEM_DATA_REBUILDER    "data_rebuilder"    /* �����ش���ϵͳ���� */


/*
 * Ӧ�����л�����������
 */
BOOL    InitSysEnv(char*);                                  /* ��ʼ��ϵͳȫ�ֻ��������������Ļ��� */
BOOL    ResetSysEnv(char*);                                 /* �������е�ϵͳȫ�ֻ������� */
char*   GetHome();                                          /* ����Ӧ����Ŀ¼·�� */
BOOL    IsHomeValid();                                      /* �����Ŀ¼�Ƿ�����ȷ���� */


/*
 * ����ʱ������������
 */
char*   GetSystemName();                                    /* ����ϵͳ���� */
void    SetSystemName(const char*);                         /* ����ϵͳ���� */

int     GetMainProcessId();                                 /* ����Ӧ��ϵͳ�����̺� */
void    SetMainProcessId(int);                              /* ����Ӧ��ϵͳ�����̺� */
void    ResetMainProcessId();                               /* ����Ӧ��ϵͳ�����̺� */

BOOL    IsRunning();                                        /* ����Ӧ��ϵͳ�Ƿ������� */
BOOL    IsShutdown();                                       /* ����Ӧ��ϵͳ�Ƿ��ѽ���Shutdown״̬ */

int     GetSystemStatus();                                  /* ����ϵͳ����״̬ */
void    SetSystemStatus(int);                               /* ����ϵͳ����״̬ */

BOOL    IsStubTeRunning();                                  /* ������ݽ�����ӹ���ϵͳ�Ƿ������� */
BOOL    IsDataVenderRunning();                              /* ������ݷ�����ϵͳ�Ƿ������� */
BOOL    IsDataRebuilderRunning();                           /* ��������ش���ϵͳ�Ƿ������� */
BOOL    IsStubMonitorRunning();                             /* ����ط�����ϵͳ�Ƿ������� */

BOOL    IsSubsystemRunning(const char*);                    /* ������ϵͳ�Ƿ������� */
BOOL    IsSubsystemShutdown(const char*);                   /* ������ϵͳ�Ƿ��ѽ���Shutdown״̬ */
int     GetSubsystemStatus(const char*);                    /* ������ϵͳ����״̬ */
void    SetSubsystemStatus(const char*, int);               /* ������ϵͳ����״̬ */

int     GetSubsystemMainProcessId(const char*);             /* ������ϵͳ�����̺� */
void    SetSubsystemMainProcessId(const char*, int);        /* ������ϵͳ�����̺� */
void    ResetSubsystemMainProcessId(const char*);           /* ������ϵͳ�����̺� */


#endif  /* _SYS_ENV_H */
