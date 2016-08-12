/******************************************************************
** �ļ���:      common_lib.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.13
** �� ��:       ����������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * �궨��
 */

#define     Min(a,b)    ((a) < (b) ? (a) : (b))
#define     Max(a,b)    ((a) > (b) ? (a) : (b))

#define     Floor(f)    ((long)(f))
#define     Ceil(f)     ((f) - (long)(f) > 0.000001 ? (long)(f) + 1 : (long)(f))

/* �������鳤�� */
#define     Dim(x)                          (sizeof(x) / sizeof(x[0]))

/* ��xС��0�򷵻�0 */
#define     TRIM_ZERO(x)                    ((x) < 0 ? 0 : (x))

/* ��xС�ڵ���0�򷵻�v */
#define     SWITCH_SMALLNESS_NUMBER(x,v)    ((x) <= 0 ? v : (x))

/* ���ַ���sΪ���򷵻�v */
#define     SWITCH_EMPTY_STR(s,v)           ((s) ? (strlen(RtrimRude((s))) > 0 ? (s) : (v)) : (v))


/*
 * ����ԭ��
 */


#endif  /* _COMMON_LIB_H */
