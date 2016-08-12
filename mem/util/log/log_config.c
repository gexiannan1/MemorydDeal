/******************************************************************
** �ļ���:      log_config.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.31
** �� ��:       ��־����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ����
 */
static LOG_CONFIG   _logConfigList[MAX_LOG_APPENDER_NUMBER];    /* ��־�����б� */
static int          _logConfigListLength = 0;                   /* ��־�����б��� */


/*
 * �ڲ���������
 */
static BOOL    		InitLogConfigSpecific(char *configFile, int appNumber, LOG_CONFIG_ITEM *appNames);
static char*        ReadRootLogConfig(char*, char*, char*);
static LOG_CONFIG*  ReadLogConfig(char*, char*);
static void         ResetLogConfigList();
static void         AddLogConfig(LOG_CONFIG*);


/*****************************************************************
** ������:      InitRootLogConfig
** �� ��:       ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��ʼ����־������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
InitRootLogConfig(char *configFile, char *rootAppName) {
    return InitRootLogConfigExceptive(configFile, rootAppName,
            LOG_CONFIG_FILE_FIELD_ROOT_CATEGORY);
}


/*****************************************************************
** ������:      InitRootLogConfigExceptive
** �� ��:       ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��ʼ������־���ã���ָ�����������ƣ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
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
    	 * ��Ӹ�����������־�����б���
    	 */
    	if (ReadLogConfig(configFile, rootAppName) != NULL) {
    		strncpy(configAppNames[i].appName, TrimRude(rootAppName), 
            		sizeof(configAppNames[i].appName) - 1);
            i ++;
    	}
    	
        /*
         * ��rootCategory�ĵ�һ�����������⴦��
         * �����һ�����ֵ����־ģʽ����ƥ�䣬������Ϊ��С��־ģʽ����
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
         * ��������Category����
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
** ������:      InitLogConfig
** �� ��:       configFile:     �����ļ�·��
**              appNames:       ��������
**                              ��������","�ָ�Ķ��ֵ��"log1, log2"
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��ʼ����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      InitLogConfigMulti
** �� ��:       configFile:     �����ļ�·��
**              appName1:       ��������1������ΪNULL
**              appName2:       ��������2������ΪNULL
**              appName3:       ��������3������ΪNULL
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��ʼ����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      InitLogConfigSpecific
** �� ��:       configFile:     �����ļ�·��
**              appNumber:      ���θ���
**              appNames:       ���������б�
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ��ʼ����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static BOOL
InitLogConfigSpecific(char *configFile, int appNumber, LOG_CONFIG_ITEM *appNames) {
    int         i       = 0;
    int			j 		= 0;
    LOG_CONFIG  *config = NULL;

    ResetLogConfigList();
    
    /*
     * �����ظ���
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
	 * ��ȡ��־����
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
     * ��ʼ����־ϵͳ
     */
    return InitLog(_logConfigListLength, _logConfigList);
}


/*****************************************************************
** ������:      GetLogConfigListLength
** �� ��:
** �� ��:
** ��������:    ������־�����б���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetLogConfigListLength() {
    return _logConfigListLength;
}


/*****************************************************************
** ������:      GetLogConfig
** �� ��:
** �� ��:       LOG_CONFIG*
** ��������:    ����ָ��λ�õ���־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
LOG_CONFIG*
GetLogConfig(int index) {
    if (index < 0 || index >= _logConfigListLength) {
        return NULL;
    }
    return &_logConfigList[index];
}


/*****************************************************************
** ������:      ReadRootLogConfig
** �� ��:       ��
** �� ��:       LOG_CONFIG*
** ��������:    ���ظ���־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.11.1
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static char*
ReadRootLogConfig(char *configFile, char *appName, char *rootCategoryField) {
    static char rootCategory[128];
    char        buf[256];

    bzero(rootCategory, sizeof(rootCategory));

    /*
     * ��ȡ��־�ļ�����
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
** ������:      ReadLogConfig
** �� ��:       ��
** �� ��:       LOG_CONFIG*
** ��������:    ������־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static LOG_CONFIG*
ReadLogConfig(char *configFile, char *appName) {
    static LOG_CONFIG  logConfig;
    char               buf[256];

    bzero((char*) &logConfig, sizeof(LOG_CONFIG));

    /*
     * ��ȡ��־ģʽ
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_LOG_MODE,
            "", buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        /*
	     * ������־ģʽ��δ���ã���ϵͳ�����������Ϊ��־�������
	     */
        return NULL;
    } else {
        strncpy(logConfig.logMode, buf, sizeof(logConfig.logMode) - 1);
    }
    
    /*
     * ��ȡ��־�Ǽǵ���ʼ����
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
     * ��ȡ��־�Ǽǵ���߼���
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
     * ��ȡ��־�ļ�����
     */
    if (get_ini_string(configFile, appName, LOG_CONFIG_FILE_FIELD_LOG_FILE,
            "", buf, sizeof(buf)) < 0
            || IsEmptyString(buf)) {
        ;
    } else {
        strncpy(logConfig.logFile, buf, sizeof(logConfig.logFile) - 1);
    }
    
    /*
     * ��ȡ��־�ļ���󳤶�
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
     * ��ȡ��־�ļ���󱸷���
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
** ������:      ResetLogConfigList
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
ResetLogConfigList() {
    memset(_logConfigList, 0, sizeof(_logConfigList));
    _logConfigListLength = 0;
}


/*****************************************************************
** ������:      AddLogConfig
** �� ��:       ��
** �� ��:       LOG_CONFIG*
** ��������:    �����־����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.31
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
AddLogConfig(LOG_CONFIG *config) {
    if (config && _logConfigListLength < MAX_LOG_APPENDER_NUMBER) {
        memcpy(&_logConfigList[_logConfigListLength++], config, sizeof(LOG_CONFIG));
    }
}
