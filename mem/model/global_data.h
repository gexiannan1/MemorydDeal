/******************************************************************
** 文件名:      global_data.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.18
** 描 述:       系统全局数据定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/global_data.h,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#ifndef _MODEL_GLOBAL_DATA_H
#define _MODEL_GLOBAL_DATA_H


#include    "../util/symbol.h"


/*
 * 系统全局数据
 */
typedef struct {
    int     multicastDelayTime;                         /* 组播分片延迟时间 */
} GLOBAL_DATA;
/* -------------------------           */


/*
 * 函数声明
 */
GLOBAL_DATA*	GetGlobalData();                        /* 返回系统全局数据结构 */

int				GetGlobalMulticastDelayTime();			/* 返回组播分片延迟时间 */
void			SetGlobalMulticastDelayTime(int);		/* 设置组播分片延迟时间 */

void     		PrintGlobalData();      				/* 打印系统全局数据 */


#endif  /* _MODEL_GLOBAL_DATA_H */
