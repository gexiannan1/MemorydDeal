/******************************************************************
** �ļ���:      common_time.h
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2004.4.26
** �� ��:       ����/ʱ�亯����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.04.15   Wei Huanyin 1�������ж��Ƿ����ꡢ������������������������������
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/time/common_time.h,v 1.1 2012/03/29 01:20:20 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:20 $
 */

#ifndef _COMMON_TIME_H
#define _COMMON_TIME_H


#include    <time.h>
#include    "../symbol.h"


/*
 * ����/ʱ���ʽ����
 */
#define     TIME_FORMAT_DATE                    "%Y%m%d"
#define     TIME_FORMAT_FORMATTED_DATE          "%Y-%m-%d"
#define     TIME_FORMAT_SHORT_DATE              "%y%m%d"
#define     TIME_FORMAT_FORMATTED_SHORT_DATE    "%y-%m-%d"
#define     TIME_FORMAT_TIME                    "%H%M%S"
#define     TIME_FORMAT_FORMATTED_TIME          "%H:%M:%S"
#define     TIME_FORMAT_TIMESTAMP               "%Y%m%d%H%M%S"
#define     TIME_FORMAT_FORMATTED_TIMESTAMP     "%Y-%m-%d %H:%M:%S"


/*
 * ��������
 */

BOOL            IsValidDate(const char*, const char*);                      /* У���ַ����Ƿ�����Ч���������ַ��� */


char*           FormatTime(const struct tm*, const char*);                  /* ��ʽ������/ʱ��ֵ */
struct tm*      ParseTime(const char *, const char *);                      /* ���ݸ����ĸ�ʽ����ʱ���ַ��� */


struct tm*      GetCurrentTime();                                           /* ���ص�ǰʱ�� */

int             GetYear(void);                                              /* ������� */
int             GetMonth(void);                                             /* �����·� */
int             GetDay(void);                                               /* ��������(���µڼ���) */
int             GetHour(void);                                              /* ����Сʱ�� */
int             GetMinute(void);                                            /* ���ط����� */
int             GetSecond(void);                                            /* �������� */


struct timeval* GetTimeOfDay();                                             /* ���ؾ�ȷ�����ʱ�� */
float           DiffMillisecond(struct timeval*, struct timeval*);          /* ������������ */
long long       DiffMicrosecond(struct timeval*, struct timeval*);          /* �������΢���� */


char*           GetDate(void);                                              /* ��������"yyyymmdd"����λ��ݵİ�λ�����ַ��� */
int             GetIntDate();                                               /* ���ص�ǰ���ڵ�������ʾ(yyyymmdd) */
char*           GetFormattedDate(void);                                     /* ��������"yyyy-mm-dd"����λ��ݵ�ʮλ�����ַ��� */

char*           GetShortDate(void);                                         /* ��������"yymmdd"����λ��ݵ���λ�����ַ��� */
char*           GetFormattedShortDate(void);                                /* ��������"yy-mm-dd"����λ��ݵİ�λ�����ַ��� */


char*           GetTime(void);                                              /* ��������"hhmmss"����λʱ���ַ��� */
char*           GetFormattedTime(void);                                     /* ��������"hh:mm:ss"�İ�λʱ���ַ��� */

char*           GetTimestamp(void);                                         /* ��������"yyyymmddhhmmss"��ʮ��λʱ����ַ��� */
char*           GetFormattedTimestamp(void);                                /* ��������"yyyy-mm-dd hh:mm:ss"��ʮ��λʱ����ַ��� */


long            DiffSecondInsideDay(const struct tm *, const struct tm *);  /* ��������ʱ��ֵ������������ͬһ���ڵ�����ʱ��ֵ, ���������ڲ */
float           DiffMinuteInsideDay(const struct tm *, const struct tm *);  /* ��������ʱ��ֵ�����ķ�������ͬһ���ڵ�����ʱ��ֵ, ���������ڲ */
float           DiffHourInsideDay(const struct tm *, const struct tm *);    /* ��������ʱ��ֵ������Сʱ����ͬһ���ڵ�����ʱ��ֵ, ���������ڲ */
float           DiffYearIntuit(const struct tm *, const struct tm *);       /* ��������ʱ��ֵ���������� */


struct tm*      AddSecond(const struct tm*, long);                          /* ��������ָ���������ʱ��ֵ (ͬһ����, �������ڵı��) */
struct tm*      AddMinute(const struct tm*, long);                          /* ��������ָ�����������ʱ��ֵ (ͬһ����, �������ڵı��) */
struct tm*      AddHour(const struct tm*, long);                            /* ��������ָ��Сʱ�����ʱ��ֵ (ͬһ����, �������ڵı��) */


int             ConvertTimeToInt(const struct tm*);                         /* ת������Ϊhhmmss��ʽ��int��ʱ��ֵ */
int             ConvertDateToInt(const struct tm*);                         /* ת������Ϊyymmdd��ʽ��int������ֵ */

int             GetIntTime();                                               /* ���ص�ǰʱ���������ʾ(hhmmss) */
int             GetSecondsFromInt(int);                                     /* ��������ʱ���ڱ��յľ������� */
int             GetIntTimeFromSeconds(int);                                 /* ���ر��վ���������Ӧ������ʱ�� */

int             DiffSecondInt(int, int);                                    /* ����������������ʾ��ʱ��ֵ������������ͬһ���ڵ�����ʱ��ֵ�� */
int				GetYears(int beginDate,int endDate);						/* ����������������ʾ�����ڼ����������� */
int				IsLeapYear(int iYear);										/* �ж��Ƿ�������*/
int				CalcDateDiff(int,int);										/* ����������������ʾ�������������� */
int				GetIntYear(int Date);										/* ���ݸ�����������ʾ�����ڷֽ���� */
int				GetIntMonth(int Date);										/* ���ݸ�����������ʾ�����ڷֽ���� */
int				GetIntDay(int Date);										/* ���ݸ�����������ʾ�����ڷֽ���� */
int				GetIntMonthDay(int Date);									/* ���ݸ�����������ʾ�����ڷֽ������ */
int				GetIntUniteYear(int year, int month, int day);				/* ���ݸ�����������ʾ���ꡢ�¡��պϲ����������� */



#endif  /* _COMMON_TIME_H */
