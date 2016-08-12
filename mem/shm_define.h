/******************************************************************
** 文件名:      shm_define.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.30
** 描 述:       共享内存常量定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
 * 共享内存操作常量定义
 */
#define     SEM_MODE_RW     (0644)//(SEM_R | SEM_A | SEM_R>>3 | SEM_R>>6)
#define     SHM_MODE_RW     (0644)//(SHM_R | SHM_W | SHM_R>>3 | SHM_R>>6)
#define		SHM_FAILED		(void *)-1L


/*
 * 共享内存编号定义
 */
#define     CS_SHARE_MEMORY_ID                  1       /* 共享内存的编号 */
#define     CS_REBUILDER_SHARE_MEMORY_ID        2       /* 数据重传共享内存的编号 */


#endif  /* _CS_SHM_DEFINE_H */
