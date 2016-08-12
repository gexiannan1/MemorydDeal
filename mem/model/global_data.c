/******************************************************************
** 文件名:      global_data.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.10.18
** 描 述:       系统全局数据定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/global_data.c,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#include    <string.h>
#include    <stdio.h>
#include    "global_data.h"
#include    "model.h"
#include    "..//memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"


/*
 * 常量定义
 */
#define	DEFAULT_MULTICAST_DELAY_TIME	10000		/* 默认的组播分片延迟时间（微秒） */


/*****************************************************************
** 函数名:      GetGlobalData
** 输 入:
** 输 出:
** 功能描述:    返回 GLOBAL_DATA 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
GLOBAL_DATA*
GetGlobalData() {
    return &GetMemStats()->globalData;
}


/*****************************************************************
** 函数名:      GetGlobalMulticastDelayTime
** 输 入:
** 输 出:
** 功能描述:    返回组播分片延迟时间
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetGlobalMulticastDelayTime() {
	int multicastDelayTime = GetGlobalData()->multicastDelayTime;
	
	if (multicastDelayTime <= 0) {
		return DEFAULT_MULTICAST_DELAY_TIME;
	}
    return multicastDelayTime;
}


/*****************************************************************
** 函数名:      SetGlobalMulticastDelayTime
** 输 入:
** 输 出:
** 功能描述:    设置组播分片延迟时间
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.10.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
SetGlobalMulticastDelayTime(int delayTime) {
    GetGlobalData()->multicastDelayTime = delayTime;
}


/*****************************************************************
** 函数名:      PrintGlobalData
** 输 入:       无
** 输 出:       无
** 功能描述:    打印系统全局数据
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.18
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintGlobalData() {
	GLOBAL_DATA *globalData = GetGlobalData();
	
	printf("GLOBAL DATA: multicastDelayTime:    %d\n", 
			globalData->multicastDelayTime);
	
	printf("\n");
}
