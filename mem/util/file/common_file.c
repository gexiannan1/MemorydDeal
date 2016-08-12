/******************************************************************
** 文件名:      common_file.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.13
** 描 述:       文件处理函数
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/file/common_file.c,v 1.1 2012/03/29 01:20:11 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:11 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <sys/wait.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <time.h>
#include    <sys/time.h>
#include    "common_file.h"
#include    "../time/common_time.h"
#include    "../string/common_str.h"
#include    "../log/log.h"


/*****************************************************************
** 函数名:      IsFileExist
** 输 入:       filePath:   文件路径
** 输 出:       TRUE:       文件存在
**              FALSE:      文件不存在
** 功能描述:    检查文件是否存在
** 全局变量:    无
** 调用模块:    无
** 创建人:      Deng Hua Wei
**              Song Fang
** 日 期:       2004.4.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsFileExist(const char *filePath) {
    return access(filePath, F_OK) < 0 ? FALSE : TRUE;
}


/*****************************************************************
** 函数名:      GetFileLength
** 输 入:       fileName:   文件名称
** 输 出:       文件长度
** 功能描述:    返回文件长度
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.29
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
long
GetFileLength(const char *fileName) {
    struct stat     f_stat;

    if (stat(fileName, &f_stat) < 0) {
        return -1;
    }
    return (long) f_stat.st_size;
}


/*****************************************************************
** 函数名:      GetFileTime
** 输 入:       fileName:   文件名称
** 输 出:       struct tm*, 文件修改时间
** 功能描述:    返回文件修改时间
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.24
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
struct tm*
GetFileTime(const char *fileName) {
    struct stat     	f_stat;
    static int          i = 0;
    static struct tm    tm[10];

    if (stat(fileName, &f_stat) < 0) {
        return NULL;
    }
    
    if (i >= 9) {
        i = 0;
    } else {
        i ++;
    }

    bzero(&tm[i], sizeof(struct tm));
    memcpy(&tm[i], localtime(&f_stat.st_mtime), sizeof(struct tm));
    
    return &tm[i];
}


/*****************************************************************
** 函数名:      GetFormattedFileTime
** 输 入:       fileName:   文件名称
** 输 出:       形如"yyyy-mm-dd hh:mm:ss"的十九位文件修改时间字符串
** 功能描述:    返回文件修改时间, 以形如"yyyy-mm-dd hh:mm:ss"的十九位时间戳字符串返回
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.24
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
char*
GetFormattedFileTime(const char *fileName) {
    return FormatTime(GetFileTime(fileName), TIME_FORMAT_FORMATTED_TIMESTAMP);
}


/*****************************************************************
** 函数名:      IsDir
** 输 入:       filePath:   文件路径
** 输 出:       TRUE:       文件存在
**              FALSE:      文件不存在
** 功能描述:    检查文件类型是否是目录
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
IsDir(const char *filePath) {
    struct stat     buf;

    if (lstat(filePath, &buf) < 0) {
        return FALSE;
    }

    return S_ISDIR(buf.st_mode) ? TRUE: FALSE;
}


/*****************************************************************
** 函数名:      MkDir
** 输 入:       dir:    目录路径
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    创建目录
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2004.4.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
MkDir(const char *dir) {
    char    cmd[256];

    if (!IsDir(dir)) {
        sprintf(cmd, "mkdir -p %s", dir);
        system(cmd);

        return IsDir(dir);
    }

    return TRUE;
}


/*****************************************************************
** 函数名:      MvFile
** 输 入:       source: 来源
**              target: 目的
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    移动文件
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
MvFile(const char *source, const char *target) {
    char    *index = NULL;
    char    cmd[256];
    char    targetDir[256];
    char    sourceDir[256];
    char    fileName[256];
    int     result = 0;

    /*
     * 检查目的目录是否存在，若不存在则创建之
     */
    if (!IsFileExist(target)) {
        index = strrchr((char *)target, '/');
        if (index) {
        	bzero(targetDir, sizeof(targetDir));
            strncpy(targetDir, target, index - target + 1);
            
            if (!MkDir(targetDir)) {
                return FALSE;
            }
        }
    }
    
    index = strrchr((char *)source, '/');
    if (!index) {
        return FALSE;
    }

	bzero(sourceDir, sizeof(sourceDir));
	bzero(fileName, sizeof(fileName));
	
    strncpy(sourceDir, source, index - source + 1);
    strcpy(fileName, index + 1);
    
    sprintf(cmd, "find \"%s\" -type f -name \"%s\" -print | xargs -i mv -f {} \"%s\"",
            sourceDir, fileName, target);

    result = system(cmd);
    Log("cmd:%s, result: %d", cmd, result);
    
    if (result < 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      MvDir
** 输 入:       source: 来源
**              target: 目的
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    移动文件夹
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
MvDir(const char *source, const char *target) {
    char    *index = NULL;
    char    cmd[256];
    char	tmp[256];
    char    root[128];
    char    dir[128];
    int     result = 0;

    /*
     * 检查目的目录是否存在，若不存在则创建之
     */
    if (!IsDir(target)) {
		strcpy(tmp, target);
		RtrimRude(tmp);
		if (tmp[strlen(tmp) - 1] == '/') {
			tmp[strlen(tmp) - 1] = '\0';
		}
		
		index = strrchr(tmp, '/');
		if (!index) {
			return FALSE;
		}
		
		bzero(root, sizeof(root));
		strncpy(root, tmp, index - tmp + 1);
		
		if (!MkDir(root)) {
			return FALSE;
		}
    }

    strcpy(tmp, source);
	TrimRude(tmp);
	if (tmp[strlen(tmp) - 1] == '/') {
		tmp[strlen(tmp) - 1] = '\0';
	}
	
    index = strrchr(tmp, '/');
    if (!index) {
        return FALSE;
    }

	bzero(root, sizeof(root));
	bzero(dir, sizeof(dir));
	
	strncpy(root, tmp, index - tmp + 1);
    strcpy(dir, index + 1);
	
    sprintf(cmd, "find \"%s\" -type d -name \"%s\" -print | xargs -i mv -f {} \"%s\"",
            root, dir, target);
    
    result = system(cmd);
    Log("cmd:%s, result: %d", cmd, result);
    
    if (result < 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      RmFile
** 输 入:       path:   待删除文件路径
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    删除文件
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.8
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
RmFile(const char *path) {
    char    *index = NULL;
    char    cmd[256];
    char    dir[128];
    char    fileName[256];
    int     result = 0;
    
    index = strrchr((char *)path, '/');
    if (!index) {
        return FALSE;
    }

	bzero(dir, sizeof(dir));
	bzero(fileName, sizeof(fileName));
	
    strncpy(dir, path, index - path + 1);
    strcpy(fileName, index + 1);
    
    sprintf(cmd, "find \"%s\" -type f -name \"%s\" -print | xargs -i rm -f {}",
            dir, fileName);

    result = system(cmd);
    Log("cmd:%s, result: %d", cmd, result);
    
    if (result != 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      RmDir
** 输 入:       path:   待删除文件夹路径
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    删除文件夹
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
RmDir(const char *path) {
    char    *index = NULL;
    char    cmd[256];
    char	tmp[256];
    char    root[128];
    char    dir[128];
    int     result = 0;
	
	strcpy(tmp, path);
	TrimRude(tmp);
	if (tmp[strlen(tmp) - 1] == '/') {
		tmp[strlen(tmp) - 1] = '\0';
	}
	
    index = strrchr(tmp, '/');
    if (!index) {
        return FALSE;
    }

	bzero(root, sizeof(root));
	bzero(dir, sizeof(dir));
	
	strncpy(root, tmp, index - tmp + 1);
    strcpy(dir, index + 1);
	
    sprintf(cmd, "find \"%s\" -type d -name \"%s\" -print | xargs -i rm -fr {}",
            root, dir);

    result = system(cmd);
    Log("cmd:%s, result: %d", cmd, result);
    
    if (result != 0) {
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************
** 函数名:      LnFile
** 输 入:       source: 来源
**              target: 目的
** 输 出:       TRUE:   成功
**              FALSE:  失败
** 功能描述:    建立文件硬链接
** 全局变量:    无
** 调用模块:    无
** 创建人:      Song Fang
** 日 期:       2005.1.14
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
BOOL
LnFile(const char *source, const char *target) {
    char    *index = NULL;
    char    cmd[256];
    char    targetDir[256];
    char    sourceDir[256];
    char    fileName[256];
    int     result = 0;

    /*
     * 检查目的目录是否存在，若不存在则创建之
     */
    if (!IsFileExist(target)) {
        index = strrchr((char *)target, '/');
        if (index) {
        	bzero(targetDir, sizeof(targetDir));
            strncpy(targetDir, target, index - target + 1);
            
            if (!MkDir(targetDir)) {
                return FALSE;
            }
        }
    }
    
    index = strrchr((char *)source, '/');
    if (!index) {
        return FALSE;
    }

	bzero(sourceDir, sizeof(sourceDir));
	bzero(fileName, sizeof(fileName));
	
    strncpy(sourceDir, source, index - source + 1);
    strcpy(fileName, index + 1);
    
    sprintf(cmd, "find \"%s\" -type f -name \"%s\" -print | xargs -i ln -f {} \"%s\"",
            sourceDir, fileName, target);

    result = system(cmd);
    Log("cmd:%s, result: %d", cmd, result);
    
    if (result != 0) {
        return FALSE;
    }
    return TRUE;
}
