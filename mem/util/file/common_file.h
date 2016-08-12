/******************************************************************
** 文件名:      common_file.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2004.5.13
** 描 述:       文件处理函数的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/util/file/common_file.h,v 1.1 2012/03/29 01:20:11 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:20:11 $
 */

#ifndef _COMMON_FILE_H
#define _COMMON_FILE_H


#include    "../symbol.h"


/*
 * 函数原型
 */
BOOL    	IsFileExist(const char*);               	/* 检查文件是否存在 */
long    	GetFileLength(const char*);             	/* 返回文件长度 */
struct tm*	GetFileTime(const char *fileName);			/* 返回文件修改时间 */
char*		GetFormattedFileTime(const char *fileName);	/* 返回文件修改时间字符串 */
BOOL    	IsDir(const char*);                     	/* 检查文件类型是否是目录 */
BOOL    	MkDir(const char*);                     	/* 创建目录 */
BOOL    	MvFile(const char*, const char*);       	/* 移动文件 */
BOOL    	MvDir(const char*, const char*);        	/* 移动文件夹 */
BOOL    	RmFile(const char*);                    	/* 删除文件 */
BOOL    	RmDir(const char*);                     	/* 删除文件夹 */
BOOL    	LnFile(const char*, const char*);       	/* 建立文件硬链接 */


#endif  /* _COMMON_FILE_H */
