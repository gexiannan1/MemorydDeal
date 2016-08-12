/******************************************************************
** 文件名:      log.h
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 描 述:       日志处理程序的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log.h,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#ifndef _LOG_H
#define _LOG_H


#include    "log_level.h"
#include    "log_mode.h"
#include    "../symbol.h"



/*
 * 日志编译标志定义
 */
#define     _VA_MACRO_ABLE              1           /* 是否支持变参宏 */
#define     _LOG_ABLE                   1           /* 日志开启标志 */
#define     _LOG_DEBUG_ABLE             1           /* 是否允许登记级别为DEBUG的日志信息 */
#define     _LOG_TRACE_ABLE             1           /* 是否允许登记级别为TRACE的日志信息 */
/* -------------------------           */



/*
 * 常量定义
 */
#define     MAX_LOG_APPENDER_NUMBER     10          /* 最大日志数量 */
#define     MAX_LOG_INFO_SIZE           8192        /* 日志内容的最大长度 */
/* -------------------------           */


/*
 * 日志默认配置值定义
 */
#define     LOG_DEFAULT_MIN_LOG_LEVEL_STR           "trace"
#define     LOG_DEFAULT_MAX_LOG_LEVEL_STR           "fatal"
#define     LOG_DEFAULT_MAX_FILE_LENGTH_STR         "10"
#define     LOG_DEFAULT_MAX_BACKUP_INDEX_STR        "10"

#define     LOG_DEFAULT_MIN_LOG_LEVEL               LOG_TRACE
#define     LOG_DEFAULT_MAX_LOG_LEVEL               LOG_FATAL
#define     LOG_DEFAULT_MAX_FILE_LENGTH             10
#define     LOG_DEFAULT_MAX_BACKUP_INDEX            10
/* -------------------------           */



/*
 * 结构体定义
 */

/*
 * 日志配置信息结构
 */
typedef struct {
    char    logMode[32];                    /* 日志模式 */
    char    minLogLevel[32];                /* 日志登记的起始级别 */
    char    maxLogLevel[32];                /* 日志登记的最高级别 */
    char    logFile[256];                   /* 日志文件名称 */
    int     maxFileLength;                  /* 日志文件最大长度 */
    int     maxBackupIndex;                 /* 日志文件最大备份数 */
} LOG_CONFIG;


/*
 * 日志处理器结构
 */
typedef struct log_appender{
    LOG_MODE    *logMode;                   /* 日志模式 */
    LOG_LEVEL   *minLogLevel;               /* 日志登记的起始级别 */
    LOG_LEVEL   *maxLogLevel;               /* 日志登记的最高级别 */
    char        logFile[256];               /* 日志文件名称 */
    int         maxFileLength;              /* 日志文件最大长度 */
    int         maxBackupIndex;             /* 日志文件最大备份数 */
} LOG_APPENDER;
/* -------------------------           */



/*
 * 函数声明
 */

BOOL    InitLog(int configNumber, LOG_CONFIG configs[]);            /* 日志系统初始化函数 */
BOOL    InitLogWithSystemName(char *systemName, int configNumber,
        LOG_CONFIG configs[]);                                      /* 日志系统初始化函数 */

BOOL    RebuildLogConfig();                                         /* 重新解析、应用日志配置 */
BOOL    RebuildLogConfigWithSystemName(char *systemName);           /* 重新解析、应用日志配置 */

BOOL            IsLogInitialized();                                 /* 返回日志系统是否已初始化 */

char*           GetLogSystemName();                                 /* 返回日志系统名称*/
void            SetLogSystemName(char*);                            /* 设置日志系统名称*/

LOG_LEVEL*      GetAllowableMinLogLevel();                          /* 返回允许的起始日志登记级别 */
void            SetAllowableMinLogLevel(LOG_LEVEL*);                /* 设置允许的起始日志登记级别 */

LOG_APPENDER*   GetLogAppenderList();                               /* 返回日志处理器列表 */
LOG_APPENDER*   GetLogAppender(int);                                /* 返回指定位置的日志处理器 */
int             GetLogAppenderNumber();                             /* 返回日志处理器个数 */

int             GetOriginalLogConfigListLength();                   /* 返回原始日志配置列表长度 */
LOG_CONFIG*     GetOriginalLogConfig(int);                          /* 返回指定位置的原始日志配置 */

void            PrintLogConfigInfo();                               /* 打印日志配置信息 */
/* -------------------------           */



/*
 * 日志函数(宏函数)定义
 */
#if _VA_MACRO_ABLE

    #if _LOG_ABLE

        /*
         * LOG, 日志登记宏函数, 为简化日志函数的使用而设
         *
         * <pre>
         * LOG(level, siteName, module, "log info");
         * LOG(level, siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   对应于LogImpl函数, 自动填充了前两个参数: srcFile, srcLine
         * @see     LogImpl()
         */
        #define     LOG(...)        LogImpl(__FILE__, __LINE__, __VA_ARGS__)

        #if _LOG_DEBUG_ABLE

            #if _LOG_TRACE_ABLE

                /*
                 * Log, 简单的日志登记宏函数, 为兼容已有程序而设
                 * 日志级别默认采用: LOG_TRACE
                 * 站点名称默认采用: ""
                 * 模块名称默认采用: __FILE__
                 *
                 * <pre>
                 * Log("log info");
                 * Log("log info, a is:%s; b is: %d", a, b);
                 * </pre>
                 *
                 * @param   日志内容
                 * @see     LogImpl()
                 */
                #define     Log(...)        LogImpl(__FILE__, __LINE__, LOG_TRACE, __FILE__, __VA_ARGS__)

                /*
                 * TRACE, Trace信息登记宏函数
                 * 以级别LOG_TRACE登记日志
                 *
                 * <pre>
                 * TRACE(siteName, module, "log info");
                 * TRACE(siteName, module, "log info, a is:%s; b is: %d", a, b);
                 * </pre>
                 *
                 * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
                 * @see     LogImpl()
                 */
                #define     TRACE(...)      LogImpl(__FILE__, __LINE__, LOG_TRACE, __VA_ARGS__)

            #else

               #define     Log(...)
              #define     TRACE(...)

       #endif

            /*
             * DEBUG, Debug信息登记宏函数
             * 以级别LOG_DEBUG登记日志
             *
             * <pre>
             * DEBUG(siteName, module, "log info");
             * DEBUG(siteName, module, "log info, a is:%s; b is: %d", a, b);
             * </pre>
             *
             * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
             * @see     LogImpl()
             */
            #define     DEBUG(...)      LogImpl(__FILE__, __LINE__, LOG_DEBUG, __VA_ARGS__)

       #else

          #define     Log(...)
          #define     TRACE(...)
           #define     DEBUG(...)

        #endif

        /*
         * INFO, Info信息登记宏函数
         * 以级别LOG_INFO登记日志
         *
         * <pre>
         * INFO(siteName, module, "log info");
         * INFO(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     INFO(...)       LogImpl(__FILE__, __LINE__, LOG_INFO, __VA_ARGS__)

        /*
         * WARN, Warn信息登记宏函数
         * 以级别LOG_WARN登记日志
         *
         * <pre>
         * WARN(siteName, module, "log info");
         * WARN(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     WARN(...)       LogImpl(__FILE__, __LINE__, LOG_WARN, __VA_ARGS__)

        /*
         * ERROR, Error信息登记宏函数
         * 以级别LOG_ERROR登记日志
         *
         * <pre>
         * ERROR(siteName, module, "log info");
         * ERROR(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     ERROR(...)      LogImpl(__FILE__, __LINE__, LOG_ERROR, __VA_ARGS__)

        /*
         * FATAL, Fatal信息登记宏函数
         * 以级别LOG_FATAL登记日志
         *
         * <pre>
         * FATAL(siteName, module, "log info");
         * FATAL(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   对应于LogImpl函数, 自动填充了前三个参数: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     FATAL(...)      LogImpl(__FILE__, __LINE__, LOG_FATAL, __VA_ARGS__)


    #else

        #define     LOG(...)
        #define     Log(...)
        #define     TRACE(...)
        #define     DEBUG(...)
        #define     INFO(...)
        #define     WARN(...)
        #define     ERROR(...)
        #define     FATAL(...)

    #endif

#else

    #if _LOG_ABLE

        #define     LOG             LogFullSimpleness

        #if _LOG_DEBUG_ABLE

            #if _LOG_TRACE_ABLE

                #define     Log         LogSimpleness
                #define     TRACE       TraceSimpleness

            #else

                #define     Log
                #define     TRACE

            #endif

            #define     DEBUG       DebugSimpleness

        #else

            #define     Log
            #define     TRACE
            #define     DEBUG

        #endif

        #define     INFO            InfoSimpleness
        #define     WARN            WarnSimpleness
        #define     ERROR           ErrorSimpleness
        #define     FATAL           FatalSimpleness
#define     TRACE           TraceSimpleness

    #else

        #define     LOG
        #define     Log
        #define     TRACE
        #define     DEBUG
        #define     INFO
        #define     WARN
        #define     ERROR
        #define     FATAL

    #endif

#endif
/* -------------------------           */



/*
 * 日志登记处理实现声明
 */
void    LogImpl(const char *srcFile, int srcLine, LOG_LEVEL *level,
        const char *module, const char *fmt, ...);
/* -------------------------           */



/*
 * 日志登记处理实现声明 (为不支持变参宏的系统而设)
 */
void    LogSimpleness(const char *fmt, ...);
void    LogFullSimpleness(LOG_LEVEL *level, const char *module, const char *fmt, ...);
void    TraceSimpleness(const char *module, const char *fmt, ...);
void    DebugSimpleness(const char *module, const char *fmt, ...);
void    InfoSimpleness(const char *module, const char *fmt, ...);
void    WarnSimpleness(const char *module, const char *fmt, ...);
void    ErrorSimpleness(const char *module, const char *fmt, ...);
void    FatalSimpleness(const char *module, const char *fmt, ...);
/* -------------------------           */


#endif  /* _LOG_H */
