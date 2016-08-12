/******************************************************************
** 文件名:      macro.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2006.3.2
** 描 述:       应用配置文件，宏参数定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 定义BOOL类型
 */
#define     MACRO_APP_HOME         	"${APP_HOME}"			/* 配置参数宏，应用主目录 */
#define     MACRO_SYSTEM_NAME  		"${SYSTEM_NAME}"		/* 配置参数宏，系统名称 */


/*
 * 外部函数引用
 */
extern char*	GetHome();									/* 返回应用主目录路径 */


/*
 * 函数声明
 */
char*			ReplaceMacroAppHome(const char*);			/* 替换${APP_HOME}宏 */
char*			ReplaceMacroSystemName(const char *target, 
				const char *value);							/* 替换${SYSTEM_NAME}宏 */


#endif  /* _COMMON_MACRO_H */
