/******************************************************************
** 文件名:      log_config.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.31
** 描 述:       日志配置
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_config.c,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "log_config.h"
#include    "log.h"
#include    "../symbol.h"
#include    "../ini/get_ini.h"
#include    "../lib/common_lib.h"
#include    "../string/common_str.h"

/*
 * 属性
 */
static LOG_CONFIG   _logConfigList[MAX_LOG_APPENDER_NUMBER];    /* 日志配置列表 */
static int          _logConfigListLength = 0;                   /* 日志配置列表长度 */


/*
 * 内部函数声明
 */
static BOOL    		InitLogConfigSpecific(char *configFile, int appNumber, LOG_CONFIG_ITEM *appNames);
static char*        ReadRootLogConfig(char*, char*, char*);
static LOG_CONFIG*  ReadLogConfig(char*, char*);
static void         ResetLogConfigList();
static void         AddLogConfig(LOG_CONFIG*);


/*****************************************************************
** 函数名:      InitRootLogConfig
** 输 入:       无
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    初始化日志根配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitRootLogConfig(char *configFile, char *rootAppName) {
    return InitRootLogConfigExceptive(configFile, rootAppName,
            LOG_CONFIG_FILE_FIELD_ROOT_CATEGORY);
}


/*****************************************************************
** 函数名:      InitRootLogConfigExceptive
** 输 入:       无
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    初始化根日志配置（可指定配置项名称）
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitRootLogConfigExceptive(char *configFile, char *rootAppName, char *rootCategoryField) {
    LOG_CONFIG_ITEM 	configAppNames[MAX_LOG_APPENDER_NUMBER];
    int         		i = 0;
    char        		*rootCategory = NULL;
    char        		*pStr = NULL;
    LOG_LEVEL   		*allowableMinLogLevel = NULL;
    
    if (IsEmptyString(configFile) || IsEmptyString(rootAppName)
            || IsEmptyString(rootCategoryField)) {
        return FALSE;
    }

    
    if ((rootCategory = ReadRootLogConfig(configFile, rootAppName, rootCategoryField))) {
    	i = 0;
    	
    	/*
    	 * 添加根配置自身到日志配置列表中
    	 */
    	if (ReadLogConfig(configFile, rootAppName) != NULL) {
    		strncpy(configAppNames[i].appName, TrimRude(rootAppName), 
            		sizeof(configAppNames[i].appName) - 1);
            i ++;
    	}
    	
        /*
         * 对rootCategory的第一项配置做特殊处理
         * 如果第一项参数值与日志模式名称匹配，则将其视为最小日志模式配置
         */
        pStr = strtok(rootCategory, LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER);
        if ((allowableMinLogLevel = GetLogLevel(pStr))) {
            SetAllowableMinLogLevel(allowableMinLogLevel);
        } else {
            strncpy(configAppNames[i].appName, TrimRude(pStr), 
            		sizeof(configAppNames[i].appName) - 1);
            i ++;
        }
        
        /*
         * 处理其后的Category配置
         */
        for (; (pStr = strtok(NULL, LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER))
                && i < MAX_LOG_APPENDER_NUMBER; i++) {
            strncpy(configAppNames[i].appName, TrimRude(pStr), 
            		sizeof(configAppNames[i].appName) - 1);
        }
        
        return InitLogConfigSpecific(configFile, i, configAppNames);
    } else {
        return InitLogConfig(configFile, rootAppName);
    }
}


/*****************************************************************
** 函数名:      InitLogConfig
** 输 入:       configFile:     配置文件路径
**              appNames:       区段名称
**                              可以是以","分割的多个值如"log1, log2"
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    初始化日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitLogConfig(char *configFile, char *appNames) {
    LOG_CONFIG_ITEM 	configAppNames[MAX_LOG_APPENDER_NUMBER];
    char    			*pStr = NULL;
    int     			i = 0;

    if (!IsEmptyString(appNames)) {
      char appNamesArray[128];

      bzero(appNamesArray, sizeof(appNamesArray));

      strncpy(appNamesArray, appNames, sizeof(appNamesArray) - 1);
        pStr = strtok(appNamesArray, LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER);
        for (i = 0; pStr && i < MAX_LOG_APPENDER_NUMBER; i++) {
            strncpy(configAppNames[i].appName, TrimRude(pStr), 
            		sizeof(configAppNames[i].appName) - 1);
            pStr = strtok(NULL, LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER);
        }
    }
    return InitLogConfigSpecific(configFile, i, configAppNames);
}


/*****************************************************************
** 函数名:      InitLogConfigMulti
** 输 入:       configFile:     配置文件路径
**              appName1:       区段名称1，可以为NULL
**              appName2:       区段名称2，可以为NULL
**              appName3:       区段名称3，可以为NULL
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    初始化日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
InitLogConfigMulti(char *configFile, char *appName1, char *appName2,
        char *appName3) {
    LOG_CONFIG_ITEM 	configAppNames[3];
    int					i = 0;
    
    if (!IsEmptyString(appName1)) {
	    strncpy(configAppNames[i].appName, Trim(appName1), 
	    		sizeof(configAppNames[i].appName) - 1);
	    i ++;
    }
    
    if (!IsEmptyString(appName2)) {
	    strncpy(configAppNames[i].appName, Trim(appName2), 
	    		sizeof(configAppNames[i].appName) - 1);
	    i ++;
    }
    
    if (!IsEmptyString(appName3)) {
	    strncpy(configAppNames[i].appName, Trim(appName3), 
	    		sizeof(configAppNames[i].appName) - 1);
	    i ++;
    }
    
    return InitLogConfigSpecific(configFile, i, configAppNames);
}


/*****************************************************************
** 函数名:      InitLogConfigSpecific
** 输 入:       configFile:     配置文件路径
**              appNumber:      区段个数
**              appNames:       区段名称列表
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    初始化日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static BOOL
InitLogConfigSpecific(char *configFile, int appNumber, LOG_CONFIG_ITEM *appNames) {
    int         i       = 0;
    int			j 		= 0;
    LOG_CONFIG  *config = NULL;

    ResetLogConfigList();
    
    /*
     * 消除重复项
     */
    for (i = 0; i < appNumber - 1; i++) {
    	if (!IsEmptyString(appNames[i].appName)) {
	    	for (j = i + 1; j < appNumber; j++) {
	    		if (!IsEmptyString(appNames[j].appName) 
	    				&& !strcmp(appNames[j].appName, appNames[i].appName)) {
	    			appNames[j].appName[0] = '\0';
	    		}
	    	}
	    }
    }
    
	/*
	 * 读取日志配置
	 */
    for (i = 0; i < appNumber; i++) {
        if (!IsEmptyString(appNames[i].appName)) {
            if ((config = ReadLogConfig(configFile, appNames[i].appName))) {
                AddLogConfig(config);
            }
        }
    }

    if (_logConfigListLength < 0) {
        return FALSE;
    }

    /*
     * 初始化日志系统
     */
    return InitLog(_logConfigListLength, _logConfigList);
}


/*****************************************************************
** 函数名:      GetLogConfigListLength
** 输 入:
** 输 出:
** 功能描述:    返回日志配置列表长度
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetLogConfigListLength() {
    return _logConfigListLength;
}


/*****************************************************************
** 函数名:      GetLogConfig
** 输 入:
** 输 出:       LOG_CONFIG*
** 功能描述:    返回指定位置的日志配置
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
LOG_CONFIG*
GetLogConfig(int index) {
    if (index < 0 || index >= _logConfigListLength) {
        return NULL;
    }
    return &_logConfigList[index];
}


/*****************************************************************
** 函数名:      ReadRootLogConfig
** 输 入:       无
** 输 出:       LOG_CONFIG*
** 功能描述:    加载根日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.11.1
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static char*
ReadRootLogConfig(char *configFile, char *appName, char *rootCategoryField) {
    static char rootCategory[128];
    char        buf[256];

    bzero(rootCategory, sizeof(rootCategory));

    /*
     * 读取日志文件名称
     */
    if (get_ini_string(configFile, appName, rootCategoryField,
            "", buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        return NULL;
    } else {
        strncpy(rootCategory, buf, sizeof(rootCategory) - 1);
    }

    return rootCategory;
}


/*****************************************************************
** 函数名:      ReadLogConfig
** 输 入:       无
** 输 出:       LOG_CONFIG*
** 功能描述:    加载日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static LOG_CONFIG*
ReadLogConfig(char *configFile, char *appName) {
    static LOG_CONFIG  logConfig;
    char               buf[256];

    bzero((char*) &logConfig, sizeof(LOG_CONFIG));

    /*
     * 读取日志模式
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_LOG_MODE,
            "", buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        /*
	     * 若“日志模式”未配置，则系统将不会把他作为日志配置项处理
	     */
        return NULL;
    } else {
        strncpy(logConfig.logMode, buf, sizeof(logConfig.logMode) - 1);
    }
    
    /*
     * 读取日志登记的起始级别
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_THRESHOLD_MIN,
            LOG_DEFAULT_MIN_LOG_LEVEL_STR, buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_THRESHOLD,
                LOG_DEFAULT_MIN_LOG_LEVEL_STR, buf, sizeof(buf)) < 0) {
            strncpy(logConfig.minLogLevel, LOG_DEFAULT_MIN_LOG_LEVEL_STR,
                    sizeof(logConfig.minLogLevel) - 1);
        } else {
            if(strlen(RtrimRude(buf))>0){
	    		strncpy(logConfig.minLogLevel, buf, sizeof(logConfig.minLogLevel) - 1);
	    	}
	    	else{
	    		strncpy(logConfig.minLogLevel, LOG_DEFAULT_MIN_LOG_LEVEL_STR, sizeof(logConfig.minLogLevel) - 1);
	    	}
        }
    } else {
        if(strlen(RtrimRude(buf))>0){
    		strncpy(logConfig.minLogLevel, buf, sizeof(logConfig.minLogLevel) - 1);
    	}
    	else{
    		strncpy(logConfig.minLogLevel, LOG_DEFAULT_MIN_LOG_LEVEL_STR, sizeof(logConfig.minLogLevel) - 1);
    	}
    }

    /*
     * 读取日志登记的最高级别
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_THRESHOLD_MAX,
            LOG_DEFAULT_MIN_LOG_LEVEL_STR, buf, sizeof(buf)) < 0) {
        strncpy(logConfig.maxLogLevel, LOG_DEFAULT_MAX_LOG_LEVEL_STR,
                sizeof(logConfig.maxLogLevel) - 1);
    } else {
        if(strlen(RtrimRude(buf))>0){
    		strncpy(logConfig.maxLogLevel, buf, sizeof(logConfig.maxLogLevel) - 1);
    	}
    	else{
    		strncpy(logConfig.maxLogLevel, LOG_DEFAULT_MAX_LOG_LEVEL_STR, sizeof(logConfig.maxLogLevel) - 1);
    	}
    }
    
    /*
     * 读取日志文件名称
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_LOG_FILE,
            "", buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        ;
    } else {
        strncpy(logConfig.logFile, buf, sizeof(logConfig.logFile) - 1);
    }
    
    /*
     * 读取日志文件最大长度
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_MAX_FILE_LENGTH,
            LOG_DEFAULT_MAX_FILE_LENGTH_STR, buf, sizeof(buf)) < 0) {
        logConfig.maxFileLength = atoi(LOG_DEFAULT_MAX_FILE_LENGTH_STR);
    } else {
        if(strlen(RtrimRude(buf))>0){
    		logConfig.maxFileLength = atoi(buf);
    	}
    	else{
    		logConfig.maxFileLength = atoi(LOG_DEFAULT_MAX_FILE_LENGTH_STR);
    	}
    }

    /*
     * 读取日志文件最大备份数
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_MAX_BACKUP_INDEX,
            LOG_DEFAULT_MAX_BACKUP_INDEX_STR, buf, sizeof(buf)) < 0) {
        logConfig.maxBackupIndex = atoi(LOG_DEFAULT_MAX_BACKUP_INDEX_STR);
    } else {
        if(strlen(RtrimRude(buf))>0){
    		logConfig.maxBackupIndex = atoi(buf);
    	}
    	else{
    		logConfig.maxBackupIndex = atoi(LOG_DEFAULT_MAX_BACKUP_INDEX_STR);
    	}
    }

    return &logConfig;
}


/*****************************************************************
** 函数名:      ResetLogConfigList
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
ResetLogConfigList() {
    memset(_logConfigList, 0, sizeof(_logConfigList));
    _logConfigListLength = 0;
}


/*****************************************************************
** 函数名:      AddLogConfig
** 输 入:       无
** 输 出:       LOG_CONFIG*
** 功能描述:    添加日志配置
** 全局变量:    无
** 调用模块:    无
** 作 者:       Song Fang
** 日 期:       2005.10.31
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
AddLogConfig(LOG_CONFIG *config) {
    if (config && _logConfigListLength < MAX_LOG_APPENDER_NUMBER) {
        memcpy(&_logConfigList[_logConfigListLength++], config, sizeof(LOG_CONFIG));
    }
}
