/******************************************************************
** 文件名:      env_base.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.12
** 描 述:       全局环境变量处理函数
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 属性(OO术语 :-))
 */
static char     _rootEnvStorage[256] = "";      /* 系统全局环境变量的根存储路径 */
static char     _envStorage[256] = "";          /* 系统全局环境变量存储路径 */


/*
 * 内部函数声明
 */
static int		_SetEnvImpl(const char*, const char*, int);
static char*	_GetEnvImpl(const char*);


/*****************************************************************
** 函数名:      InitEnv
** 输 入:       全局环境变量的存储位置
** 输 出:       0:  成功
**              -1: 失败
** 功能描述:    初始化全局环境变量的上下文环境
**              若要使用全局环境变量，则必须调用该函数，以初始化上下文环境
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      ResetEnv
** 输 入:       全局环境变量的存储位置
** 输 出:       0:  成功
**              -1: 失败
** 功能描述:    重设全局环境变量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      GetRootEnvStorage
** 输 入:       无
** 输 出:       无
** 功能描述:    返回系统全局环境变量的根存储路径
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetRootEnvStorage() {
    return _rootEnvStorage;
}


/*****************************************************************
** 函数名:      SetRootEnvStorage
** 输 入:       系统全局环境变量的根存储路径
** 输 出:       无
** 功能描述:    设置系统全局环境变量的根存储路径
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetRootEnvStorage(const char *rootStorage) {
    if (rootStorage != NULL && rootStorage != _rootEnvStorage) {
        strncpy(_rootEnvStorage, rootStorage, sizeof(_rootEnvStorage));
    }
}


/*****************************************************************
** 函数名:      GetEnvStorage
** 输 入:       无
** 输 出:       无
** 功能描述:    返回系统全局环境变量存储路径
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetEnvStorage() {
    return _envStorage;
}


/*****************************************************************
** 函数名:      SetEnvStorage
** 输 入:       系统全局环境变量存储路径
** 输 出:       无
** 功能描述:    设置系统全局环境变量存储路径
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetEnvStorage(const char *storage) {
    if (storage != NULL && storage != _envStorage) {
        strncpy(_envStorage, storage, sizeof(_envStorage));
    }
}


/*****************************************************************
** 函数名:      SetEnv
** 输 入:       name:      全局环境变量名称
**              value:     变量值
**              overwrite: 若name已存在是否使用value覆盖原来的变量值
**                  - 0:   不覆盖
**                  - <>0: 覆盖
** 输 出:       0:  成功
**              -1: 失败
**              1:  环境变量已存在(只有在overwrite==0的情况下返回)
** 功能描述:    设置全局环境变量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
SetEnv(const char *name, const char *value, int overwrite) {
    char    buf[256];

    sprintf(buf, "%s/%s", GetEnvStorage(), name);
    
	return _SetEnvImpl(buf, value, overwrite);
}


/*****************************************************************
** 函数名:      GetEnv
** 输 入:       name:  全局环境变量名称
** 输 出:       全局环境变量值, 若变量不存在则返回NULL
** 功能描述:    返回全局环境变量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2004.5.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetEnv(const char *name) {
    char        buf[256];

    sprintf(buf, "%s/%s", GetEnvStorage(), name);

    return _GetEnvImpl(buf);
}


/*****************************************************************
** 函数名:      SetEnvFully
** 输 入:       systemName: 系统名称
**              envName:    全局环境变量名称
**              value:     	变量值
**              overwrite: 	若name已存在是否使用value覆盖原来的变量值
**                  - 0:   	不覆盖
**                  - <>0: 	覆盖
** 输 出:       0:  成功
**              -1: 失败
**              1:  环境变量已存在(只有在overwrite==0的情况下返回)
** 功能描述:    设置全局环境变量（可指定系统名称）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
SetEnvFully(const char *systemName, const char *envName, const char *value, int overwrite) {
    char    buf[256];

    sprintf(buf, "%s/%s/%s", GetRootEnvStorage(), systemName, envName);

    return _SetEnvImpl(buf, value, overwrite);
}


/*****************************************************************
** 函数名:      GetEnvFully
** 输 入:       envName:  全局环境变量名称
** 输 出:       全局环境变量值, 若变量不存在则返回NULL
** 功能描述:    返回全局环境变量（可指定系统名称）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetEnvFully(const char *systemName, const char *envName) {
    char	buf[256];

    sprintf(buf, "%s/%s/%s", GetRootEnvStorage(), systemName, envName);

    return _GetEnvImpl(buf);
}


/*****************************************************************
** 函数名:      _SetEnvImpl
** 输 入:       env:        全局环境变量名称
**              value:     	变量值
**              overwrite: 	若name已存在是否使用value覆盖原来的变量值
**                  - 0:   	不覆盖
**                  - <>0: 	覆盖
** 输 出:       0:  成功
**              -1: 失败
**              1:  环境变量已存在(只有在overwrite==0的情况下返回)
** 功能描述:    设置全局环境变量（可指定系统名称）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      _GetEnvImpl
** 输 入:       env:  全局环境变量名称
** 输 出:       全局环境变量值, 若变量不存在则返回NULL
** 功能描述:    返回全局环境变量（可指定系统名称）
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.6
** 修 改:
** 日 期:
** 版本:        0.1
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
