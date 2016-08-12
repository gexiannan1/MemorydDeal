/******************************************************************
** �ļ���:      env_base.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.12
** �� ��:       ȫ�ֻ�������������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/env_base/env_base.c,v 1.1 2012/03/29 01:20:10 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:10 $
 */
#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <fcntl.h>
#include    <errno.h>
#include	<unistd.h>

#include    "env_base.h"
#include    "../../util/symbol.h"
#include    "../../util/file/common_file.h"


/*
 * ����(OO���� :-))
 */
static char     _rootEnvStorage[256] = "";      /* ϵͳȫ�ֻ��������ĸ��洢·�� */
static char     _envStorage[256] = "";          /* ϵͳȫ�ֻ��������洢·�� */


/*
 * �ڲ���������
 */
static int		_SetEnvImpl(const char*, const char*, int);
static char*	_GetEnvImpl(const char*);


/*****************************************************************
** ������:      InitEnv
** �� ��:       ȫ�ֻ��������Ĵ洢λ��
** �� ��:       0:  �ɹ�
**              -1: ʧ��
** ��������:    ��ʼ��ȫ�ֻ��������������Ļ���
**              ��Ҫʹ��ȫ�ֻ����������������øú������Գ�ʼ�������Ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
InitEnv(const char *envRoot, const char *systemName) {
	char    envStorage[256];

	if (systemName) {
    	sprintf(envStorage, "%s/%s", envRoot, systemName);
	} else {
		sprintf(envStorage, "%s", envRoot);
	}
	
    if (IsFileExist(envStorage)) {
        if (!IsDir(envStorage)) {
            return -1;
        }
    } else {
        if (!MkDir(envStorage)) {
            return -1;
        }
    }

	SetRootEnvStorage(envRoot);
    SetEnvStorage(envStorage);

    return 0;
}


/*****************************************************************
** ������:      ResetEnv
** �� ��:       ȫ�ֻ��������Ĵ洢λ��
** �� ��:       0:  �ɹ�
**              -1: ʧ��
** ��������:    ����ȫ�ֻ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
ResetEnv(const char *envRoot, const char *systemName) {
	char    envStorage[256];

	if (systemName) {
    	sprintf(envStorage, "%s/%s", envRoot, systemName);
	} else {
		sprintf(envStorage, "%s", envRoot);
	}
	
    if (!RmDir(envStorage)) {
        return -1;
    }
    return 0;
}


/*****************************************************************
** ������:      GetRootEnvStorage
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ϵͳȫ�ֻ��������ĸ��洢·��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetRootEnvStorage() {
    return _rootEnvStorage;
}


/*****************************************************************
** ������:      SetRootEnvStorage
** �� ��:       ϵͳȫ�ֻ��������ĸ��洢·��
** �� ��:       ��
** ��������:    ����ϵͳȫ�ֻ��������ĸ��洢·��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetRootEnvStorage(const char *rootStorage) {
    if (rootStorage != NULL && rootStorage != _rootEnvStorage) {
        strncpy(_rootEnvStorage, rootStorage, sizeof(_rootEnvStorage));
    }
}


/*****************************************************************
** ������:      GetEnvStorage
** �� ��:       ��
** �� ��:       ��
** ��������:    ����ϵͳȫ�ֻ��������洢·��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetEnvStorage() {
    return _envStorage;
}


/*****************************************************************
** ������:      SetEnvStorage
** �� ��:       ϵͳȫ�ֻ��������洢·��
** �� ��:       ��
** ��������:    ����ϵͳȫ�ֻ��������洢·��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetEnvStorage(const char *storage) {
    if (storage != NULL && storage != _envStorage) {
        strncpy(_envStorage, storage, sizeof(_envStorage));
    }
}


/*****************************************************************
** ������:      SetEnv
** �� ��:       name:      ȫ�ֻ�����������
**              value:     ����ֵ
**              overwrite: ��name�Ѵ����Ƿ�ʹ��value����ԭ���ı���ֵ
**                  - 0:   ������
**                  - <>0: ����
** �� ��:       0:  �ɹ�
**              -1: ʧ��
**              1:  ���������Ѵ���(ֻ����overwrite==0������·���)
** ��������:    ����ȫ�ֻ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
SetEnv(const char *name, const char *value, int overwrite) {
    char    buf[256];

    sprintf(buf, "%s/%s", GetEnvStorage(), name);
    
	return _SetEnvImpl(buf, value, overwrite);
}


/*****************************************************************
** ������:      GetEnv
** �� ��:       name:  ȫ�ֻ�����������
** �� ��:       ȫ�ֻ�������ֵ, �������������򷵻�NULL
** ��������:    ����ȫ�ֻ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2004.5.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetEnv(const char *name) {
    char        buf[256];

    sprintf(buf, "%s/%s", GetEnvStorage(), name);

    return _GetEnvImpl(buf);
}


/*****************************************************************
** ������:      SetEnvFully
** �� ��:       systemName: ϵͳ����
**              envName:    ȫ�ֻ�����������
**              value:     	����ֵ
**              overwrite: 	��name�Ѵ����Ƿ�ʹ��value����ԭ���ı���ֵ
**                  - 0:   	������
**                  - <>0: 	����
** �� ��:       0:  �ɹ�
**              -1: ʧ��
**              1:  ���������Ѵ���(ֻ����overwrite==0������·���)
** ��������:    ����ȫ�ֻ�����������ָ��ϵͳ���ƣ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
SetEnvFully(const char *systemName, const char *envName, const char *value, int overwrite) {
    char    buf[256];

    sprintf(buf, "%s/%s/%s", GetRootEnvStorage(), systemName, envName);

    return _SetEnvImpl(buf, value, overwrite);
}


/*****************************************************************
** ������:      GetEnvFully
** �� ��:       envName:  ȫ�ֻ�����������
** �� ��:       ȫ�ֻ�������ֵ, �������������򷵻�NULL
** ��������:    ����ȫ�ֻ�����������ָ��ϵͳ���ƣ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetEnvFully(const char *systemName, const char *envName) {
    char	buf[256];

    sprintf(buf, "%s/%s/%s", GetRootEnvStorage(), systemName, envName);

    return _GetEnvImpl(buf);
}


/*****************************************************************
** ������:      _SetEnvImpl
** �� ��:       env:        ȫ�ֻ�����������
**              value:     	����ֵ
**              overwrite: 	��name�Ѵ����Ƿ�ʹ��value����ԭ���ı���ֵ
**                  - 0:   	������
**                  - <>0: 	����
** �� ��:       0:  �ɹ�
**              -1: ʧ��
**              1:  ���������Ѵ���(ֻ����overwrite==0������·���)
** ��������:    ����ȫ�ֻ�����������ָ��ϵͳ���ƣ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int
_SetEnvImpl(const char *env, const char *value, int overwrite) {
    int     fp = -1;

    if (!overwrite) {
        if ((fp = open(env, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
            return errno == EEXIST ? 1 : -1;
        }
    } else {
        if ((fp = open(env, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) < 0) {
            return -1;
        }
    }

    write(fp, value, strlen(value));
    close(fp);

    return 0;
}


/*****************************************************************
** ������:      _GetEnvImpl
** �� ��:       env:  ȫ�ֻ�����������
** �� ��:       ȫ�ֻ�������ֵ, �������������򷵻�NULL
** ��������:    ����ȫ�ֻ�����������ָ��ϵͳ���ƣ�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.6
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static char*
_GetEnvImpl(const char *env) {
    static int  i = 0;
    static char values[5][256];

    int         fp = -1;

    if (i >= 4) {
        i = 0;
    } else {
        i ++;
    }

    if ((fp = open(env, O_RDONLY)) < 0) {
        return NULL;
    }

    bzero(values[i], sizeof(values[i]));

    read(fp, values[i], sizeof(values[i]));
    close(fp);

    return values[i];
}
