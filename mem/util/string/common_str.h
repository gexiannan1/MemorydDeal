/******************************************************************
** 文件名:      common_str.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.4.17
** 描 述:       字符串处理函数的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/string/common_str.h,v 1.1 2012/03/29 01:20:19 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:19 $
 */

#ifndef _COMMON_STRING_H
#define _COMMON_STRING_H


#include    "../symbol.h"


/*
 * 函数原型
 */

char*       Trim(const char*);                                                      /* 返回去除空格后的字符串 */
char*       Ltrim(const char*);                                                     /* 返回去除左端空格后的字符串 */
char*       Rtrim(const char*);                                                     /* 返回去除右端空格后的字符串 */
char*       TrimRude(char*);                                                        /* 去除字符串前后端的空格（修改字符串本身） */
char*       RtrimRude(char*);                                                       /* 去除字符串右端的空格（修改字符串本身） */
char*       Replace(const char*, const char*, const char*);                         /* 替换目标字符串中的待替换字符串 */
char*       ReplaceAppointed(const char*, const char*, const char*, const int);     /* 替换目标字符串中的待替换字符串（可指定替换个数） */
char*       RReplace(const char*, const char*, const char*);                        /* 替换目标字符串中的待替换字符串（从右侧开始） */
char*       RReplaceAppointed(const char*, const char*, const char*, const int);    /* 替换目标字符串中的待替换字符串（从右侧开始，可指定替换个数） */
char*       ReplaceCharAll(const char*, const char, const char, const int);         /* 替换字符串中的所有字符 */

long long   Atoll(const char*);                                                     /* 字符串转为长整型数值 */
long long	StringToll(char*, unsigned int);										/* 字符串转换为长整型，指定小数点向右移动point位*/
int			StringToInt(char*, unsigned int);										/* 字符串转换为整型，指定小数点向右移动point位*/

char*       Itoa(int);                                                              /* 整型数值转为字符串 */
char*       Ltoa(long);                                                             /* 数值转为字符串 */
char*       Lltoa(long long);                                                       /* 长整型数值转为字符串 */

int         Atox(const char*);                                                      /* 转换十六进制形式的字符串为整型数值 */
char*       Xtoa(int);                                                              /* 整型数值转为十六进制形式的字符串 */

BOOL        IsEmptyString(const char*);                                             /* 返回字符串是否是空字符串 */
BOOL        IsNumeric(const char*);                                                 /* 校验字符串是否是有效的数值型字符串 */
BOOL        IsDate(const char*, const char*);                                       /* 校验字符串是否是有效的日期型字符串 */
BOOL        IsStringEqual(const char*, const char*);                                /* 返回字符串是否是空字符串 */



#endif  /* _COMMON_STRING_H */
