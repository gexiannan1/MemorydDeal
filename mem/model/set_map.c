/******************************************************************
** �ļ���:      set_map.c
** �� ��:       0.1
** ������:      Li Bo
** �� ��:       2006.9.11
** �� ��:       set��Ӧӳ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * ��������
 */


/*
 * ģ������
 */
static const char   _moduleName[] = "set_map";


/*
 * ����
 */
SET_ID_MAP*	_setIdMap = NULL;
SET_MAP*	_setMapHead	= NULL;


/*
 * �ڲ���������
 */
static SET_ID_MAP*		GetSetIdMapHead();					/* ��ȡ SET_ID_MAP ���׵�ַ */
static SET_STATS*		GetSetStatsHead();					/* ��ȡ SET_STATS ���׵�ַ */
static SET_MAP*			GetSetMapHead(int);					/* ��ȡָ�� SET_MAP ����׵�ַ */


/*****************************************************************
** ������:      InitSetMap
** �� ��:		��
** �� ��:       ��
** ��������:    ��ʼ��SET_ID_MAP��SET_STATS
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.12
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetSetIdMap
** �� ��:		index
** �� ��:       ָ��λ�õ� SET_ID_MAP
** ��������:    ��ȡָ��λ�õ� SET_ID_MAP
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
SET_ID_MAP*	
GetSetIdMap(int index){
	if (index < 0 || index >= GetShareMemConfigStats()->setIdMapMax) {
        return NULL;
    }
	return GetSetIdMapHead() + index;
}


/*****************************************************************
** ������:      GetSetStats
** �� ��:		index
** �� ��:       ָ��λ�õ� SET_STATS
** ��������:    ��ȡָ��λ�õ� SET_STATS
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
SET_STATS*	
GetSetStats(int index){
	if (index < 0 || index >= GetShareMemConfigStats()->setMax) {
        return NULL;
    }
	return GetSetStatsHead() + index;
}


/*****************************************************************
** ������:      GetSetMap
** �� ��:		setid  tradeIndex
** �� ��:       ��Ӧλ�õ�SET_MAPָ��
** ��������:    ����setid��tradeindex��ȡ��Ӧ��setmap
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetNextSetMap
** �� ��:		SET_MAP*
** �� ��:       ��һ��trade��ŵ�setmap
** ��������:    �����һ��trade��ŵ�setmap
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.13
** �� ��:
** �� ��:
** �汾:        0.1
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
** ������:      GetSetMapByIndex
** �� ��:		setStatsIndex  setMapIndex
** �� ��:       ��Ӧλ�õ�SET_MAPָ��
** ��������:    ����setStatsIndex��setMapIndex��ȡ��Ӧ��setmap
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.12
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
SET_MAP*	
GetSetMapByIndex(int setStatsIndex, int setMapIndex){
	if (setStatsIndex < 0 || setStatsIndex >= GetShareMemConfigStats()->setMax) {
        return NULL;
    }
	return GetSetMapHead(setStatsIndex) + setMapIndex;
}


/*****************************************************************
** ������:      AddSetMap
** �� ��:		setId,TRADE_CSָ��
** �� ��:       TRUE FALSE
** ��������:    ���set_map ��ָ��set�����ڴ���,������trade��ŵ�tradeCS
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.12
** �� ��:       Wangyx
** �� ��:       2010.12.7
** �汾:        0.2
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
	 * ����������ǵ�һ��setMap����
	 */
	if(setIdMap->lastSetOffset == -1){
		if(GetShareMemCurrentStats()->nextSet > GetShareMemConfigStats()->setMax){
			ERROR(_moduleName, "set����Դ�ľ���");
			return FALSE;
		}
		setIdMap->firstSetOffset = setIdMap->lastSetOffset = GetShareMemCurrentStats()->nextSet;
		setStats = GetSetStats(setIdMap->lastSetOffset);        
        setMap = GetSetMapByIndex(setIdMap->lastSetOffset, 0);
        // wangyx modify 2010.12.07 >>>>
        // setMap��ָ���飬�������жϣ�����return true
		if (setMap == NULL) {
            ERROR(_moduleName, "��ȡsetMapʧ�ܣ�setIdMap->lastSetOffset=[%d]", setIdMap->lastSetOffset);
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
        ERROR(_moduleName, "��ȡsetMapʧ�ܣ�setIdMap->lastSetOffset=[%d], setStats->lastSetMember=[%d]", setIdMap->lastSetOffset, setStats->lastSetMember);
        return TRUE;
	}
    // wangyx modify 2010.12.07 <<<<
    
	lastTradeIndex = setMap->tradeIndex;
	/*
	 * ��������ʱ��Ҫ�����set��պ�ȫ�������ˣ���Ҫ��ȡһ���µ�set��
	 */
	if(setStats->lastSetMember == GetShareMemConfigStats()->setSizeMax - 1){
		if(GetShareMemCurrentStats()->nextSet > GetShareMemConfigStats()->setMax){
			ERROR(_moduleName, "set�鹲���ڴ�ľ���");
			return FALSE;
		}
		setStats->nextSetMap = setIdMap->lastSetOffset = GetShareMemCurrentStats()->nextSet;
		setStats = GetSetStats(setIdMap->lastSetOffset);
        setMap = GetSetMapByIndex(setIdMap->lastSetOffset, 0);
        // wangyx modify 2010.12.07 >>>>
		if (setMap == NULL) {
            ERROR(_moduleName, "��ȡsetMapʧ�ܣ�setIdMap->lastSetOffset=[%d]", setIdMap->lastSetOffset);
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
	 * ��ͨ��������ݲ���
	 */
	setMap++;
	tradeCS->index = setMap->tradeIndex = lastTradeIndex + 1;
	setMap->tradeProcessNo = tradeCS->tradeProcessNo;
	setStats->lastSetMember++;
	
	return TRUE;
}


/*****************************************************************
** ������:      GetSetIdMapHead
** �� ��:		��
** �� ��:       SET_ID_MAP ���׵�ַ
** ��������:    ��ȡSET_ID_MAP ���׵�ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static SET_ID_MAP*	
GetSetIdMapHead(){
	return (SET_ID_MAP*)GetShareMemory() + GetShareMemAllocationStats()->setIdMapOffset;
}


/*****************************************************************
** ������:      GetSetIdMapHead
** �� ��:		��
** �� ��:       SET_STATS ���׵�ַ
** ��������:    ��ȡ SET_STATS ���׵�ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static SET_STATS* 
GetSetStatsHead(){
	return (SET_STATS*)GetShareMemory() + GetShareMemAllocationStats()->setStatsOffset;
}


/*****************************************************************
** ������:      GetSetIdMapHead
** �� ��:		setIndex
** �� ��:       ָ�� SET_MAP ����׵�ַ
** ��������:    ��ȡָ�� SET_MAP ����׵�ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static SET_MAP* 
GetSetMapHead(int setIndex){
	SET_MAP* setMapHead = NULL;
	
	setMapHead = (SET_MAP*)GetShareMemory() + GetShareMemAllocationStats()->setMapOffset;
	
	return setMapHead + GetShareMemConfigStats()->setSizeMax * setIndex;
}
