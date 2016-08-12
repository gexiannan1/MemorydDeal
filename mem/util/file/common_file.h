/******************************************************************
** �ļ���:      common_file.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2004.5.13
** �� ��:       �ļ���������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ����ԭ��
 */
BOOL    	IsFileExist(const char*);               	/* ����ļ��Ƿ���� */
long    	GetFileLength(const char*);             	/* �����ļ����� */
struct tm*	GetFileTime(const char *fileName);			/* �����ļ��޸�ʱ�� */
char*		GetFormattedFileTime(const char *fileName);	/* �����ļ��޸�ʱ���ַ��� */
BOOL    	IsDir(const char*);                     	/* ����ļ������Ƿ���Ŀ¼ */
BOOL    	MkDir(const char*);                     	/* ����Ŀ¼ */
BOOL    	MvFile(const char*, const char*);       	/* �ƶ��ļ� */
BOOL    	MvDir(const char*, const char*);        	/* �ƶ��ļ��� */
BOOL    	RmFile(const char*);                    	/* ɾ���ļ� */
BOOL    	RmDir(const char*);                     	/* ɾ���ļ��� */
BOOL    	LnFile(const char*, const char*);       	/* �����ļ�Ӳ���� */


#endif  /* _COMMON_FILE_H */
