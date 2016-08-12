/******************************************************************
** 文件名:      log.c
** 版 本:       0.4
** 创建人:      Song Fang
** 日 期:       2004.4.26
** 描 述:       日志处理程序
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 属性
 */
static BOOL             _isLogInitialized = FALSE;                          /* 日志系统初始化标志 */
static char             _systemName[64] = "";                               /* 系统名称 */
static LOG_APPENDER     _logAppenderList[MAX_LOG_APPENDER_NUMBER];          /* 日志处理器列表 */
static int              _logAppenderListLength = 0;                         /* 日志处理器列表长度 */
static LOG_LEVEL        *_allowableMinLogLevel = LOG_TRACE;                 /* 允许的起始日志登记级别 */

static LOG_CONFIG       _originalLogConfigList[MAX_LOG_APPENDER_NUMBER];    /* 原始日志配置列表 */
static int              _originalLogConfigListLength = 0;                   /* 原始日志配置列表长度 */


/*
 * 内部函数声明
 */
static LOG_APPENDER*    ResolveLogAppender(LOG_CONFIG*);                    /* 解析日志配置 */
static void             ResetLogAppenderList();                             /* 重置日志配置 */
static void             AddLogAppender(LOG_APPENDER*);                      /* 添加日志处理器 */
static void             AddOriginalLogConfig(LOG_CONFIG*);                  /* 添加原始日志配置 */
static void             DoFileLog(const char *srcFile, int srcLine,
                        LOG_LEVEL *level, const char *module,
                        const char *msg);


/*****************************************************************
** 函数名:      InitLog
** 输 入:       configNumber:   日志文件配置项数量
**              configs:        日志文件配置项列表
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    日志系统初始化函数
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitLog(int configNumber, LOG_CONFIG configs[]) {
    return InitLogWithSystemName(NULL, configNumber, configs);
}


/*****************************************************************
** 函数名:      InitLogWithSystemName
** 输 入:       systemName:     系统名称
**              configNumber:   日志文件配置项数量
**              configs:        日志文件配置项列表
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    日志系统初始化函数
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitLogWithSystemName(char *systemName, int configNumber, LOG_CONFIG configs[]) {

#if _LOG_ABLE

    int             i               = 0;
    LOG_APPENDER    *logAppender    = NULL;

    /*
     * 重置日志配置
     */
    ResetLogAppenderList();

    if (!configs || configNumber <= 0) {
        return FALSE;
    }

    /*
     * 设置日志系统名称
     */
    if (systemName) {
        SetLogSystemName(systemName);
    }

    /*
     * 添加日志配置
     */
    for (i = 0; i < configNumber; i++) {
        if (!(logAppender = ResolveLogAppender(&configs[i]))) {
            return FALSE;
        }

        if (IsFileLogAppender(logAppender->logMode)) {
            /*
             * 测试文件日志系统的配置是否有效
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
** 函数名:      RebuildLogConfig
** 输 入:       无
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    重新解析、应用日志配置
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
RebuildLogConfig() {
    return RebuildLogConfigWithSystemName(NULL);
}


/*****************************************************************
** 函数名:      RebuildLogConfig
** 输 入:       无
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    重新解析、应用日志配置
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
RebuildLogConfigWithSystemName(char *systemName) {
    return InitLogWithSystemName(systemName, _originalLogConfigListLength,
            _originalLogConfigList);
}


/*****************************************************************
** 函数名:      IsLogInitialized
** 输 入:
** 输 出:
** 功能描述:    返回日志系统是否已初始化
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsLogInitialized() {
    return _isLogInitialized;
}


/*****************************************************************
** 函数名:      GetLogSystemName
** 输 入:
** 输 出:
** 功能描述:    返回日志系统名称
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetLogSystemName() {
    return _systemName;
}


/*****************************************************************
** 函数名:      SetLogSystemName
** 输 入:
** 输 出:
** 功能描述:    设置日志系统名称
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      GetAllowableMinLogLevel
** 输 入:
** 输 出:       LOG_LEVEL*
** 功能描述:    返回允许的起始日志登记级别
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_LEVEL*
GetAllowableMinLogLevel() {
    if (!_allowableMinLogLevel) {
        _allowableMinLogLevel = LOG_TRACE;
    }
    return _allowableMinLogLevel;
}


/*****************************************************************
** 函数名:      SetAllowableMinLogLevel
** 输 入:
** 输 出:       无
** 功能描述:    返回允许的起始日志登记级别
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetAllowableMinLogLevel(LOG_LEVEL *allowableMinLogLevel) {
    if (allowableMinLogLevel) {
        _allowableMinLogLevel = allowableMinLogLevel;
    }
}


/*****************************************************************
** 函数名:      GetLogAppenderList
** 输 入:
** 输 出:       LOG_APPENDER*
** 功能描述:    返回日志处理器列表
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_APPENDER*
GetLogAppenderList() {
    return _logAppenderList;
}


/*****************************************************************
** 函数名:      GetLogAppender
** 输 入:
** 输 出:       LOG_APPENDER*
** 功能描述:    返回指定位置的日志处理器
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_APPENDER*
GetLogAppender(int index) {
    if (index < 0 || index >= _logAppenderListLength) {
        return NULL;
    }
    return &_logAppenderList[index];
}


/*****************************************************************
** 函数名:      GetLogAppenderNumber
** 输 入:
** 输 出:
** 功能描述:    返回日志处理器列表长度
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetLogAppenderNumber() {
    return _logAppenderListLength;
}


/*****************************************************************
** 函数名:      GetOriginalLogConfigListLength
** 输 入:
** 输 出:
** 功能描述:    返回原始日志配置列表长度
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetOriginalLogConfigListLength() {
    return _originalLogConfigListLength;
}


/*****************************************************************
** 函数名:      GetOriginalLogConfig
** 输 入:
** 输 出:       LOG_CONFIG*
** 功能描述:    返回指定位置的原始日志配置
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_CONFIG*
GetOriginalLogConfig(int index) {
    if (index < 0 || index >= _originalLogConfigListLength) {
        return NULL;
    }
    return &_originalLogConfigList[index];
}


/*****************************************************************
** 函数名:      ResolveLogAppender
** 输 入:       LOG_CONFIG*
** 输 出:       LOG_APPENDER*
** 功能描述:    解析日志配置
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.30
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 设置日志模式
     */
    if ((logMode = GetLogMode(config->logMode))) {
        logAppender.logMode = logMode;
    } else {
        return NULL;
    }

    /*
     * 设置初始默认值
     */
    logAppender.minLogLevel = LOG_DEFAULT_MIN_LOG_LEVEL;
    logAppender.maxLogLevel = LOG_DEFAULT_MAX_LOG_LEVEL;
    logAppender.maxFileLength = LOG_DEFAULT_MAX_FILE_LENGTH;
    logAppender.maxBackupIndex = LOG_DEFAULT_MAX_BACKUP_INDEX;

    /*
     * 设置日志起始登记级别
     */
    if ((logLevel = GetLogLevel(config->minLogLevel))) {
        logAppender.minLogLevel = logLevel;
    }

    /*
     * 设置日志最高登记级别
     */
    if ((logLevel = GetLogLevel(config->maxLogLevel))) {
        logAppender.maxLogLevel = logLevel;
    }

    if (IsFileLogAppender(logAppender.logMode)) {
        /*
         * 设置日志文件路径
         */
        char *pAppHome = ReplaceMacroAppHome(config->logFile);
        char *pLogSystemName = GetLogSystemName();
        char *pSystemName = ReplaceMacroSystemName(pAppHome, pLogSystemName);
        				
        strncpy(logAppender.logFile, pSystemName, 
                sizeof(logAppender.logFile) - 1);

        /*
         * 设置日志文件最大长度与最大备份数
         */
        logAppender.maxFileLength = config->maxFileLength;
        logAppender.maxBackupIndex = config->maxBackupIndex;
    }

    return &logAppender;
}


/*****************************************************************
** 函数名:      ResetLogAppenderList
** 输 入:       无
** 输 出:       无
** 功能描述:    重置日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
ResetLogAppenderList() {
    _isLogInitialized = FALSE;

    _logAppenderListLength = 0;
    memset(_logAppenderList, 0, sizeof(_logAppenderList));

    _originalLogConfigListLength = 0;
}


/*****************************************************************
** 函数名:      AddLogAppender
** 输 入:       LOG_APPENDER*
** 输 出:       无
** 功能描述:    添加日志处理器
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
AddLogAppender(LOG_APPENDER *appender) {
    if (appender && _logAppenderListLength < MAX_LOG_APPENDER_NUMBER) {
        memcpy(&_logAppenderList[_logAppenderListLength++],
                appender, sizeof(LOG_APPENDER));
    }
}


/*****************************************************************
** 函数名:      AddOriginalLogConfig
** 输 入:       LOG_CONFIG*
** 输 出:       无
** 功能描述:    添加原始日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      LogImpl
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为简化调用方式而夹杂了业务逻辑)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2004.6.7
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
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
** 函数名:      LogImpl
** 输 入:       srcFile:  调用程序的代码文件名称
**              srcLine:  调用处在代码文件中的行数
**              level:    日志级别
**              module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为简化调用方式而夹杂了业务逻辑)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2004.6.7
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog(srcFile, srcLine, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      LogSimpleness
** 输 入:       fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      LogFullSimpleness
** 输 入:       level:    日志级别
**              module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      TraceSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      DebugSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      InfoSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      WarnSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      ErrorSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      FatalSimpleness
** 输 入:       module:   来源模块名称(为登记数据库日志)
**              fmt:      日志内容, 支持可变参数
**              ...:      可变参数列表
** 输 出:       无
** 功能描述:    日志登记函数(为不支持变参宏的系统而设)
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.12
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 登记日志
     */
    DoFileLog("", 0, level, moduleName, buf);
}


/*****************************************************************
** 函数名:      PrintLogConfigInfo
** 输 入:       无
** 输 出:       无
** 功能描述:    打印日志配置信息
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
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
