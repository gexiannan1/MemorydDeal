/******************************************************************
** 文件名:      common_str.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.4.17
** 描 述:       字符串处理函数
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/string/common_str.c,v 1.1 2012/03/29 01:20:19 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:19 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "common_str.h"
#include    "../symbol.h"
#include    "../time/common_time.h"

/*****************************************************************
** 函数名:      Trim
** 输 入:       pStr: 字符串
** 输 出:
** 功能描述:    返回去除空格后的字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Trim(const char* s) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][8192];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    strncpy(buf[i], s, sizeof(buf[i]));
    return TrimRude(buf[i]);
}


/*****************************************************************
** 函数名:      Ltrim
** 输 入:       pStr: 字符串
** 输 出:
** 功能描述:    返回去除左端空格后的字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Ltrim(const char* pStr) {
    int beginPos = 0;

    while (pStr[beginPos] == ' ' || pStr[beginPos] == '\t'
            || pStr[beginPos] == '\n' || pStr[beginPos] == '\r')
        beginPos++;

    return (char*) (pStr + beginPos);
}


/*****************************************************************
** 函数名:      Rtrim
** 输 入:       pStr: 字符串
** 输 出:
** 功能描述:    Replace
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Rtrim(const char* s) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][8192];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    strncpy(buf[i], s, sizeof(buf[i]));
    return RtrimRude(buf[i]);
}


/*****************************************************************
** 函数名:      TrimRude
** 输 入:       pStr: 字符串
** 输 出:       去除空格后的字符串
** 功能描述:    去除字符串前后端的空格
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
TrimRude(char* pStr) {
    return Ltrim(RtrimRude(pStr));
}


/*****************************************************************
** 函数名:      RtrimRude
** 输 入:       pStr: 字符串
** 输 出:       去除空格后的字符串
** 功能描述:    去除字符串右端的空格
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
RtrimRude(char* pStr) {
    int endPos = strlen(pStr) - 1;

    while (endPos >= 0 &&
            (pStr[endPos] == ' ' || pStr[endPos] == '\t'
                    || pStr[endPos] == '\n' || pStr[endPos] == '\r'))
        endPos--;

    pStr[++endPos] = '\0';
    return (char*) pStr;
}


/*****************************************************************
** 函数名:      Replace
** 输 入:       target: 待处理的目标字符串
**              oldStr: 待替换字符串
** 输 出:
** 功能描述:    替换目标字符串中的待替换字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Replace(const char *target, const char *oldStr, const char *newStr) {
    return ReplaceAppointed(target, oldStr, newStr, -1);
}


/*****************************************************************
** 函数名:      ReplaceAppointed
** 输 入:       target: 待处理的目标字符串
**              oldStr: 待替换字符串
** 输 出:
** 功能描述:    替换目标字符串中的待替换字符串（可指定替换个数）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
ReplaceAppointed(const char *target, const char *oldStr, const char *newStr,
        const int count) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][8192];

    char        *ptr        = NULL;
    char        *targetPtr  = NULL;
    char        *bufPtr     = NULL;
    int         oldStrLen   = 0;
    int         newStrLen   = 0;
    int         j           = 0;

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    targetPtr = (char*) target;
    bufPtr = buf[i];

    if (! target || ! oldStr || ! newStr || count == 0
            || ! (ptr = strstr((char *)target, oldStr))) {
        return targetPtr;
    }

    oldStrLen = strlen(oldStr);
    newStrLen = strlen(newStr);
    j = 0;

    do {
        strncpy(bufPtr, targetPtr, ptr - targetPtr);
        bufPtr += ptr - targetPtr;

        strncpy(bufPtr, newStr, newStrLen);
        bufPtr += newStrLen;

        targetPtr = ptr + oldStrLen;
        strcpy(bufPtr, targetPtr);

        j ++;
    } while ((ptr = strstr(targetPtr, oldStr)) && (count < 0 || j < count));

    return buf[i];
}


/*****************************************************************
** 函数名:      RReplace
** 输 入:       target: 待处理的目标字符串
**              oldStr: 待替换字符串
** 输 出:
** 功能描述:    替换目标字符串中的待替换字符串（从右侧开始）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
RReplace(const char *target, const char *oldStr, const char *newStr) {
    return RReplaceAppointed(target, oldStr, newStr, -1);
}


/*****************************************************************
** 函数名:      RReplaceAppointed
** 输 入:       target: 待处理的目标字符串
**              oldStr: 待替换字符串
** 输 出:
** 功能描述:    替换目标字符串中的待替换字符串（从右侧开始，可指定替换个数）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
RReplaceAppointed(const char *target, const char *oldStr, const char *newStr,
        const int count) {
    return (char*) target;
    /*static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][8192];

    char        tmpBuf[8192];
    char        *ptr        = NULL;
    char        *targetPtr  = NULL;
    char        *bufPtr     = NULL;
    int         oldStrLen   = 0;
    int         newStrLen   = 0;
    int         j           = 0;

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    strncpy(buf[i], target, sizeof(buf[i]));
    targetPtr = buf[i];
    bufPtr = tmpBuf;

    if (! target || ! oldStr || ! newStr || count == 0
            || ! (ptr = strrstr(targetPtr, oldStr))) {
        return (char*) target;
    }

    oldStrLen = strlen(oldStr);
    newStrLen = strlen(newStr);
    j = 0;

    do {
        strncpy(bufPtr, targetPtr, ptr - targetPtr);
        bufPtr += ptr - targetPtr;

        strncpy(bufPtr, newStr, newStrLen);
        bufPtr += newStrLen;

        strcpy(bufPtr, ptr + oldStrLen);

        strcpy(targetPtr, tmpBuf);
        bufPtr = tmpBuf;

        j ++;
    } while ((ptr = strrstr(targetPtr, oldStr)) && (count < 0 || j < count));

    return buf[i];*/
}


/*****************************************************************
** 函数名:      ReplaceCharAll
** 输 入:       target: 待处理的字符串
** 输 出:
** 功能描述:    替换字符串中的所有字符
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
ReplaceCharAll(const char *target, const char c1, const char c2, const int maxLen) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][8192];
    char        *ptr = NULL;
    int         count = 0;

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    strcpy(buf[i], target);
    ptr = buf[i];

    while (*ptr != 0 && (maxLen <= 0 || count < maxLen)) {
        if (*ptr == c1) *ptr = c2;
        count ++;
    }
    return buf[i];
}


/*****************************************************************
** 函数名:      Atoll
** 输 入:       char*
** 输 出:       long long
** 功能描述:    转换字符串->long long
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.9.3
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
long long
Atoll(const char *str) {
    long long value = 0;

    (void) sscanf(str, "%lld", &value);
    return value;
}


/*****************************************************************
** 函数名:      StringToll
** 输 入:       char* unsigned int
** 输 出:       long long
** 功能描述:    字符串转换为长整型，指定小数点向右移动point位
** 全局变量:    无
** 调用模块:    无
** 创建人:      Li Bo
** 日 期:       2006.9.14
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
long long	
StringToll(char* str, unsigned int point){
	char strTemp[64];
	char *p = NULL;
	int  n = 1;
	long long value = 0;
	unsigned int i = 0;
	
	if(str == NULL || strlen(str) > 63){
		return 0;
	}
	
	memset(strTemp, 0, 64);
	strcpy(strTemp, str);
	
	p = strchr(strTemp, '.');
	if(p == NULL){
		sscanf(strTemp, "%lld", &value);
		for(i = 0;i<point;i++){
			n = n * 10;
		}
		return value * n;
	}
	
	memmove(p, p + 1, point);
	for(i = p - strTemp; i < p - strTemp + point; i++){
		if(strTemp[i] == 0){
			strTemp[i] = '0';
		}
	}
	strTemp[i] = 0;
	sscanf(strTemp, "%lld", &value);
	
	return value;
}



/*****************************************************************
** 函数名:      StringToInt
** 输 入:       char* unsigned int
** 输 出:       int
** 功能描述:    字符串转换为整型，指定小数点向右移动point位
** 全局变量:    无
** 调用模块:    无
** 创建人:      Li Bo
** 日 期:       2006.9.14
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int	
StringToInt(char* str, unsigned int point){
	char strTemp[32];
	char *p = NULL;
	int  n = 1;
	unsigned int i = 0;
	
	if(str == NULL || strlen(str) > 31){
		return 0;
	}
	
	memset(strTemp, 0, 32);
	strcpy(strTemp, str);
	
	p = strchr(strTemp, '.');
	if(p == NULL){
		for(i = 0;i<point;i++){
			n = n * 10;
		}
		return atoi(str) * n;
	}
	
	memmove(p, p + 1, point);
	for(i = p - strTemp; i < p - strTemp + point; i++){
		if(strTemp[i] == 0){
			strTemp[i] = '0';
		}
	}
	strTemp[i] = 0;
	
	return atoi(strTemp);
}


/*****************************************************************
** 函数名:      Itoa
** 输 入:       int
** 输 出:       转换后的字符串
** 功能描述:    整型数值转为字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Itoa(int num) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][15];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    sprintf(buf[i], "%d", num);

    return (char*) buf[i];
}


/*****************************************************************
** 函数名:      Ltoa
** 输 入:       long
** 输 出:       转换后的字符串
** 功能描述:    长整型数值转为字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Ltoa(long num) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][23];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    sprintf(buf[i], "%ld", num);

    return (char*) buf[i];
}


/*****************************************************************
** 函数名:      Lltoa
** 输 入:       long long
** 输 出:       转换后的字符串
** 功能描述:    long long型数值转为字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Lltoa(long long num) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][43];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    sprintf(buf[i], "%lld", num);

    return (char*) buf[i];
}


/*****************************************************************
** 函数名:      Atox
** 输 入:       char*
** 输 出:       int
** 功能描述:    转换十六进制形式的字符串为整型数值
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.10.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
Atox(const char *str) {
    int value = 0;

    sscanf(str, "%xd", &value);

    return value;
}


/*****************************************************************
** 函数名:      Xtoa
** 输 入:       int
** 输 出:       转换后的字符串
** 功能描述:    整型数值转为十六进制形式的字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.10.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
Xtoa(int num) {
    static int  i = 0;
    static char buf[_STATIC_BUFFER_ARRAY_SIZE][15];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    sprintf(buf[i], "%x", num);

    return (char*) buf[i];
}


/*****************************************************************
** 函数名:      IsEmptyString
** 输 入:       s:      待校验字符串
** 输 出:       TRUE:   为空
**              FALSE:  不为空
** 功能描述:    返回字符串是否是空字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsEmptyString(const char *s) {
    if (s && strlen(Trim(s)) > 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      IsNumeric
** 输 入:       s:      待校验字符串
** 输 出:       TRUE:   是有效的数值型字符串
**              FALSE:  不是有效的数值型字符串
** 功能描述:    校验字符串是否是有效的数值型字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsNumeric(const char *s) {
    int     i = 0;

    for (i = 0; i < strlen(s); i++) {
        if (s[i] < 48 || s[i] > 57) {
            return FALSE;
        }
    }

    return i > 0 ? TRUE : FALSE;
}


/*****************************************************************
** 函数名:      IsDate
** 输 入:       s:       待校验字符串
**              format:  日期格式
** 输 出:       TRUE:    是有效的日期型字符串
**              FALSE:   不是有效的日期型字符串
** 功能描述:    校验字符串是否是有效的日期型字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsDate(const char *s, const char *format) {
    return IsValidDate(s, format);
}
