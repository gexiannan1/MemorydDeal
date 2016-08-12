/******************************************************************
** 文件名:      env.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 描 述:       系统全局环境变量处理函数的头文件
** 修订记录：
** 修订日期     修订人          SIR         修复的bug或者修订原因
**-----------------------------------------------------------------
** 2011-08-18	Wu	Zheng		00100		修改GW-QE之间的CS结构
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
 * 开启应用私有存储区域标志
 */
#define     _ENV_PRIVATE_ABLE       1


/*
 * 系统主目录环境变量名称
 */
#define     APPLICATION_HOME        "L2_QE_HOME"


/*
 * 运行时环境常量定义
 */
#define     SYSTEM_NAME             "SYSTEM_NAME"           /* 应用系统名称的环境变量名称 */
#define     MAIN_PROCESS_ID         "MAIN_PID"              /* 应用系统主进程号的环境变量名称 */
#define     SHUTDOWN_SIGN           "SHUTDOWN_SIGN"         /* 应用系统Shutdown标志的环境变量名称 */


/*
 * 系统状态定义
 */
#define     SYSTEM_STATUS_STOP      0                       /* 系统状态.已停止 */
#define     SYSTEM_STATUS_RUNNING   1                       /* 系统状态.正在运行 */
/*** SIR 00100 BEGIN */
#define     SYSTEM_STATUS_ABNORMAL  2                       /* 系统状态，异常 */
/*** SIR 00100 END */

/*
 * 子系统名称常量定义
 */
#define     SUBSYSTEM_STUB_TE           "stub_te"           /* 数据接收与加工子系统名称 */
#define     SUBSYSTEM_STUB_MONITOR      "stub_mon"          /* 监控服务子系统名称 */
#define     SUBSYSTEM_DATA_VENDER       "data_vender"       /* 数据发布子系统名称 */
#define     SUBSYSTEM_DATA_REBUILDER    "data_rebuilder"    /* 数据重传子系统名称 */


/*
 * 应用运行环境函数声明
 */
BOOL    InitSysEnv(char*);                                  /* 初始化系统全局环境变量的上下文环境 */
BOOL    ResetSysEnv(char*);                                 /* 重设所有的系统全局环境变量 */
char*   GetHome();                                          /* 返回应用主目录路径 */
BOOL    IsHomeValid();                                      /* 检查主目录是否已正确设置 */


/*
 * 运行时环境函数声明
 */
char*   GetSystemName();                                    /* 返回系统名称 */
void    SetSystemName(const char*);                         /* 设置系统名称 */

int     GetMainProcessId();                                 /* 返回应用系统主进程号 */
void    SetMainProcessId(int);                              /* 设置应用系统主进程号 */
void    ResetMainProcessId();                               /* 重置应用系统主进程号 */

BOOL    IsRunning();                                        /* 返回应用系统是否已启动 */
BOOL    IsShutdown();                                       /* 返回应用系统是否已进入Shutdown状态 */

int     GetSystemStatus();                                  /* 返回系统运行状态 */
void    SetSystemStatus(int);                               /* 设置系统运行状态 */

BOOL    IsStubTeRunning();                                  /* 检查数据接收与加工子系统是否已启动 */
BOOL    IsDataVenderRunning();                              /* 检查数据发布子系统是否已启动 */
BOOL    IsDataRebuilderRunning();                           /* 检查数据重传子系统是否已启动 */
BOOL    IsStubMonitorRunning();                             /* 检查监控服务子系统是否已启动 */

BOOL    IsSubsystemRunning(const char*);                    /* 返回子系统是否已启动 */
BOOL    IsSubsystemShutdown(const char*);                   /* 返回子系统是否已进入Shutdown状态 */
int     GetSubsystemStatus(const char*);                    /* 返回子系统运行状态 */
void    SetSubsystemStatus(const char*, int);               /* 设置子系统运行状态 */

int     GetSubsystemMainProcessId(const char*);             /* 返回子系统主进程号 */
void    SetSubsystemMainProcessId(const char*, int);        /* 设置子系统主进程号 */
void    ResetSubsystemMainProcessId(const char*);           /* 重置子系统主进程号 */


#endif  /* _SYS_ENV_H */
