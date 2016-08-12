/******************************************************************
** �ļ���:      common_str.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.4.17
** �� ��:       �ַ�����������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ����ԭ��
 */

char*       Trim(const char*);                                                      /* ����ȥ���ո����ַ��� */
char*       Ltrim(const char*);                                                     /* ����ȥ����˿ո����ַ��� */
char*       Rtrim(const char*);                                                     /* ����ȥ���Ҷ˿ո����ַ��� */
char*       TrimRude(char*);                                                        /* ȥ���ַ���ǰ��˵Ŀո��޸��ַ������� */
char*       RtrimRude(char*);                                                       /* ȥ���ַ����Ҷ˵Ŀո��޸��ַ������� */
char*       Replace(const char*, const char*, const char*);                         /* �滻Ŀ���ַ����еĴ��滻�ַ��� */
char*       ReplaceAppointed(const char*, const char*, const char*, const int);     /* �滻Ŀ���ַ����еĴ��滻�ַ�������ָ���滻������ */
char*       RReplace(const char*, const char*, const char*);                        /* �滻Ŀ���ַ����еĴ��滻�ַ��������Ҳ࿪ʼ�� */
char*       RReplaceAppointed(const char*, const char*, const char*, const int);    /* �滻Ŀ���ַ����еĴ��滻�ַ��������Ҳ࿪ʼ����ָ���滻������ */
char*       ReplaceCharAll(const char*, const char, const char, const int);         /* �滻�ַ����е������ַ� */

long long   Atoll(const char*);                                                     /* �ַ���תΪ��������ֵ */
long long	StringToll(char*, unsigned int);										/* �ַ���ת��Ϊ�����ͣ�ָ��С���������ƶ�pointλ*/
int			StringToInt(char*, unsigned int);										/* �ַ���ת��Ϊ���ͣ�ָ��С���������ƶ�pointλ*/

char*       Itoa(int);                                                              /* ������ֵתΪ�ַ��� */
char*       Ltoa(long);                                                             /* ��ֵתΪ�ַ��� */
char*       Lltoa(long long);                                                       /* ��������ֵתΪ�ַ��� */

int         Atox(const char*);                                                      /* ת��ʮ��������ʽ���ַ���Ϊ������ֵ */
char*       Xtoa(int);                                                              /* ������ֵתΪʮ��������ʽ���ַ��� */

BOOL        IsEmptyString(const char*);                                             /* �����ַ����Ƿ��ǿ��ַ��� */
BOOL        IsNumeric(const char*);                                                 /* У���ַ����Ƿ�����Ч����ֵ���ַ��� */
BOOL        IsDate(const char*, const char*);                                       /* У���ַ����Ƿ�����Ч���������ַ��� */
BOOL        IsStringEqual(const char*, const char*);                                /* �����ַ����Ƿ��ǿ��ַ��� */



#endif  /* _COMMON_STRING_H */
