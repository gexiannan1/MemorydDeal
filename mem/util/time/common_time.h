/******************************************************************
** 文件名:      common_time.h
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 描 述:       日期/时间函数的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.04.15   Wei Huanyin 1、增加判断是否闰年、计算日期相差天数、相差整年数函数
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
 * 日期/时间格式定义
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
 * 函数声明
 */

BOOL            IsValidDate(const char*, const char*);                      /* 校验字符串是否是有效的日期型字符串 */


char*           FormatTime(const struct tm*, const char*);                  /* 格式化日期/时间值 */
struct tm*      ParseTime(const char *, const char *);                      /* 根据给定的格式解析时间字符串 */


struct tm*      GetCurrentTime();                                           /* 返回当前时间 */

int             GetYear(void);                                              /* 返回年份 */
int             GetMonth(void);                                             /* 返回月份 */
int             GetDay(void);                                               /* 返回日期(本月第几日) */
int             GetHour(void);                                              /* 返回小时数 */
int             GetMinute(void);                                            /* 返回分钟数 */
int             GetSecond(void);                                            /* 返回秒数 */


struct timeval* GetTimeOfDay();                                             /* 返回精确的相对时间 */
float           DiffMillisecond(struct timeval*, struct timeval*);          /* 返回相差毫秒数 */
long long       DiffMicrosecond(struct timeval*, struct timeval*);          /* 返回相差微秒数 */


char*           GetDate(void);                                              /* 返回形如"yyyymmdd"的四位年份的八位日期字符串 */
int             GetIntDate();                                               /* 返回当前日期的整数表示(yyyymmdd) */
char*           GetFormattedDate(void);                                     /* 返回形如"yyyy-mm-dd"的四位年份的十位日期字符串 */

char*           GetShortDate(void);                                         /* 返回形如"yymmdd"的两位年份的六位日期字符串 */
char*           GetFormattedShortDate(void);                                /* 返回形如"yy-mm-dd"的两位年份的八位日期字符串 */


char*           GetTime(void);                                              /* 返回形如"hhmmss"的六位时间字符串 */
char*           GetFormattedTime(void);                                     /* 返回形如"hh:mm:ss"的八位时间字符串 */

char*           GetTimestamp(void);                                         /* 返回形如"yyyymmddhhmmss"的十四位时间戳字符串 */
char*           GetFormattedTimestamp(void);                                /* 返回形如"yyyy-mm-dd hh:mm:ss"的十九位时间戳字符串 */


long            DiffSecondInsideDay(const struct tm *, const struct tm *);  /* 计算两个时间值间相差的秒数（同一天内的两个时间值, 不计算日期差） */
float           DiffMinuteInsideDay(const struct tm *, const struct tm *);  /* 计算两个时间值间相差的分钟数（同一天内的两个时间值, 不计算日期差） */
float           DiffHourInsideDay(const struct tm *, const struct tm *);    /* 计算两个时间值间相差的小时数（同一天内的两个时间值, 不计算日期差） */
float           DiffYearIntuit(const struct tm *, const struct tm *);       /* 计算两个时间值间相差的年数 */


struct tm*      AddSecond(const struct tm*, long);                          /* 计算增加指定秒数后的时间值 (同一天内, 忽略日期的变更) */
struct tm*      AddMinute(const struct tm*, long);                          /* 计算增加指定分钟数后的时间值 (同一天内, 忽略日期的变更) */
struct tm*      AddHour(const struct tm*, long);                            /* 计算增加指定小时数后的时间值 (同一天内, 忽略日期的变更) */


int             ConvertTimeToInt(const struct tm*);                         /* 转换日期为hhmmss格式的int型时间值 */
int             ConvertDateToInt(const struct tm*);                         /* 转换日期为yymmdd格式的int型日期值 */

int             GetIntTime();                                               /* 返回当前时间的整数表示(hhmmss) */
int             GetSecondsFromInt(int);                                     /* 返回整数时间在本日的绝对秒数 */
int             GetIntTimeFromSeconds(int);                                 /* 返回本日绝对秒数对应的整数时间 */

int             DiffSecondInt(int, int);                                    /* 计算两个用整数表示的时间值间相差的秒数（同一天内的两个时间值） */
int				GetYears(int beginDate,int endDate);						/* 计算两个用整数表示的日期间相差的整年数 */
int				IsLeapYear(int iYear);										/* 判断是否是闰年*/
int				CalcDateDiff(int,int);										/* 计算两个用整数表示的日期相差的天数 */
int				GetIntYear(int Date);										/* 根据给定用整数表示的日期分解出年 */
int				GetIntMonth(int Date);										/* 根据给定用整数表示的日期分解出月 */
int				GetIntDay(int Date);										/* 根据给定用整数表示的日期分解出日 */
int				GetIntMonthDay(int Date);									/* 根据给定用整数表示的日期分解出月日 */
int				GetIntUniteYear(int year, int month, int day);				/* 根据给定用整数表示的年、月、日合并成整数日期 */



#endif  /* _COMMON_TIME_H */
