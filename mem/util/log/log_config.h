/******************************************************************
** 文件名:      log_config.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.31
** 描 述:       日志配置头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/log/log_config.h,v 1.1 2012/03/29 01:20:13 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:13 $
 */

#ifndef _LOG_CONFIG_H
#define _LOG_CONFIG_H


#include    "log.h"
#include    "../symbol.h"


/*
 * 常量定义
 */
#define LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER       ","
#define LOG_CONFIG_FILE_FIELD_ROOT_CATEGORY         "log.root_category"			/* 日志根配置 */
#define LOG_CONFIG_FILE_FIELD_LOG_MODE              "log.mode"					/* 日志模式（DATE_SINGLE、DATE、ALTERNATE） */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD             "log.threshold"				/* 起始日志登记级别 */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD_MIN         "log.threshold.min"			/* 最小日志登记级别 */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD_MAX         "log.threshold.max"			/* 最大日志登记级别 */
#define LOG_CONFIG_FILE_FIELD_LOG_FILE              "log.file"					/* 日志文件路径与名称 */
#define LOG_CONFIG_FILE_FIELD_MAX_FILE_LENGTH       "log.file.max_file_length"	/* 日志文件最大长度 */
#define LOG_CONFIG_FILE_FIELD_MAX_BACKUP_INDEX      "log.file.max_backup_index"	/* 日志文件最大备份数 */


/*
 * 结构体定义
 */

/*
 * 日志配置条目
 */
typedef struct {
    char    appName[128];							/* 日志配置的区段名称 */
} LOG_CONFIG_ITEM;


/*
 * 函数声明
 */
BOOL    InitRootLogConfig(char *configFile, char *rootAppName);     /* 初始化根日志配置 */
BOOL    InitRootLogConfigExceptive(char *configFile,
        char *rootAppName, char *rootCategoryField);                /* 初始化根日志配置（可指定配置项名称） */

BOOL    InitLogConfig(char *configFile, char *appNames);            /* 初始化日志配置 */
BOOL    InitLogConfigMulti(char *configFile, char *appName1,
        char *appName2, char *appName3);                            /* 初始化日志配置 */

int             GetLogConfigListLength();                           /* 返回日志配置列表长度 */
LOG_CONFIG*     GetLogConfig(int);                                  /* 返回指定位置的日志配置 */

#endif  /* _LOG_CONFIG_H */
