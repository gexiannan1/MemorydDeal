/******************************************************************
** �ļ���:      log.c
** �� ��:       0.4
** ������:      Song Fang
** �� ��:       2004.4.26
** �� ��:       ��־�������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log.c,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <stdarg.h>
#include    "log.h"
#include    "log_mode.h"
#include    "log_level.h"
#include    "log_file.h"
#include    "../symbol.h"
#include    "../macro/macro.h"
#include    "../lib/common_lib.h"
#include    "../string/common_str.h"


/*
 * ����
 */
static BOOL             _isLogInitialized = FALSE;                          /* ��־ϵͳ��ʼ����־ */
static char             _systemName[64] = "";                               /* ϵͳ���� */
static LOG_APPENDER     _logAppenderList[MAX_LOG_APPENDER_NUMBER];          /* ��־�������б� */
static int              _logAppenderListLength = 0;                         /* ��־�������б��� */
static LOG_LEVEL        *_allowableMinLogLevel = LOG_TRACE;                 /* �������ʼ��־�ǼǼ��� */

static LOG_CONFIG       _originalLogConfigList[MAX_LOG_APPENDER_NUMBER];    /* ԭʼ��־�����б� */
static int              _originalLogConfigListLength = 0;                   /* ԭʼ��־�����б��� */


/*
 * �ڲ���������
 */
static LOG_APPENDER*    ResolveLogAppender(LOG_CONFIG*);                    /* ������־���� */
static void             ResetLogAppenderList();                             /* ������־���� */
static void             AddLogAppender(LOG_APPENDER*);                      /* �����־������ */
static void             AddOriginalLogConfig(LOG_CONFIG*);                  /* ���ԭʼ��־���� */
static void             DoFileLog(const char *srcFile, int srcLine,
                        LOG_LEVEL *level, const char *module,
                        const char *msg);


/*****************************************************************
** ������:      InitLog
** �� ��:       configNumber:   ��־�ļ�����������
**              configs:        ��־�ļ��������б�
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��־ϵͳ��ʼ������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
InitLog(int configNumber, LOG_CONFIG configs[]) {
    return InitLogWithSystemName(NULL, configNumber, configs);
}


/*****************************************************************
** ������:      InitLogWithSystemName
** �� ��:       systemName:     ϵͳ����
**              configNumber:   ��־�ļ�����������
**              configs:        ��־�ļ��������б�
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��־ϵͳ��ʼ������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
InitLogWithSystemName(char *systemName, int configNumber, LOG_CONFIG configs[]) {

#if _LOG_ABLE

    int             i               = 0;
    LOG_APPENDER    *logAppender    = NULL;

    /*
     * ������־����
     */
    ResetLogAppenderList();

    if (!configs || configNumber <= 0) {
        return FALSE;
    }

    /*
     * ������־ϵͳ����
     */
    if (systemName) {
        SetLogSystemName(systemName);
    }

    /*
     * �����־����
     */
    for (i = 0; i < configNumber; i++) {
        if (!(logAppender = ResolveLogAppender(&configs[i]))) {
            return FALSE;
        }

        if (IsFileLogAppender(logAppender->logMode)) {
            /*
             * �����ļ���־ϵͳ�������Ƿ���Ч
             */
            if (!TestFileLogConfig(logAppender)) {
                return FALSE;
            }
        }

        AddLogAppender(logAppender);
        AddOriginalLogConfig(&configs[i]);
    }

    if (_logAppenderListLength <= 0) {
        return FALSE;
    }

    _isLogInitialized = TRUE;

#endif

    return TRUE;
}


/*****************************************************************
** ������:      RebuildLogConfig
** �� ��:       ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ���½�����Ӧ����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
RebuildLogConfig() {
    return RebuildLogConfigWithSystemName(NULL);
}


/*****************************************************************
** ������:      RebuildLogConfig
** �� ��:       ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ���½�����Ӧ����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
RebuildLogConfigWithSystemName(char *systemName) {
    return InitLogWithSystemName(systemName, _originalLogConfigListLength,
            _originalLogConfigList);
}


/*****************************************************************
** ������:      IsLogInitialized
** �� ��:
** �� ��:
** ��������:    ������־ϵͳ�Ƿ��ѳ�ʼ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsLogInitialized() {
    return _isLogInitialized;
}


/*****************************************************************
** ������:      GetLogSystemName
** �� ��:
** �� ��:
** ��������:    ������־ϵͳ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetLogSystemName() {
    return _systemName;
}


/*****************************************************************
** ������:      SetLogSystemName
** �� ��:
** �� ��:
** ��������:    ������־ϵͳ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetLogSystemName(char *systemName) {
    if (systemName) {
        strncpy(_systemName, systemName, sizeof(_systemName) - 1);
    } else {
        _systemName[0] = '\0';
    }
}


/*****************************************************************
** ������:      GetAllowableMinLogLevel
** �� ��:
** �� ��:       LOG_LEVEL*
** ��������:    �����������ʼ��־�ǼǼ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
LOG_LEVEL*
GetAllowableMinLogLevel() {
    if (!_allowableMinLogLevel) {
        _allowableMinLogLevel = LOG_TRACE;
    }
    return _allowableMinLogLevel;
}


/*****************************************************************
** ������:      SetAllowableMinLogLevel
** �� ��:
** �� ��:       ��
** ��������:    �����������ʼ��־�ǼǼ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetAllowableMinLogLevel(LOG_LEVEL *allowableMinLogLevel) {
    if (allowableMinLogLevel) {
        _allowableMinLogLevel = allowableMinLogLevel;
    }
}


/*****************************************************************
** ������:      GetLogAppenderList
** �� ��:
** �� ��:       LOG_APPENDER*
** ��������:    ������־�������б�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
LOG_APPENDER*
GetLogAppenderList() {
    return _logAppenderList;
}


/*****************************************************************
** ������:      GetLogAppender
** �� ��:
** �� ��:       LOG_APPENDER*
** ��������:    ����ָ��λ�õ���־������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
LOG_APPENDER*
GetLogAppender(int index) {
    if (index < 0 || index >= _logAppenderListLength) {
        return NULL;
    }
    return &_logAppenderList[index];
}


/*****************************************************************
** ������:      GetLogAppenderNumber
** �� ��:
** �� ��:
** ��������:    ������־�������б���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetLogAppenderNumber() {
    return _logAppenderListLength;
}


/*****************************************************************
** ������:      GetOriginalLogConfigListLength
** �� ��:
** �� ��:
** ��������:    ����ԭʼ��־�����б���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetOriginalLogConfigListLength() {
    return _originalLogConfigListLength;
}


/*****************************************************************
** ������:      GetOriginalLogConfig
** �� ��:
** �� ��:       LOG_CONFIG*
** ��������:    ����ָ��λ�õ�ԭʼ��־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
LOG_CONFIG*
GetOriginalLogConfig(int index) {
    if (index < 0 || index >= _originalLogConfigListLength) {
        return NULL;
    }
    return &_originalLogConfigList[index];
}


/*****************************************************************
** ������:      ResolveLogAppender
** �� ��:       LOG_CONFIG*
** �� ��:       LOG_APPENDER*
** ��������:    ������־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static LOG_APPENDER*
ResolveLogAppender(LOG_CONFIG *config) {
    static LOG_APPENDER     logAppender;
    LOG_MODE                *logMode = NULL;
    LOG_LEVEL               *logLevel = NULL;

    if (!config) {
        return NULL;
    }

    /*
     * ������־ģʽ
     */
    if ((logMode = GetLogMode(config->logMode))) {
        logAppender.logMode = logMode;
    } else {
        return NULL;
    }

    /*
     * ���ó�ʼĬ��ֵ
     */
    logAppender.minLogLevel = LOG_DEFAULT_MIN_LOG_LEVEL;
    logAppender.maxLogLevel = LOG_DEFAULT_MAX_LOG_LEVEL;
    logAppender.maxFileLength = LOG_DEFAULT_MAX_FILE_LENGTH;
    logAppender.maxBackupIndex = LOG_DEFAULT_MAX_BACKUP_INDEX;

    /*
     * ������־��ʼ�ǼǼ���
     */
    if ((logLevel = GetLogLevel(config->minLogLevel))) {
        logAppender.minLogLevel = logLevel;
    }

    /*
     * ������־��ߵǼǼ���
     */
    if ((logLevel = GetLogLevel(config->maxLogLevel))) {
        logAppender.maxLogLevel = logLevel;
    }

    if (IsFileLogAppender(logAppender.logMode)) {
        /*
         * ������־�ļ�·��
         */
        char *pAppHome = ReplaceMacroAppHome(config->logFile);
        char *pLogSystemName = GetLogSystemName();
        char *pSystemName = ReplaceMacroSystemName(pAppHome, pLogSystemName);
        				
        strncpy(logAppender.logFile, pSystemName, 
                sizeof(logAppender.logFile) - 1);

        /*
         * ������־�ļ���󳤶�����󱸷���
         */
        logAppender.maxFileLength = config->maxFileLength;
        logAppender.maxBackupIndex = config->maxBackupIndex;
    }

    return &logAppender;
}


/*****************************************************************
** ������:      ResetLogAppenderList
** �� ��:       ��
** �� ��:       ��
** ��������:    ������־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
ResetLogAppenderList() {
    _isLogInitialized = FALSE;

    _logAppenderListLength = 0;
    memset(_logAppenderList, 0, sizeof(_logAppenderList));

    _originalLogConfigListLength = 0;
}


/*****************************************************************
** ������:      AddLogAppender
** �� ��:       LOG_APPENDER*
** �� ��:       ��
** ��������:    �����־������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
AddLogAppender(LOG_APPENDER *appender) {
    if (appender && _logAppenderListLength < MAX_LOG_APPENDER_NUMBER) {
        memcpy(&_logAppenderList[_logAppenderListLength++],
                appender, sizeof(LOG_APPENDER));
    }
}


/*****************************************************************
** ������:      AddOriginalLogConfig
** �� ��:       LOG_CONFIG*
** �� ��:       ��
** ��������:    ���ԭʼ��־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
AddOriginalLogConfig(LOG_CONFIG *config) {
    if (config && _originalLogConfigListLength < MAX_LOG_APPENDER_NUMBER) {
        if (config == &_originalLogConfigList[_originalLogConfigListLength]) {
            _originalLogConfigListLength ++;
        } else {
            memcpy(&_originalLogConfigList[_originalLogConfigListLength], config,
                    sizeof(LOG_CONFIG));
            _originalLogConfigListLength ++;
        }
    }
}


/*****************************************************************
** ������:      LogImpl
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ�򻯵��÷�ʽ��������ҵ���߼�)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2004.6.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
DoFileLog(const char *srcFile, int srcLine, LOG_LEVEL *level, const char *module,
        const char *msg) {
    int             i = 0;
    LOG_APPENDER    *specialLogConfig = NULL;

    if (!IsLogInitialized()) {
        return;
    }

    /*
     * �Ǽ���־
     */
    for (i = 0; i < _logAppenderListLength; i++) {
        specialLogConfig = &_logAppenderList[i];
        if (level->level >= _allowableMinLogLevel->level
                && level->level >= specialLogConfig->minLogLevel->level
                && level->level <= specialLogConfig->maxLogLevel->level) {
      //      (*(specialLogConfig->logMode->fileLog))(specialLogConfig, srcFile,
        //            srcLine, level, module, msg);
        }
    }
}


/*****************************************************************
** ������:      LogImpl
** �� ��:       srcFile:  ���ó���Ĵ����ļ�����
**              srcLine:  ���ô��ڴ����ļ��е�����
**              level:    ��־����
**              module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ�򻯵��÷�ʽ��������ҵ���߼�)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2004.6.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
LogImpl(const char *srcFile, int srcLine, LOG_LEVEL *level, const char *module,
        const char *fmt, ...) {
    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog(srcFile, srcLine, level, moduleName, buf);
}


/*****************************************************************
** ������:      LogSimpleness
** �� ��:       fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
LogSimpleness(const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_TRACE;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, "");

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      LogFullSimpleness
** �� ��:       level:    ��־����
**              module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
LogFullSimpleness(LOG_LEVEL *level, const char *module, const char *fmt, ...) {
    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      TraceSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
TraceSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_TRACE;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      DebugSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
DebugSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_DEBUG;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      InfoSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
InfoSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_INFO;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      WarnSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
WarnSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_WARN;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      ErrorSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
ErrorSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_ERROR;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      FatalSimpleness
** �� ��:       module:   ��Դģ������(Ϊ�Ǽ����ݿ���־)
**              fmt:      ��־����, ֧�ֿɱ����
**              ...:      �ɱ�����б�
** �� ��:       ��
** ��������:    ��־�ǼǺ���(Ϊ��֧�ֱ�κ��ϵͳ����)
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
FatalSimpleness(const char *module, const char *fmt, ...) {
    LOG_LEVEL           *level = LOG_FATAL;

    va_list             ap;
    char                buf[MAX_LOG_INFO_SIZE];
    char                moduleName[128];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    sprintf(moduleName, "%s.%s", _systemName, module);

    /*
     * �Ǽ���־
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** ������:      PrintLogConfigInfo
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ��־������Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintLogConfigInfo() {

#if _LOG_ABLE

    int             i                       = 0;
    LOG_APPENDER    *appender               = NULL;
    LOG_LEVEL       *allowableMinLogLevel   = NULL;

    if ((allowableMinLogLevel = GetAllowableMinLogLevel())) {
        printf("LOG CONFIG: allowableMinLogLevel: %s\n", allowableMinLogLevel->name);
    }

    for (i = 0; i < GetLogAppenderNumber(); i++) {
        appender = GetLogAppender(i);

        printf("LOG APPENDER [%d]: logFile: %s\n", i, appender->logFile);
        printf("LOG APPENDER [%d]: logMode: %s\n", i, appender->logMode->name);
        printf("LOG APPENDER [%d]: minLogLevel: %s\n", i, appender->minLogLevel->name);
        printf("LOG APPENDER [%d]: maxLogLevel: %s\n", i, appender->maxLogLevel->name);
        printf("LOG APPENDER [%d]: maxFileLength: %d\n", i, appender->maxFileLength);
        printf("LOG APPENDER [%d]: maxBackupIndex: %d\n", i, appender->maxBackupIndex);
    }

#else

    printf("LOG CONFIG: LOG CLOSED!\n");

#endif

    printf("\n");

}
