/******************************************************************
** 文件名:      symbol.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.4.18
** 描 述:       公共类型、符号、常量定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/symbol.h,v 1.1 2012/03/29 01:20:20 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:20 $
 */

#ifndef _COMMON_SYMBOL_H
#define _COMMON_SYMBOL_H

/*
 * 定义BOOL类型
 */
typedef int         BOOL;

#define TRUE        1
#define FALSE       0


/*
 * 定义BOOL类型
 */
#ifndef NULL
#define NULL     0        /* NULL定义 */

#endif

/*
 * 定义静态缓冲数组大小
 */
#define _STATIC_BUFFER_ARRAY_SIZE		11


#endif  /* _COMMON_SYMBOL_H */
