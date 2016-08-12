/******************************************************************
** �ļ���:      env.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.12
** �� ��:       ϵͳȫ�ֻ�������������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/env/env.c,v 1.1 2012/03/29 01:19:48 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:48 $
 */


#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <time.h>
#include    "env.h"
#include    "../util/symbol.h"
#include    "../util/env_base/env_base.h"
#include    "../util/string/common_str.h"
#include    "../util/file/common_file.h"


/*
 * ��������
 */
#define     ENV_STORAGE_NAME        "env"               /* ϵͳȫ�ֻ��������洢Ŀ¼����(�����HOME) */


/*****************************************************************
** ������:      GetHome
** �� ��:       ��
** �� ��:       HOMEֵ
** ��������:    ����Ӧ����Ŀ¼·��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetHome() {
    char    *pHome;

    pHome = (char*) getenv(APPLICATION_HOME);

    if (pHome != NULL && strlen(pHome) <= 0) {
        pHome = NULL;
    }

    return pHome;
}


/*****************************************************************
** ������:      IsHomeValid
** �� ��:       ��
** �� ��:       TRUE: ok;  FALSE: fail
** ��������:    �����Ŀ¼�Ƿ�����ȷ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsHomeValid() {
    char    *pHome;

    if (!(pHome = GetHome())) {
        return FALSE;
    }
    return IsDir(pHome);
}


/*****************************************************************
** ������:      InitSysEnv
** �� ��:       ��
** �� ��:       TRUE:  �ɹ�
**              FALSE: ʧ��
** ��������:    ��ʼ��ϵͳȫ�ֻ��������������Ļ���
** ȫ�ֱ���:    ��
** ����ģ��:    ���س���
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
InitSysEnv(char *systemName) {
    char    envRoot[256]    = "";
    char    *sysName        = NULL;

    sprintf(envRoot, "%s/%s", GetHome(), ENV_STORAGE_NAME);

#if _ENV_PRIVATE_ABLE
    if (systemName) {
        sysName = systemName;
    } else {
        sysName = GetSystemName();
    }
#endif

    if (InitEnv(envRoot, sysName) < 0) {
        return FALSE;
    }

    if (systemName) {
        SetSystemName(systemName);
    }
    return TRUE;
}


/*****************************************************************
** ������:      ResetSysEnv
** �� ��:       ��
** �� ��:       TRUE:  �ɹ�
**              FALSE: ʧ��
** ��������:    �������е�ϵͳȫ�ֻ�������
** ȫ�ֱ���:    ��
** ����ģ��:    ���س���
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
ResetSysEnv(char *systemName) {
    char    envRoot[256]    = "";
    char    *sysName        = NULL;

    sprintf(envRoot, "%s/%s", GetHome(), ENV_STORAGE_NAME);

#if _ENV_PRIVATE_ABLE
    if (systemName) {
        sysName = systemName;
    } else {
        sysName = GetSystemName();
    }
#endif

    if (ResetEnv(envRoot, sysName) < 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** ������:      GetSystemName
** �� ��:       ��
** �� ��:       ϵͳ����
** ��������:    ����ϵͳ����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.7.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetSystemName() {
    static char     systemName[33] = "";
    char            *value = NULL;

    if (strlen(systemName) <= 0) {
        if ((value = GetEnv(SYSTEM_NAME)) != NULL) {
            strncpy(systemName, value, sizeof(systemName));
        }
    }
    return systemName;
}


/*****************************************************************
** ������:      SetSystemName
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ϵͳ����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.7.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetSystemName(const char *systemName) {
    SetEnv(SYSTEM_NAME, systemName, 1);
}


/*****************************************************************
** ������:      GetMainProcessId
** �� ��:       ��
** �� ��:       �����̺�
** ��������:    ����Ӧ��ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetMainProcessId() {
    static int  pid = 0;
    char        *value = NULL;

  /*hekh modified 
    *2007.03.19
    *ÿ�ζ����ļ��ж�����ID
    */
    if ((value = GetEnv(MAIN_PROCESS_ID)) != NULL) {
            pid = atoi(value);
        }
    /*
    if (pid <= 0) {
        if ((value = GetEnv(MAIN_PROCESS_ID)) != NULL) {
            pid = atoi(value);
        }
    }
    */
    return pid;
}


/*****************************************************************
** ������:      SetMainProcessId
** �� ��:       �����̺�
** �� ��:       ��
** ��������:    ����Ӧ��ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetMainProcessId(int pid) {
    SetEnv(MAIN_PROCESS_ID, Itoa(pid), 1);
}


/*****************************************************************
** ������:      SetMainProcessId
** �� ��:       ��
** �� ��:       ��
** ��������:    ����Ӧ��ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
ResetMainProcessId() {
    SetMainProcessId(0);
}


/*****************************************************************
** ������:      IsRunning
** �� ��:       ��
** �� ��:       TRUE:  ������
**              FALSE: δ����
** ��������:    ����Ӧ��ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsRunning() {
    return GetMainProcessId() > 0 ? TRUE : FALSE;
}


/*****************************************************************
** ������:      IsShutdown
** �� ��:       ��
** �� ��:       TRUE: �ѽ���Shutdown״̬;  FALSE: δ����Shutdown״̬
** ��������:    ����Ӧ��ϵͳ�Ƿ��ѽ���Shutdown״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsShutdown() {
    return GetSystemStatus() == SYSTEM_STATUS_RUNNING ? FALSE : TRUE;
}


/*****************************************************************
** ������:      GetSystemStatus
** �� ��:       status:  ϵͳ����״̬
** �� ��:       ��
** ��������:    ����ϵͳ����״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetSystemStatus() {
    char    *value;

    if ((value = GetEnv(SHUTDOWN_SIGN)) == NULL) {
        return SYSTEM_STATUS_STOP;
    } else {
        return atoi(value);
    }
}


/*****************************************************************
** ������:      SetSystemStatus
** �� ��:       status:  ϵͳ����״̬
** �� ��:       ��
** ��������:    ����ϵͳ����״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetSystemStatus(int status) {
    SetEnv(SHUTDOWN_SIGN, Itoa(status), 1);
}


/*****************************************************************
** ������:      IsStubTeRunning
** �� ��:       ��
** �� ��:
** ��������:    ������ݽ�����ӹ���ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.3
****************************************************************/
BOOL
IsStubTeRunning() {
    return IsSubsystemRunning(SUBSYSTEM_STUB_TE);
}


/*****************************************************************
** ������:      IsStubMonitorRunning
** �� ��:       ��
** �� ��:
** ��������:    ����ط�����ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.3
****************************************************************/
BOOL
IsStubMonitorRunning() {
    return IsSubsystemRunning(SUBSYSTEM_STUB_MONITOR);
}


/*****************************************************************
** ������:      IsDataVenderRunning
** �� ��:       ��
** �� ��:
** ��������:    ������ݷ�����ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.3
****************************************************************/
BOOL
IsDataVenderRunning() {
    return IsSubsystemRunning(SUBSYSTEM_DATA_VENDER);
}


/*****************************************************************
** ������:      IsDataRebuilderRunning
** �� ��:       ��
** �� ��:
** ��������:    ��������ش���ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.3
****************************************************************/
BOOL
IsDataRebuilderRunning() {
    return IsSubsystemRunning(SUBSYSTEM_DATA_REBUILDER);
}


/*****************************************************************
** ������:      GetSubsystemMainProcessId
** �� ��:       ��
** �� ��:       �����̺�
** ��������:    ������ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetSubsystemMainProcessId(const char *systemName) {
    char        *value = NULL;

    if ((value = GetEnvFully(systemName, MAIN_PROCESS_ID))) {
        return atoi(value);
    }
    return 0;
}


/*****************************************************************
** ������:      SetSubsystemMainProcessId
** �� ��:       systemName: ��ϵͳ����
**              pid:         �����̺�
** �� ��:       ��
** ��������:    ������ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetSubsystemMainProcessId(const char *systemName, int pid) {
    SetEnvFully(systemName, MAIN_PROCESS_ID, Itoa(pid), 1);
}


/*****************************************************************
** ������:      ResetSubsystemMainProcessId
** �� ��:       systemName: ��ϵͳ����
** �� ��:       ��
** ��������:    ������ϵͳ�����̺�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
ResetSubsystemMainProcessId(const char *systemName) {
    SetSubsystemMainProcessId(systemName, 0);
}


/*****************************************************************
** ������:      IsSubsystemRunning
** �� ��:       systemName: ��ϵͳ����
** �� ��:       TRUE:  ������
**              FALSE: δ����
** ��������:    ������ϵͳ�Ƿ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsSubsystemRunning(const char *systemName) {
    return GetSubsystemMainProcessId(systemName) > 0 ? TRUE : FALSE;
}


/*****************************************************************
** ������:      IsSubsystemShutdown
** �� ��:       systemName: ��ϵͳ����
** �� ��:       TRUE: �ѽ���Shutdown״̬;  FALSE: δ����Shutdown״̬
** ��������:    ������ϵͳ�Ƿ��ѽ���Shutdown״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsSubsystemShutdown(const char *systemName) {
    return GetSubsystemStatus(systemName) == SYSTEM_STATUS_RUNNING ? FALSE : TRUE;
}


/*****************************************************************
** ������:      GetSubsystemStatus
** �� ��:       systemName: ��ϵͳ����
** �� ��:       ��
** ��������:    ����ϵͳ����״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetSubsystemStatus(const char *systemName) {
    char    *value;

    if ((value = GetEnvFully(systemName, SHUTDOWN_SIGN)) == NULL) {
        return SYSTEM_STATUS_STOP;
    } else {
        return atoi(value);
    }
}


/*****************************************************************
** ������:      SetSubsystemStatus
** �� ��:       systemName: ��ϵͳ����
**              status:     ϵͳ����״̬
** �� ��:       ��
** ��������:    ������ϵͳ����״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.8
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetSubsystemStatus(const char *systemName, int status) {
    SetEnvFully(systemName, SHUTDOWN_SIGN, Itoa(status), 1);
}
