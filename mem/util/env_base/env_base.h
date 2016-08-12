/******************************************************************
** 文件名:      env_base.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 描 述:       全局环境变量处理函数的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 函数声明
 */
int     InitEnv(const char*, const char *);         /* 初始化全局环境变量的上下文环境 */
int     ResetEnv(const char*, const char *);     	/* 重设全局环境变量 */

void    SetRootEnvStorage(const char*);             /* 设置系统全局环境变量的根存储路径 */
char*   GetRootEnvStorage();                        /* 返回系统全局环境变量的根存储路径 */

void    SetEnvStorage(const char*);                 /* 设置返回系统全局环境变量存储路径 */
char*   GetEnvStorage();                            /* 返回系统全局环境变量存储路径 */

int     SetEnv(const char*, const char*, int);      /* 设置全局环境变量 */
char*   GetEnv(const char*);                        /* 返回全局环境变量 */

/* 设置全局环境变量（可指定系统名称） */
int     SetEnvFully(const char*, const char*, const char*, int);

/* 返回全局环境变量（可指定系统名称） */
char*   GetEnvFully(const char*, const char*);


#endif  /* _ENV_BASE_H */
