/******************************************************************
** 文件名:      macro.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2006.3.2
** 描 述:       应用配置文件，宏参数处理
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/macro/macro.c,v 1.1 2012/03/29 01:20:15 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:15 $
 */


#include    "macro.h"
#include    "../string/common_str.h"


/*****************************************************************
** 函数名:      ReplaceMacroAppHome
** 输 入:       pStr: 字符串
** 输 出:       
** 功能描述:    替换${APP_HOME}宏
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.2
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
ReplaceMacroAppHome(const char *target) {
	return Replace(target, MACRO_APP_HOME, GetHome());
}


/*****************************************************************
** 函数名:      ReplaceMacroSystemName
** 输 入:       pStr: 字符串
** 输 出:       
** 功能描述:    替换${SYSTEM_NAME}宏
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.2
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
ReplaceMacroSystemName(const char *target, const char *value) {
	return Replace(target, MACRO_SYSTEM_NAME, value);
}
