/******************************************************************
** �ļ���:      shm_define.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.30
** �� ��:       �����ڴ泣������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/memory/shm_define.h,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#ifndef _CS_SHM_DEFINE_H
#define _CS_SHM_DEFINE_H


/*
 * �����ڴ������������
 */
#define     SEM_MODE_RW     (0644)//(SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)
#define     SHM_MODE_RW     (0644)//(SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)
#define		SHM_FAILED		(void *)-1L


/*
 * �����ڴ��Ŷ���
 */
#define     CS_SHARE_MEMORY_ID                  1       /* �����ڴ�ı�� */
#define     CS_REBUILDER_SHARE_MEMORY_ID        2       /* �����ش������ڴ�ı�� */


#endif  /* _CS_SHM_DEFINE_H */
