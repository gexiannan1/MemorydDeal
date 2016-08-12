/******************************************************************
** 文件名:      env.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 描 述:       系统全局环境变量处理函数
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 常量定义
 */
#define     ENV_STORAGE_NAME        "env"               /* 系统全局环境变量存储目录名称(相对于HOME) */


/*****************************************************************
** 函数名:      GetHome
** 输 入:       无
** 输 出:       HOME值
** 功能描述:    返回应用主目录路径
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      IsHomeValid
** 输 入:       无
** 输 出:       TRUE: ok;  FALSE: fail
** 功能描述:    检查主目录是否已正确设置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      InitSysEnv
** 输 入:       无
** 输 出:       TRUE:  成功
**              FALSE: 失败
** 功能描述:    初始化系统全局环境变量的上下文环境
** 全局变量:    无
** 调用模块:    主控程序
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      ResetSysEnv
** 输 入:       无
** 输 出:       TRUE:  成功
**              FALSE: 失败
** 功能描述:    重设所有的系统全局环境变量
** 全局变量:    无
** 调用模块:    主控程序
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      GetSystemName
** 输 入:       无
** 输 出:       系统名称
** 功能描述:    返回系统名称
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.7.1
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      SetSystemName
** 输 入:       无
** 输 出:       无
** 功能描述:    设置系统名称
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.7.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetSystemName(const char *systemName) {
    SetEnv(SYSTEM_NAME, systemName, 1);
}


/*****************************************************************
** 函数名:      GetMainProcessId
** 输 入:       无
** 输 出:       主进程号
** 功能描述:    返回应用系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetMainProcessId() {
    static int  pid = 0;
    char        *value = NULL;

  /*hekh modified 
    *2007.03.19
    *每次都从文件中读进程ID
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
** 函数名:      SetMainProcessId
** 输 入:       主进程号
** 输 出:       无
** 功能描述:    设置应用系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetMainProcessId(int pid) {
    SetEnv(MAIN_PROCESS_ID, Itoa(pid), 1);
}


/*****************************************************************
** 函数名:      SetMainProcessId
** 输 入:       无
** 输 出:       无
** 功能描述:    重置应用系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
ResetMainProcessId() {
    SetMainProcessId(0);
}


/*****************************************************************
** 函数名:      IsRunning
** 输 入:       无
** 输 出:       TRUE:  已启动
**              FALSE: 未启动
** 功能描述:    返回应用系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsRunning() {
    return GetMainProcessId() > 0 ? TRUE : FALSE;
}


/*****************************************************************
** 函数名:      IsShutdown
** 输 入:       无
** 输 出:       TRUE: 已进入Shutdown状态;  FALSE: 未进入Shutdown状态
** 功能描述:    返回应用系统是否已进入Shutdown状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsShutdown() {
    return GetSystemStatus() == SYSTEM_STATUS_RUNNING ? FALSE : TRUE;
}


/*****************************************************************
** 函数名:      GetSystemStatus
** 输 入:       status:  系统运行状态
** 输 出:       无
** 功能描述:    返回系统运行状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.19
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      SetSystemStatus
** 输 入:       status:  系统运行状态
** 输 出:       无
** 功能描述:    设置系统运行状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.20
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetSystemStatus(int status) {
    SetEnv(SHUTDOWN_SIGN, Itoa(status), 1);
}


/*****************************************************************
** 函数名:      IsStubTeRunning
** 输 入:       无
** 输 出:
** 功能描述:    检查数据接收与加工子系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.3
****************************************************************/
BOOL
IsStubTeRunning() {
    return IsSubsystemRunning(SUBSYSTEM_STUB_TE);
}


/*****************************************************************
** 函数名:      IsStubMonitorRunning
** 输 入:       无
** 输 出:
** 功能描述:    检查监控服务子系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.3
****************************************************************/
BOOL
IsStubMonitorRunning() {
    return IsSubsystemRunning(SUBSYSTEM_STUB_MONITOR);
}


/*****************************************************************
** 函数名:      IsDataVenderRunning
** 输 入:       无
** 输 出:
** 功能描述:    检查数据发布子系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.3
****************************************************************/
BOOL
IsDataVenderRunning() {
    return IsSubsystemRunning(SUBSYSTEM_DATA_VENDER);
}


/*****************************************************************
** 函数名:      IsDataRebuilderRunning
** 输 入:       无
** 输 出:
** 功能描述:    检查数据重传子系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.3
****************************************************************/
BOOL
IsDataRebuilderRunning() {
    return IsSubsystemRunning(SUBSYSTEM_DATA_REBUILDER);
}


/*****************************************************************
** 函数名:      GetSubsystemMainProcessId
** 输 入:       无
** 输 出:       主进程号
** 功能描述:    返回子系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      SetSubsystemMainProcessId
** 输 入:       systemName: 子系统名称
**              pid:         主进程号
** 输 出:       无
** 功能描述:    设置子系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetSubsystemMainProcessId(const char *systemName, int pid) {
    SetEnvFully(systemName, MAIN_PROCESS_ID, Itoa(pid), 1);
}


/*****************************************************************
** 函数名:      ResetSubsystemMainProcessId
** 输 入:       systemName: 子系统名称
** 输 出:       无
** 功能描述:    重置子系统主进程号
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
ResetSubsystemMainProcessId(const char *systemName) {
    SetSubsystemMainProcessId(systemName, 0);
}


/*****************************************************************
** 函数名:      IsSubsystemRunning
** 输 入:       systemName: 子系统名称
** 输 出:       TRUE:  已启动
**              FALSE: 未启动
** 功能描述:    返回子系统是否已启动
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsSubsystemRunning(const char *systemName) {
    return GetSubsystemMainProcessId(systemName) > 0 ? TRUE : FALSE;
}


/*****************************************************************
** 函数名:      IsSubsystemShutdown
** 输 入:       systemName: 子系统名称
** 输 出:       TRUE: 已进入Shutdown状态;  FALSE: 未进入Shutdown状态
** 功能描述:    返回子系统是否已进入Shutdown状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsSubsystemShutdown(const char *systemName) {
    return GetSubsystemStatus(systemName) == SYSTEM_STATUS_RUNNING ? FALSE : TRUE;
}


/*****************************************************************
** 函数名:      GetSubsystemStatus
** 输 入:       systemName: 子系统名称
** 输 出:       无
** 功能描述:    返回系统运行状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      SetSubsystemStatus
** 输 入:       systemName: 子系统名称
**              status:     系统运行状态
** 输 出:       无
** 功能描述:    设置子系统运行状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetSubsystemStatus(const char *systemName, int status) {
    SetEnvFully(systemName, SHUTDOWN_SIGN, Itoa(status), 1);
}
