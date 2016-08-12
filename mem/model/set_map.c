/******************************************************************
** 文件名:      set_map.c
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.9.11
** 描 述:       set对应映射区
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/set_map.c,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    "set_map.h"
#include	"../memory/memory.h"
#include    "../model/model.h"
#include    "../model/model_te.h"
#include    "../../util/log/log.h"


/*
 * 常量定义
 */


/*
 * 模块名称
 */
static const char   _moduleName[] = "set_map";


/*
 * 属性
 */
SET_ID_MAP*	_setIdMap = NULL;
SET_MAP*	_setMapHead	= NULL;


/*
 * 内部函数声明
 */
static SET_ID_MAP*		GetSetIdMapHead();					/* 获取 SET_ID_MAP 的首地址 */
static SET_STATS*		GetSetStatsHead();					/* 获取 SET_STATS 的首地址 */
static SET_MAP*			GetSetMapHead(int);					/* 获取指定 SET_MAP 块的首地址 */


/*****************************************************************
** 函数名:      InitSetMap
** 输 入:		无
** 输 出:       无
** 功能描述:    初始化SET_ID_MAP、SET_STATS
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void 
InitSetMap(){
	SET_ID_MAP* setIdMap	= NULL;
	SET_STATS*	setStats	= NULL;
	int			i			= 0;
	
	MEM_CONFIG_SHARE *memConfig = GetShareMemConfigStats();
	
	setIdMap = GetSetIdMapHead();
	for(i = 0; i < memConfig->setIdMapMax; i++, setIdMap++){
		setIdMap->firstSetOffset = -1;
		setIdMap->lastSetOffset = -1;
	}
	
	setStats = GetSetStatsHead();
	for(i = 0; i < memConfig->setMax; i++, setStats++){
		setStats->nextSetMap = -1;
		setStats->lastSetMember = -1;
	}
}

/*****************************************************************
** 函数名:      GetSetIdMap
** 输 入:		index
** 输 出:       指定位置的 SET_ID_MAP
** 功能描述:    获取指定位置的 SET_ID_MAP
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
SET_ID_MAP*	
GetSetIdMap(int index){
	if (index < 0 || index >= GetShareMemConfigStats()->setIdMapMax) {
        return NULL;
    }
	return GetSetIdMapHead() + index;
}


/*****************************************************************
** 函数名:      GetSetStats
** 输 入:		index
** 输 出:       指定位置的 SET_STATS
** 功能描述:    获取指定位置的 SET_STATS
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
SET_STATS*	
GetSetStats(int index){
	if (index < 0 || index >= GetShareMemConfigStats()->setMax) {
        return NULL;
    }
	return GetSetStatsHead() + index;
}


/*****************************************************************
** 函数名:      GetSetMap
** 输 入:		setid  tradeIndex
** 输 出:       对应位置的SET_MAP指针
** 功能描述:    根据setid和tradeindex获取对应的setmap
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
SET_MAP*	
GetSetMap(int setId, int tradeIndex){
	SET_ID_MAP* setIdMap = NULL;
	SET_STATS*	setStats = NULL;
	int			i		 = 0;
	int			setNum	 = 0;
	int			index	 = 0;
	int			setMapSize = GetShareMemConfigStats()->setSizeMax;
	
	setIdMap = GetSetIdMap(setId);
	if(setIdMap == NULL){
		return NULL;
	}
	
	if(setIdMap->firstSetOffset == -1){
		return NULL;
	}
	
	setStats = GetSetStats(setIdMap->firstSetOffset);
	setNum = tradeIndex / setMapSize;
	index = tradeIndex % setMapSize;
	for(i = 0; i < setNum; i++){
		setStats = GetSetStats(setStats->nextSetMap);
		if(setStats == NULL){
			return NULL;
		}
	}
	
	if(setStats->lastSetMember < index){
		return NULL;
	}
	
	return GetSetMapByIndex(setStats - GetSetStatsHead(), index);
}


/*****************************************************************
** 函数名:      GetNextSetMap
** 输 入:		SET_MAP*
** 输 出:       下一个trade序号的setmap
** 功能描述:    获得下一个trade序号的setmap
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.13
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
SET_MAP* 
GetNextSetMap(SET_MAP* setMap){
	int			setMapSize	 = GetShareMemConfigStats()->setSizeMax;
	SET_STATS*	setStats	 = NULL;
	SET_MAP*	setMapReturn = NULL;
	
	if(setMap == NULL){
		return NULL;
	}
	
	if((setMap->tradeIndex % setMapSize) != (setMapSize - 1)){
		setMapReturn = setMap + 1;
		if(setMapReturn->tradeIndex != setMap->tradeIndex + 1){
			return NULL;
		}
		return setMapReturn;
	}
	
	setStats = GetSetStats((setMap - GetSetMapHead(0)) / setMapSize);
	if(setStats->nextSetMap == -1){
		return NULL;
	}
	setMapReturn = GetSetMapHead(setStats->nextSetMap);
	if(setMapReturn->tradeIndex != setMap->tradeIndex + 1){
		return NULL;
	}
	return setMapReturn;
}


/*****************************************************************
** 函数名:      GetSetMapByIndex
** 输 入:		setStatsIndex  setMapIndex
** 输 出:       对应位置的SET_MAP指针
** 功能描述:    根据setStatsIndex和setMapIndex获取对应的setmap
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.12
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
SET_MAP*	
GetSetMapByIndex(int setStatsIndex, int setMapIndex){
	if (setStatsIndex < 0 || setStatsIndex >= GetShareMemConfigStats()->setMax) {
        return NULL;
    }
	return GetSetMapHead(setStatsIndex) + setMapIndex;
}


/*****************************************************************
** 函数名:      AddSetMap
** 输 入:		setId,TRADE_CS指针
** 输 出:       TRUE FALSE
** 功能描述:    添加set_map 到指定set共享内存中,并回填trade序号到tradeCS
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.12
** 修 改:       Wangyx
** 日 期:       2010.12.7
** 版本:        0.2
****************************************************************/
BOOL 
AddSetMap(int setId, TRADE_CS* tradeCS){
	SET_ID_MAP*	setIdMap = NULL;
	SET_STATS*	setStats = NULL;
	SET_MAP*	setMap	 = NULL;
	int			lastTradeIndex = 0;
	
	if(tradeCS == NULL){
		return FALSE;
	}
	
	setIdMap = GetSetIdMap(setId);
	if(setIdMap == NULL){
		return FALSE;
	}
	/*
	 * 插入的数据是第一个setMap数据
	 */
	if(setIdMap->lastSetOffset == -1){
		if(GetShareMemCurrentStats()->nextSet > GetShareMemConfigStats()->setMax){
			ERROR(_moduleName, "set块资源耗尽！");
			return FALSE;
		}
		setIdMap->firstSetOffset = setIdMap->lastSetOffset = GetShareMemCurrentStats()->nextSet;
		setStats = GetSetStats(setIdMap->lastSetOffset);        
        setMap = GetSetMapByIndex(setIdMap->lastSetOffset, 0);
        // wangyx modify 2010.12.07 >>>>
        // setMap空指针检查，程序不能中断，所以return true
		if (setMap == NULL) {
            ERROR(_moduleName, "获取setMap失败，setIdMap->lastSetOffset=[%d]", setIdMap->lastSetOffset);
            return TRUE;
		}
        // wangyx modify 2010.12.07 <<<<
        
		tradeCS->index = setMap->tradeIndex = 0;
		setMap->tradeProcessNo = tradeCS->tradeProcessNo;
		setStats->lastSetMember = 0;
		GetShareMemCurrentStats()->nextSet++;
		return TRUE;
	}
	
	setStats = GetSetStats(setIdMap->lastSetOffset);
    setMap = GetSetMapByIndex(setIdMap->lastSetOffset, setStats->lastSetMember);
    // wangyx modify 2010.12.07 >>>>
    if (setMap == NULL) {
        ERROR(_moduleName, "获取setMap失败，setIdMap->lastSetOffset=[%d], setStats->lastSetMember=[%d]", setIdMap->lastSetOffset, setStats->lastSetMember);
        return TRUE;
	}
    // wangyx modify 2010.12.07 <<<<
    
	lastTradeIndex = setMap->tradeIndex;
	/*
	 * 插入数据时，要插入的set块刚好全部用完了，需要获取一个新的set块
	 */
	if(setStats->lastSetMember == GetShareMemConfigStats()->setSizeMax - 1){
		if(GetShareMemCurrentStats()->nextSet > GetShareMemConfigStats()->setMax){
			ERROR(_moduleName, "set块共享内存耗尽！");
			return FALSE;
		}
		setStats->nextSetMap = setIdMap->lastSetOffset = GetShareMemCurrentStats()->nextSet;
		setStats = GetSetStats(setIdMap->lastSetOffset);
        setMap = GetSetMapByIndex(setIdMap->lastSetOffset, 0);
        // wangyx modify 2010.12.07 >>>>
		if (setMap == NULL) {
            ERROR(_moduleName, "获取setMap失败，setIdMap->lastSetOffset=[%d]", setIdMap->lastSetOffset);
            return TRUE;
		}        
        // wangyx modify 2010.12.07 <<<<
        
		tradeCS->index = setMap->tradeIndex = lastTradeIndex + 1;
		setMap->tradeProcessNo = tradeCS->tradeProcessNo;
		setStats->lastSetMember = 0;
		GetShareMemCurrentStats()->nextSet++;
		return TRUE;
	}
	/*
	 * 普通情况下数据插入
	 */
	setMap++;
	tradeCS->index = setMap->tradeIndex = lastTradeIndex + 1;
	setMap->tradeProcessNo = tradeCS->tradeProcessNo;
	setStats->lastSetMember++;
	
	return TRUE;
}


/*****************************************************************
** 函数名:      GetSetIdMapHead
** 输 入:		无
** 输 出:       SET_ID_MAP 的首地址
** 功能描述:    获取SET_ID_MAP 的首地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static SET_ID_MAP*	
GetSetIdMapHead(){
	return (SET_ID_MAP*)GetShareMemory() + GetShareMemAllocationStats()->setIdMapOffset;
}


/*****************************************************************
** 函数名:      GetSetIdMapHead
** 输 入:		无
** 输 出:       SET_STATS 的首地址
** 功能描述:    获取 SET_STATS 的首地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static SET_STATS* 
GetSetStatsHead(){
	return (SET_STATS*)GetShareMemory() + GetShareMemAllocationStats()->setStatsOffset;
}


/*****************************************************************
** 函数名:      GetSetIdMapHead
** 输 入:		setIndex
** 输 出:       指定 SET_MAP 块的首地址
** 功能描述:    获取指定 SET_MAP 块的首地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.11
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static SET_MAP* 
GetSetMapHead(int setIndex){
	SET_MAP* setMapHead = NULL;
	
	setMapHead = (SET_MAP*)GetShareMemory() + GetShareMemAllocationStats()->setMapOffset;
	
	return setMapHead + GetShareMemConfigStats()->setSizeMax * setIndex;
}
