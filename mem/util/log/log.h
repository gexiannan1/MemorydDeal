/******************************************************************
** �ļ���:      log.h
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2004.4.26
** �� ��:       ��־��������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��־�����־����
 */
#define     _VA_MACRO_ABLE              1           /* �Ƿ�֧�ֱ�κ� */
#define     _LOG_ABLE                   1           /* ��־������־ */
#define     _LOG_DEBUG_ABLE             1           /* �Ƿ�����ǼǼ���ΪDEBUG����־��Ϣ */
#define     _LOG_TRACE_ABLE             1           /* �Ƿ�����ǼǼ���ΪTRACE����־��Ϣ */
/* -------------------------           */



/*
 * ��������
 */
#define     MAX_LOG_APPENDER_NUMBER     10          /* �����־���� */
#define     MAX_LOG_INFO_SIZE           8192        /* ��־���ݵ���󳤶� */
/* -------------------------           */


/*
 * ��־Ĭ������ֵ����
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
 * �ṹ�嶨��
 */

/*
 * ��־������Ϣ�ṹ
 */
typedef struct {
    char    logMode[32];                    /* ��־ģʽ */
    char    minLogLevel[32];                /* ��־�Ǽǵ���ʼ���� */
    char    maxLogLevel[32];                /* ��־�Ǽǵ���߼��� */
    char    logFile[256];                   /* ��־�ļ����� */
    int     maxFileLength;                  /* ��־�ļ���󳤶� */
    int     maxBackupIndex;                 /* ��־�ļ���󱸷��� */
} LOG_CONFIG;


/*
 * ��־�������ṹ
 */
typedef struct log_appender{
    LOG_MODE    *logMode;                   /* ��־ģʽ */
    LOG_LEVEL   *minLogLevel;               /* ��־�Ǽǵ���ʼ���� */
    LOG_LEVEL   *maxLogLevel;               /* ��־�Ǽǵ���߼��� */
    char        logFile[256];               /* ��־�ļ����� */
    int         maxFileLength;              /* ��־�ļ���󳤶� */
    int         maxBackupIndex;             /* ��־�ļ���󱸷��� */
} LOG_APPENDER;
/* -------------------------           */



/*
 * ��������
 */

BOOL    InitLog(int configNumber, LOG_CONFIG configs[]);            /* ��־ϵͳ��ʼ������ */
BOOL    InitLogWithSystemName(char *systemName, int configNumber,
        LOG_CONFIG configs[]);                                      /* ��־ϵͳ��ʼ������ */

BOOL    RebuildLogConfig();                                         /* ���½�����Ӧ����־���� */
BOOL    RebuildLogConfigWithSystemName(char *systemName);           /* ���½�����Ӧ����־���� */

BOOL            IsLogInitialized();                                 /* ������־ϵͳ�Ƿ��ѳ�ʼ�� */

char*           GetLogSystemName();                                 /* ������־ϵͳ����*/
void            SetLogSystemName(char*);                            /* ������־ϵͳ����*/

LOG_LEVEL*      GetAllowableMinLogLevel();                          /* �����������ʼ��־�ǼǼ��� */
void            SetAllowableMinLogLevel(LOG_LEVEL*);                /* �����������ʼ��־�ǼǼ��� */

LOG_APPENDER*   GetLogAppenderList();                               /* ������־�������б� */
LOG_APPENDER*   GetLogAppender(int);                                /* ����ָ��λ�õ���־������ */
int             GetLogAppenderNumber();                             /* ������־���������� */

int             GetOriginalLogConfigListLength();                   /* ����ԭʼ��־�����б��� */
LOG_CONFIG*     GetOriginalLogConfig(int);                          /* ����ָ��λ�õ�ԭʼ��־���� */

void            PrintLogConfigInfo();                               /* ��ӡ��־������Ϣ */
/* -------------------------           */



/*
 * ��־����(�꺯��)����
 */
#if _VA_MACRO_ABLE

    #if _LOG_ABLE

        /*
         * LOG, ��־�ǼǺ꺯��, Ϊ����־������ʹ�ö���
         *
         * <pre>
         * LOG(level, siteName, module, "log info");
         * LOG(level, siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine
         * @see     LogImpl()
         */
        #define     LOG(...)        LogImpl(__FILE__, __LINE__, __VA_ARGS__)

        #if _LOG_DEBUG_ABLE

            #if _LOG_TRACE_ABLE

                /*
                 * Log, �򵥵���־�ǼǺ꺯��, Ϊ�������г������
                 * ��־����Ĭ�ϲ���: LOG_TRACE
                 * վ������Ĭ�ϲ���: ""
                 * ģ������Ĭ�ϲ���: __FILE__
                 *
                 * <pre>
                 * Log("log info");
                 * Log("log info, a is:%s; b is: %d", a, b);
                 * </pre>
                 *
                 * @param   ��־����
                 * @see     LogImpl()
                 */
                #define     Log(...)        LogImpl(__FILE__, __LINE__, LOG_TRACE, __FILE__, __VA_ARGS__)

                /*
                 * TRACE, Trace��Ϣ�ǼǺ꺯��
                 * �Լ���LOG_TRACE�Ǽ���־
                 *
                 * <pre>
                 * TRACE(siteName, module, "log info");
                 * TRACE(siteName, module, "log info, a is:%s; b is: %d", a, b);
                 * </pre>
                 *
                 * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
                 * @see     LogImpl()
                 */
                #define     TRACE(...)      LogImpl(__FILE__, __LINE__, LOG_TRACE, __VA_ARGS__)

            #else

               #define     Log(...)
              #define     TRACE(...)

       #endif

            /*
             * DEBUG, Debug��Ϣ�ǼǺ꺯��
             * �Լ���LOG_DEBUG�Ǽ���־
             *
             * <pre>
             * DEBUG(siteName, module, "log info");
             * DEBUG(siteName, module, "log info, a is:%s; b is: %d", a, b);
             * </pre>
             *
             * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
             * @see     LogImpl()
             */
            #define     DEBUG(...)      LogImpl(__FILE__, __LINE__, LOG_DEBUG, __VA_ARGS__)

       #else

          #define     Log(...)
          #define     TRACE(...)
           #define     DEBUG(...)

        #endif

        /*
         * INFO, Info��Ϣ�ǼǺ꺯��
         * �Լ���LOG_INFO�Ǽ���־
         *
         * <pre>
         * INFO(siteName, module, "log info");
         * INFO(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     INFO(...)       LogImpl(__FILE__, __LINE__, LOG_INFO, __VA_ARGS__)

        /*
         * WARN, Warn��Ϣ�ǼǺ꺯��
         * �Լ���LOG_WARN�Ǽ���־
         *
         * <pre>
         * WARN(siteName, module, "log info");
         * WARN(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     WARN(...)       LogImpl(__FILE__, __LINE__, LOG_WARN, __VA_ARGS__)

        /*
         * ERROR, Error��Ϣ�ǼǺ꺯��
         * �Լ���LOG_ERROR�Ǽ���־
         *
         * <pre>
         * ERROR(siteName, module, "log info");
         * ERROR(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
         * @see     LogImpl()
         */
        #define     ERROR(...)      LogImpl(__FILE__, __LINE__, LOG_ERROR, __VA_ARGS__)

        /*
         * FATAL, Fatal��Ϣ�ǼǺ꺯��
         * �Լ���LOG_FATAL�Ǽ���־
         *
         * <pre>
         * FATAL(siteName, module, "log info");
         * FATAL(siteName, module, "log info, a is:%s; b is: %d", a, b);
         * </pre>
         *
         * @param   ��Ӧ��LogImpl����, �Զ������ǰ��������: srcFile, srcLine, level
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
 * ��־�ǼǴ���ʵ������
 */
void    LogImpl(const char *srcFile, int srcLine, LOG_LEVEL *level,
        const char *module, const char *fmt, ...);
/* -------------------------           */



/*
 * ��־�ǼǴ���ʵ������ (Ϊ��֧�ֱ�κ��ϵͳ����)
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
