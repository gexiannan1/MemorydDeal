/******************************************************************
** �ļ���:      common_str.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.4.17
** �� ��:       �ַ���������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
** ������:      Trim
** �� ��:       pStr: �ַ���
** �� ��:
** ��������:    ����ȥ���ո����ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Ltrim
** �� ��:       pStr: �ַ���
** �� ��:
** ��������:    ����ȥ����˿ո����ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Rtrim
** �� ��:       pStr: �ַ���
** �� ��:
** ��������:    Replace
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      TrimRude
** �� ��:       pStr: �ַ���
** �� ��:       ȥ���ո����ַ���
** ��������:    ȥ���ַ���ǰ��˵Ŀո�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.17
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
TrimRude(char* pStr) {
    return Ltrim(RtrimRude(pStr));
}


/*****************************************************************
** ������:      RtrimRude
** �� ��:       pStr: �ַ���
** �� ��:       ȥ���ո����ַ���
** ��������:    ȥ���ַ����Ҷ˵Ŀո�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.17
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Replace
** �� ��:       target: �������Ŀ���ַ���
**              oldStr: ���滻�ַ���
** �� ��:
** ��������:    �滻Ŀ���ַ����еĴ��滻�ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
Replace(const char *target, const char *oldStr, const char *newStr) {
    return ReplaceAppointed(target, oldStr, newStr, -1);
}


/*****************************************************************
** ������:      ReplaceAppointed
** �� ��:       target: �������Ŀ���ַ���
**              oldStr: ���滻�ַ���
** �� ��:
** ��������:    �滻Ŀ���ַ����еĴ��滻�ַ�������ָ���滻������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2006.3.9
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      RReplace
** �� ��:       target: �������Ŀ���ַ���
**              oldStr: ���滻�ַ���
** �� ��:
** ��������:    �滻Ŀ���ַ����еĴ��滻�ַ��������Ҳ࿪ʼ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2006.3.9
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
RReplace(const char *target, const char *oldStr, const char *newStr) {
    return RReplaceAppointed(target, oldStr, newStr, -1);
}


/*****************************************************************
** ������:      RReplaceAppointed
** �� ��:       target: �������Ŀ���ַ���
**              oldStr: ���滻�ַ���
** �� ��:
** ��������:    �滻Ŀ���ַ����еĴ��滻�ַ��������Ҳ࿪ʼ����ָ���滻������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2006.3.9
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      ReplaceCharAll
** �� ��:       target: ��������ַ���
** �� ��:
** ��������:    �滻�ַ����е������ַ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Atoll
** �� ��:       char*
** �� ��:       long long
** ��������:    ת���ַ���->long long
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.9.3
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
long long
Atoll(const char *str) {
    long long value = 0;

    (void) sscanf(str, "%lld", &value);
    return value;
}


/*****************************************************************
** ������:      StringToll
** �� ��:       char* unsigned int
** �� ��:       long long
** ��������:    �ַ���ת��Ϊ�����ͣ�ָ��С���������ƶ�pointλ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Li Bo
** �� ��:       2006.9.14
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      StringToInt
** �� ��:       char* unsigned int
** �� ��:       int
** ��������:    �ַ���ת��Ϊ���ͣ�ָ��С���������ƶ�pointλ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Li Bo
** �� ��:       2006.9.14
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Itoa
** �� ��:       int
** �� ��:       ת������ַ���
** ��������:    ������ֵתΪ�ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.18
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Ltoa
** �� ��:       long
** �� ��:       ת������ַ���
** ��������:    ��������ֵתΪ�ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.18
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Lltoa
** �� ��:       long long
** �� ��:       ת������ַ���
** ��������:    long long����ֵתΪ�ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.18
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      Atox
** �� ��:       char*
** �� ��:       int
** ��������:    ת��ʮ��������ʽ���ַ���Ϊ������ֵ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.10.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
Atox(const char *str) {
    int value = 0;

    sscanf(str, "%xd", &value);

    return value;
}


/*****************************************************************
** ������:      Xtoa
** �� ��:       int
** �� ��:       ת������ַ���
** ��������:    ������ֵתΪʮ��������ʽ���ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.10.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsEmptyString
** �� ��:       s:      ��У���ַ���
** �� ��:       TRUE:   Ϊ��
**              FALSE:  ��Ϊ��
** ��������:    �����ַ����Ƿ��ǿ��ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsEmptyString(const char *s) {
    if (s && strlen(Trim(s)) > 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** ������:      IsNumeric
** �� ��:       s:      ��У���ַ���
** �� ��:       TRUE:   ����Ч����ֵ���ַ���
**              FALSE:  ������Ч����ֵ���ַ���
** ��������:    У���ַ����Ƿ�����Ч����ֵ���ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.5.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      IsDate
** �� ��:       s:       ��У���ַ���
**              format:  ���ڸ�ʽ
** �� ��:       TRUE:    ����Ч���������ַ���
**              FALSE:   ������Ч���������ַ���
** ��������:    У���ַ����Ƿ�����Ч���������ַ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.5.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsDate(const char *s, const char *format) {
    return IsValidDate(s, format);
}
