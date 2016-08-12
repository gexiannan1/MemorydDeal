/******************************************************************
** �ļ���:      log_config.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.31
** �� ��:       ��־����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��������
 */
#define LOG_CONFIG_FILE_MULTI_VALUE_DELIMITER       ","
#define LOG_CONFIG_FILE_FIELD_ROOT_CATEGORY         "log.root_category"			/* ��־������ */
#define LOG_CONFIG_FILE_FIELD_LOG_MODE              "log.mode"					/* ��־ģʽ��DATE_SINGLE��DATE��ALTERNATE�� */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD             "log.threshold"				/* ��ʼ��־�ǼǼ��� */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD_MIN         "log.threshold.min"			/* ��С��־�ǼǼ��� */
#define LOG_CONFIG_FILE_FIELD_THRESHOLD_MAX         "log.threshold.max"			/* �����־�ǼǼ��� */
#define LOG_CONFIG_FILE_FIELD_LOG_FILE              "log.file"					/* ��־�ļ�·�������� */
#define LOG_CONFIG_FILE_FIELD_MAX_FILE_LENGTH       "log.file.max_file_length"	/* ��־�ļ���󳤶� */
#define LOG_CONFIG_FILE_FIELD_MAX_BACKUP_INDEX      "log.file.max_backup_index"	/* ��־�ļ���󱸷��� */


/*
 * �ṹ�嶨��
 */

/*
 * ��־������Ŀ
 */
typedef struct {
    char    appName[128];							/* ��־���õ��������� */
} LOG_CONFIG_ITEM;


/*
 * ��������
 */
BOOL    InitRootLogConfig(char *configFile, char *rootAppName);     /* ��ʼ������־���� */
BOOL    InitRootLogConfigExceptive(char *configFile,
        char *rootAppName, char *rootCategoryField);                /* ��ʼ������־���ã���ָ�����������ƣ� */

BOOL    InitLogConfig(char *configFile, char *appNames);            /* ��ʼ����־���� */
BOOL    InitLogConfigMulti(char *configFile, char *appName1,
        char *appName2, char *appName3);                            /* ��ʼ����־���� */

int             GetLogConfigListLength();                           /* ������־�����б��� */
LOG_CONFIG*     GetLogConfig(int);                                  /* ����ָ��λ�õ���־���� */

#endif  /* _LOG_CONFIG_H */
