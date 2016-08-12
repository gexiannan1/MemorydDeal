/******************************************************************
** 文件名:      common_time.c
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
 * $Header: /RepLevel2/v3/src/qe/src/util/time/common_time.c,v 1.1 2012/03/29 01:20:20 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:20 $
 */
#if defined __GNUC__ && ! defined _XOPEN_SOURCE
	#define    _XOPEN_SOURCE           /* for strptime on glibc2 */
#endif

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <strings.h>
#include    <time.h>
#include    <sys/time.h>
#include    "common_time.h"
#include    "../symbol.h"
#include    "../lib/common_lib.h"
#include    "../string/common_str.h"


/*****************************************************************
** 函数名:      IsValidDate
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
IsValidDate(const char *s, const char *format) {
    struct tm   tm;
    char        buf[128];
    char        *ptr;

    if (time == NULL) {
        return 0;
    }

    strncpy(buf, s, sizeof(buf));
    ptr = (char*) TrimRude(buf);

    if (strlen(ptr) < 1) {
        return 0;
    }

    ptr = (char*) strftime(buf, sizeof(buf), format, &tm);
    return ptr != NULL ? 1 : 0;
}


/*****************************************************************
** 函数名:      FormatTime
** 输 入:       tm1:     tm 结构
**              format:  日期格式
** 输 出:       指定格式的日期/时间字符串
** 功能描述:    格式化日期/时间值
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
FormatTime(const struct tm *tm1, const char *format) {
    static int      i = 0;
    static char     buf[_STATIC_BUFFER_ARRAY_SIZE][50];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    strftime(buf[i], sizeof(buf[i]), format, tm1);

    return buf[i];
}


/*****************************************************************
** 函数名:      ParseTime
** 输 入:       time:    日期/时间字符串
**              format:  日期格式
** 输 出:       tm 结构
** 功能描述:    根据给定的格式解析时间字符串
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm*
ParseTime(const char *time, const char *format) {
    static int  i = 0;
    static struct tm    tm[_STATIC_BUFFER_ARRAY_SIZE];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    bzero(&tm[i], sizeof(struct tm));

    strptime(time, format, &tm[i]);
    return &tm[i];
}


/*****************************************************************
** 函数名:      GetCurrentTime
** 输 入:       无
** 输 出:       tm 结构
** 功能描述:    返回当前时间
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm*
GetCurrentTime() {
    time_t              t = 0;
    static int          i = 0;
    static struct tm    tm[_STATIC_BUFFER_ARRAY_SIZE];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    bzero(&tm[i], sizeof(struct tm));

    t = time(NULL);
    memcpy(&tm[i], localtime(&t), sizeof(struct tm));

    return &tm[i];
}


/*****************************************************************
** 函数名:      GetDate
** 输 入:       无
** 输 出:       年份
** 功能描述:    返回年份
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetYear() {
    return GetCurrentTime()->tm_year + 1900;
}


/*****************************************************************
** 函数名:      GetMonth
** 输 入:       无
** 输 出:       月份
** 功能描述:    返回月份
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetMonth() {
    return GetCurrentTime()->tm_mon + 1;
}


/*****************************************************************
** 函数名:      GetDay
** 输 入:       无
** 输 出:       本月第几日
** 功能描述:    返回日期(本月第几日)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetDay() {
    return GetCurrentTime()->tm_mday;
}


/*****************************************************************
** 函数名:      GetHour
** 输 入:       无
** 输 出:       小时数
** 功能描述:    返回小时数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetHour() {
    return GetCurrentTime()->tm_hour;
}


/*****************************************************************
** 函数名:      GetMinute
** 输 入:       无
** 输 出:       分钟数
** 功能描述:    返回分钟数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetMinute() {
    return GetCurrentTime()->tm_min;
}


/*****************************************************************
** 函数名:      GetSecond
** 输 入:       无
** 输 出:       秒数
** 功能描述:    返回秒数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetSecond() {
    return GetCurrentTime()->tm_sec;
}


/*****************************************************************
** 函数名:      GetTimeOfDay
** 输 入:       无
** 输 出:       精确时间(struct timeval*)
** 功能描述:    返回精确的相对时间
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct timeval*
GetTimeOfDay() {
    static int              i = 0;
    static struct timeval   tv[_STATIC_BUFFER_ARRAY_SIZE];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    bzero(&tv[i], sizeof(struct timeval));
    gettimeofday(&tv[i], NULL);

    return &tv[i];
}


/*****************************************************************
** 函数名:      DiffMillisecond
** 输 入:       无
** 输 出:       相差毫秒数
** 功能描述:    返回相差毫秒数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.12.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
float
DiffMillisecond(struct timeval *tv1, struct timeval *tv2) {
    float   diff = 0;

    diff = (tv2->tv_sec - tv1->tv_sec) * 1000;
    diff += (float) (tv2->tv_usec - tv1->tv_usec) / 1000;

    return diff;
}


/*****************************************************************
** 函数名:      DiffMicrosecond
** 输 入:       无
** 输 出:       相差微秒数
** 功能描述:    返回相差微秒数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.10.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
long long
DiffMicrosecond(struct timeval *tv1, struct timeval *tv2) {
    long long   diff = 0;

    diff = (long long) (tv2->tv_sec - tv1->tv_sec) * 1000000;
    diff += tv2->tv_usec - tv1->tv_usec;

    return diff;
}


/*****************************************************************
** 函数名:      GetDate
** 输 入:       无
** 输 出:       形如"yyyymmdd"的八位字符串
** 功能描述:    返回当前日期, 以形如"yyyymmdd"的四位年份的八位日期字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetDate() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_DATE);
}


/*****************************************************************
** 函数名:      GetIntDate
** 输 入:       无
** 输 出:       int 当前日期，格式为yymmdd
** 功能描述:    返回当前日期的整数表示(yymmdd)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntDate() {
    return ConvertDateToInt(GetCurrentTime());
}


/*****************************************************************
** 函数名:      GetFormattedDate
** 输 入:       无
** 输 出:       形如"yyyy-mm-dd"的十位日期字符串
** 功能描述:    返回当前日期, 以形如"yyyy-mm-dd"的四位年份的十位日期字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetFormattedDate() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_FORMATTED_DATE);
}


/*****************************************************************
** 函数名:      GetShortDate
** 输 入:       无
** 输 出:       形如"yymmdd"的六位日期字符串
** 功能描述:    返回当前日期, 以形如"yymmdd"的两位年份的六位日期字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetShortDate() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_SHORT_DATE);
}


/*****************************************************************
** 函数名:      GetFormattedShortDate
** 输 入:       无
** 输 出:       形如"yy-mm-dd"的八位日期字符串
** 功能描述:    返回当前日期, 以形如"yy-mm-dd"的两位年份的八位日期字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetFormattedShortDate() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_FORMATTED_SHORT_DATE);
}


/*****************************************************************
** 函数名:      GetTime
** 输 入:       无
** 输 出:       形如"hhmmss"的六位时间字符串
** 功能描述:    返回当前时间, 以形如"hhmmss"的六位时间字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetTime() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_TIME);
}


/*****************************************************************
** 函数名:      GetFormattedTime
** 输 入:       无
** 输 出:       形如"hh:mm:ss"的八位时间字符串
** 功能描述:    返回当前时间, 以形如"hh:mm:ss"的八位时间字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetFormattedTime() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_FORMATTED_TIME);
}


/*****************************************************************
** 函数名:      GetTimestamp
** 输 入:       无
** 输 出:       形如"yyyymmddhhmmss"的十四位时间戳字符串
** 功能描述:    返回当前时间, 以形如"yyyymmddhhmmss"的十四位时间戳字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetTimestamp() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_TIMESTAMP);
}


/*****************************************************************
** 函数名:      GetFormattedTimestamp
** 输 入:       无
** 输 出:       形如"yyyy-mm-dd hh:mm:ss"的十九位时间戳字符串
** 功能描述:    返回当前时间, 以形如"yyyy-mm-dd hh:mm:ss"的十九位时间戳字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetFormattedTimestamp() {
    return FormatTime(GetCurrentTime(), TIME_FORMAT_FORMATTED_TIMESTAMP);
}


/*****************************************************************
** 函数名:      DiffSecondInsideDay
** 输 入:       tm1: tm结构;  tm2: tm结构
** 输 出:       相差秒数
**                  -  >0: tm1 > tm2
**                  -  <0: tm1 < tm2
** 功能描述:    计算两个时间值间相差的秒数（同一天内的两个时间值, 不计算日期差）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
long
DiffSecondInsideDay(const struct tm *tm1, const struct tm *tm2) {
    long result = 0;

    result = (tm1->tm_hour - tm2->tm_hour) * 60 * 60;
    result += (tm1->tm_min - tm2->tm_min) * 60;
    result += tm1->tm_sec - tm2->tm_sec;

    return result;
}


/*****************************************************************
** 函数名:      DiffMinuteInsideDay
** 输 入:       tm1: tm结构;  tm2: tm结构
** 输 出:       相差分钟数
**                  -  >0: tm1 > tm2
**                  -  <0: tm1 < tm2
** 功能描述:    计算两个时间值间相差的分钟数（同一天内的两个时间值, 不计算日期差）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
float
DiffMinuteInsideDay(const struct tm *tm1, const struct tm *tm2) {
    return DiffSecondInsideDay(tm1, tm2) / 60;
}


/*****************************************************************
** 函数名:      DiffMinuteInsideDay
** 输 入:       tm1: tm结构;  tm2: tm结构
** 输 出:       相差小时数
**                  -  >0: tm1 > tm2
**                  -  <0: tm1 < tm2
** 功能描述:    计算两个时间值间相差的小时数（同一天内的两个时间值, 不计算日期差）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
float
DiffHourInsideDay(const struct tm *tm1, const struct tm *tm2) {
    return DiffSecondInsideDay(tm1, tm2) / (60 * 60);
}


/*****************************************************************
** 函数名:      DiffYearIntuit
** 输 入:       tm1: tm结构;  tm2: tm结构
** 输 出:       相差的年数
**                  -  >0: tm1 > tm2
**                  -  <0: tm1 < tm2
** 功能描述:    计算两个时间值间相差的年数, 以直观的年、月、日和时钟进行计算.
**              仅以粗略计算满足日期比较的需要, 小数位部分的值并不精确
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
float
DiffYearIntuit(const struct tm *tm1, const struct tm *tm2) {
    long    intuitDiffHour = 0;

    intuitDiffHour = (tm1->tm_year - tm2->tm_year) * 12 * 30 * 24;
    intuitDiffHour += (tm1->tm_mon - tm2->tm_mon) * 30 * 24;
    intuitDiffHour += (tm1->tm_mday - tm2->tm_mday) * 24;
    intuitDiffHour += tm1->tm_hour - tm2->tm_hour;

    return (float) intuitDiffHour / (12 * 30 * 24);
}


/*****************************************************************
** 函数名:      AddSecond
** 输 入:       tm1:     tm 结构, 基准日期
**              second:  增加秒数
** 输 出:       tm 结构, 增加后的日期
** 功能描述:    计算增加指定秒数后的时间值 (同一天内, 忽略日期的变更)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm *
AddSecond(const struct tm *tm1, long second) {
    static int          i = 0;
    static struct tm    tm[_STATIC_BUFFER_ARRAY_SIZE];

    int     hour = 0;
    int     min = 0;

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    memcpy(&tm[i], tm1, sizeof(struct tm));

    second += tm[i].tm_sec;
    min += tm[i].tm_min;
    hour += tm[i].tm_hour;

    if (second >= 60 * 60) {
        hour += Floor(second / (60 * 60));
        second %= 60 * 60;
    }

    if (second >= 60) {
        min += Floor(second / 60);
        second %= 60;
    }

    if (min >= 60) {
        hour += Floor(min / 60);
        min %= 60;
    }

    hour %= 24;

    tm[i].tm_hour = hour;
    tm[i].tm_min = min;
    tm[i].tm_sec = second;

    return &tm[i];
}


/*****************************************************************
** 函数名:      AddMinute
** 输 入:       tm1:     tm 结构, 基准日期
**              minute:  增加的分钟数
** 输 出:       tm 结构, 增加后的日期
** 功能描述:    计算增加指定分钟数后的时间值 (同一天内, 忽略日期的变更)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm *
AddMinute(const struct tm *tm1, long minute) {
    static int          i = 0;
    static struct tm    tm[_STATIC_BUFFER_ARRAY_SIZE];

    int     hour = 0;

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    memcpy(&tm[i], tm1, sizeof(struct tm));

    minute += tm[i].tm_min;
    hour += tm[i].tm_hour;

    if (minute >= 60) {
        hour += Floor(minute / 60);
        minute %= 60;
    }

    hour %= 24;

    tm[i].tm_hour = hour;
    tm[i].tm_min = minute;

    return &tm[i];
}


/*****************************************************************
** 函数名:      AddHour
** 输 入:       tm1:     tm 结构, 基准日期
**              hour:    增加的小时数
** 输 出:       tm 结构, 增加后的日期
** 功能描述:    计算增加指定小时数后的时间值 (同一天内, 忽略日期的变更)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm *
AddHour(const struct tm *tm1, long hour) {
    static int          i = 0;
    static struct tm    tm[_STATIC_BUFFER_ARRAY_SIZE];

    if (i >= _STATIC_BUFFER_ARRAY_SIZE - 1) {
        i = 0;
    } else {
        i ++;
    }

    memcpy(&tm[i], tm1, sizeof(struct tm));

    tm[i].tm_hour += hour;
    tm[i].tm_hour %= 24;

    return &tm[i];
}


/*****************************************************************
** 函数名:      ConvertTimeToInt
** 输 入:       tm1:     tm 结构
** 输 出:       hhmmss格式的int型时间值
** 功能描述:    转换日期为hhmmss格式的int型时间值
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
ConvertTimeToInt(const struct tm *tm1) {
    return tm1->tm_hour * 10000 + tm1->tm_min * 100 + tm1->tm_sec;
}


/*****************************************************************
** 函数名:      ConvertDateToInt
** 输 入:       tm1:     tm 结构
** 输 出:       yyyymmdd格式的int型日期值
** 功能描述:    转换日期为yyyymmdd格式的int型日期值
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
ConvertDateToInt(const struct tm *tm1) {
    return (tm1->tm_year + 1900) * 10000 + (tm1->tm_mon + 1) * 100 + tm1->tm_mday;
}


/*****************************************************************
** 函数名:      GetIntTime
** 输 入:       无
** 输 出:       int 当前时间，格式为hhmmss
** 功能描述:    返回当前时间的整数表示(hhmmss)
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.9.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntTime() {
    return ConvertTimeToInt(GetCurrentTime());
}


/*****************************************************************
** 函数名:      GetSecondsFromInt
** 输 入:       time: Int型时间值
** 输 出:       整数时间在本日的绝对秒数
** 功能描述:    返回整数时间在本日的绝对秒数
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetSecondsFromInt(int time) {
    return (time / 10000) * 3600 + ((time % 10000) / 100) * 60 + time % 100;
}


/*****************************************************************
** 函数名:      GetIntTimeFromSeconds
** 输 入:       seconds: 秒数
** 输 出:       整数时间
** 功能描述:    返回本日绝对秒数对应的整数时间
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2006.3.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntTimeFromSeconds(int seconds) {
    return (seconds / 3600) * 10000 + ((seconds % 3600) / 60) * 100 + seconds % 60;
}


/*****************************************************************
** 函数名:      DiffSecondInt
** 输 入:       t1: 时间1
**              t2: 时间2
** 输 出:       (t1-t2)相差的秒数
** 功能描述:    计算两个用整数表示的时间值间相差的秒数（同一天内的两个时间值）
** 全局变量:    无
** 调用模块:    无
** 创建人:      Wei Huanyin
** 日 期:       2006.2.21
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
DiffSecondInt(int t1, int t2) {
    return GetSecondsFromInt(t2) - GetSecondsFromInt(t1);
}


/*****************************************************************
** 函数名:      GetYears
** 输 入:       整数类型日期(YYYYMMDD)
** 输 出:
** 功能描述:    计算整年数
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetYears(int beginDate,int endDate){
	if (endDate < beginDate )
		return 0;
	if (GetIntMonthDay(endDate) >= GetIntMonthDay(beginDate)){
		return GetIntYear(endDate) - GetIntYear(beginDate);
	} else {
		if (IsLeapYear(GetIntYear(beginDate))&& GetIntDay(beginDate)==29 && GetIntDay(endDate)==28)
			return GetIntYear(endDate) - GetIntYear(beginDate);
		else
			return GetIntYear(endDate) - GetIntYear(beginDate)-1;
	}
}


/*****************************************************************
** 函数名:      IsLeapYear
** 输 入:       整数类型年(YYYY)
** 输 出:		0:表示不是闰年；1:表示是闰年
** 功能描述:    判断是否闰年
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
IsLeapYear(int iYear){
	return (!(iYear%4) && (iYear%100)) || !(iYear%400);
}


/*****************************************************************
** 函数名:      CalcDateDiff
** 输 入:       整数类型开始日期(YYYYMMDD)，整数类型结束日期(YYYYMMDD)
** 输 出:       相差的天数
** 功能描述:    计算两个整数日期相差的天数
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
CalcDateDiff(int beginDate,int endDate){

   int iEndYear = endDate / 10000 ;
   int iEndMonth = (endDate - iEndYear*10000) / 100;
   int iEndDay =  endDate - iEndYear*10000 - iEndMonth*100;
   int iStartYear =  beginDate / 10000 ;
   int iStartMonth = (beginDate - iStartYear*10000) / 100;
   int iStartDay =  beginDate - iStartYear*10000 - iStartMonth*100;



   int monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334};

   /*计算两个年份1月1日之间相差的天数 */
   int iDiffDays =(iEndYear - iStartYear)*365;
   iDiffDays += (iEndYear-1)/4 - (iStartYear-1)/4;
   iDiffDays -= ((iEndYear-1)/100 - (iStartYear-1)/100);
   iDiffDays += (iEndYear-1)/400 - (iStartYear-1)/400;

   /*加上iEndYear年1月1日到iEndMonth月iEndDay日之间的天数 */
   iDiffDays += monthday[iEndMonth-1] +
      (IsLeapYear(iEndYear)&&iEndMonth>2? 1: 0);
   iDiffDays += iEndDay;

   /*减去iStartYear年1月1日到iStartMonth月iStartDay日之间的天数 */
   iDiffDays -= (monthday[iStartMonth-1] +(IsLeapYear(iStartYear)&&iStartMonth>2 ? 1: 0));
   iDiffDays -= iStartDay;
   return iDiffDays;
}


/*****************************************************************
** 函数名:      GetIntYear
** 输 入:       整数类型日期(YYYYMMDD)
** 输 出:       整数类型年
** 功能描述:    根据给定日期分解出年
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntYear(int Date){
	return Date / 10000;
}


/*****************************************************************
** 函数名:      GetIntMonth
** 输 入:       整数类型日期(YYYYMMDD)
** 输 出:       整数类型月
** 功能描述:    根据给定日期分解出月
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntMonth(int Date){
	return Date % 10000 / 100;
}


/*****************************************************************
** 函数名:      GetIntDay
** 输 入:       整数类型日期(YYYYMMDD)
** 输 出:       整数类型日期
** 功能描述:    根据给定日期分解出日
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntDay(int Date){
	return Date % 100;
}


/*****************************************************************
** 函数名:      GetIntDay
** 输 入:       整数类型日期(YYYYMMDD)
** 输 出:       整数类型日期
** 功能描述:    根据给定日期分解出日
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntMonthDay(int Date){
	return Date % 10000;
}


/*****************************************************************
** 函数名:      GetIntUniteYear
** 输 入:       整数类型日期(YYYMMDD)
** 输 出:       整数类型日期
** 功能描述:    根据给定年、月、日合并成整数日期
**
**
** 全局变量:
** 调用模块:    无
** 作 者:       Wei Huanyin
** 日 期:       2006.3.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetIntUniteYear(int year, int month, int day){
	return (year*10000) + (month)*100 + day;
}
