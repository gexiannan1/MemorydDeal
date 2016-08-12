/******************************************************************
** �ļ���:      common_file.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.13
** �� ��:       �ļ�������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
** ������:      IsFileExist
** �� ��:       filePath:   �ļ�·��
** �� ��:       TRUE:       �ļ�����
**              FALSE:      �ļ�������
** ��������:    ����ļ��Ƿ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Deng Hua Wei
**              Song Fang
** �� ��:       2004.4.25
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
BOOL
IsFileExist(const char *filePath) {
    return access(filePath, F_OK) < 0 ? FALSE : TRUE;
}


/*****************************************************************
** ������:      GetFileLength
** �� ��:       fileName:   �ļ�����
** �� ��:       �ļ�����
** ��������:    �����ļ�����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.29
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetFileTime
** �� ��:       fileName:   �ļ�����
** �� ��:       struct tm*, �ļ��޸�ʱ��
** ��������:    �����ļ��޸�ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.24
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetFormattedFileTime
** �� ��:       fileName:   �ļ�����
** �� ��:       ����"yyyy-mm-dd hh:mm:ss"��ʮ��λ�ļ��޸�ʱ���ַ���
** ��������:    �����ļ��޸�ʱ��, ������"yyyy-mm-dd hh:mm:ss"��ʮ��λʱ����ַ�������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.24
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
char*
GetFormattedFileTime(const char *fileName) {
    return FormatTime(GetFileTime(fileName), TIME_FORMAT_FORMATTED_TIMESTAMP);
}


/*****************************************************************
** ������:      IsDir
** �� ��:       filePath:   �ļ�·��
** �� ��:       TRUE:       �ļ�����
**              FALSE:      �ļ�������
** ��������:    ����ļ������Ƿ���Ŀ¼
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.25
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      MkDir
** �� ��:       dir:    Ŀ¼·��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ����Ŀ¼
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2004.4.25
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      MvFile
** �� ��:       source: ��Դ
**              target: Ŀ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    �ƶ��ļ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.7
** �� ��:
** �� ��:
** �汾:        0.1
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
     * ���Ŀ��Ŀ¼�Ƿ���ڣ����������򴴽�֮
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
** ������:      MvDir
** �� ��:       source: ��Դ
**              target: Ŀ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    �ƶ��ļ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.12
** �� ��:
** �� ��:
** �汾:        0.1
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
     * ���Ŀ��Ŀ¼�Ƿ���ڣ����������򴴽�֮
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
** ������:      RmFile
** �� ��:       path:   ��ɾ���ļ�·��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ɾ���ļ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.8
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      RmDir
** �� ��:       path:   ��ɾ���ļ���·��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    ɾ���ļ���
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.9
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      LnFile
** �� ��:       source: ��Դ
**              target: Ŀ��
** �� ��:       TRUE:   �ɹ�
**              FALSE:  ʧ��
** ��������:    �����ļ�Ӳ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** ������:      Song Fang
** �� ��:       2005.1.14
** �� ��:
** �� ��:
** �汾:        0.1
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
     * ���Ŀ��Ŀ¼�Ƿ���ڣ����������򴴽�֮
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
