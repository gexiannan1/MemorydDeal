/******************************************************************
** 文件名:      common_lib.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.13
** 描 述:       公共函数库
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/lib/common_lib.h,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#ifndef _COMMON_LIB_H
#define _COMMON_LIB_H


#include	"../string/common_str.h"


/*
 * 宏定义
 */

#define     Min(a,b)    ((a) < (b) ? (a) : (b))
#define     Max(a,b)    ((a) > (b) ? (a) : (b))

#define     Floor(f)    ((long)(f))
#define     Ceil(f)     ((f) - (long)(f) > 0.000001 ? (long)(f) + 1 : (long)(f))

/* 计算数组长度 */
#define     Dim(x)                          (sizeof(x) / sizeof(x[0]))

/* 若x小于0则返回0 */
#define     TRIM_ZERO(x)                    ((x) < 0 ? 0 : (x))

/* 若x小于等于0则返回v */
#define     SWITCH_SMALLNESS_NUMBER(x,v)    ((x) <= 0 ? v : (x))

/* 若字符串s为空则返回v */
#define     SWITCH_EMPTY_STR(s,v)           ((s) ? (strlen(RtrimRude((s))) > 0 ? (s) : (v)) : (v))


/*
 * 函数原型
 */


#endif  /* _COMMON_LIB_H */
