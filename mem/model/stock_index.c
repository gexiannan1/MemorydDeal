/******************************************************************
** �ļ���:      stock_index.c
** �� ��:       0.1
** ������:      Li Bo
** �� ��:       2006.8.30
** �� ��:       ��Ʊָ��������
** �޶���¼��
** �޶�����     �޶���      SIR         �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2011.5.10    wangyx      00016       ���ӽ�����ָ��
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_index.c,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include	"stock_index.h"
#include    "../memory/memory.h"
#include    "../../util/log/log.h"


/*
 * ��������
 */
 
/*
 * ģ������
 */
static const char   _moduleName[] = "stock_index";

/*
 * ����
 */


/*
 * �ڲ���������
 */
 
/*****************************************************************
** ������:      GetStockIndexHead
** �� ��:		��
** �� ��:       ��Ʊָ���洢�׵�ַ
** ��������:    ���ع�Ʊָ���洢�׵�ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.8.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_INDEX_CS* 
GetStockIndexHead(){
	return (STOCK_INDEX_CS*) GetShareMemory() + GetShareMemAllocationStats()->stockIndexOffset;
}


/*****************************************************************
** ������:      GetStockIndex
** �� ��:		index
** �� ��:       ��Ʊָ���洢��ַ
** ��������:    ����ָ��λ�õ�STOCK_INDEX_CS
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.8.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_INDEX_CS*		
GetStockIndex(int index){
	return GetStockIndexHead() + index;
}


/*****************************************************************
** ������:      GetStockIndexByIsinCod
** �� ��:		��Ʊ�������
** �� ��:       ��Ʊָ���洢��ַ
** ��������:    ���ع�Ʊָ���洢��ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.8.30
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_INDEX_CS* 
GetStockIndexByIsinCod(char* isinCod){
	int i = 0;
	STOCK_INDEX_CS* pStockIndex = NULL;
	
	pStockIndex = GetStockIndexHead();
	
	if(pStockIndex == NULL){
		return NULL;
	}
	
	for(i = 0; i < GetShareMemCurrentStats()->nextStockIndex; i++,pStockIndex++){
		if(strcmp(isinCod, pStockIndex->detailedIndex.isinCod) == 0){
			return pStockIndex;
		}
	}
	
	return NULL;
}

/*****************************************************************
** ������:      GetStockIndexNumber
** �� ��:       ��
** �� ��:       StockIndex����
** ��������:    ����StockIndex����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       wugb
** �� ��:       2006.9.5
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��

****************************************************************/
int
GetStockIndexNumber() {
    return GetShareMemCurrentStats()->nextStockIndex;
}  


/*****************************************************************
** ������:      AddStockIndexInfo
** �� ��:       ָ����Ϣ
** �� ��:       ��ӵ������ڴ��λ��
** ��������:    ���ָ����Ϣ�������ڴ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.4
** �� ��:       
** �� ��:       
** �汾:        0.1
****************************************************************/
int	
AddStockIndexInfo(STOCK_INFO* pStockIndexInfo){
	STOCK_INDEX_CS		*target = NULL;
    int         		index   = GetShareMemCurrentStats()->nextStockIndex;

    //����ڴ��Ƿ�Խ��
    if (index >= GetShareMemConfigStats()->stockIndexMax) {
        ERROR(_moduleName, "���ָ��isinCod=[%s]ʱʧ�ܣ�ָ����������Ԥ��������ָ�������� Current Number: %d"
                ,pStockIndexInfo->isinCod, index);
        return -1;
    }
    
    target = GetStockIndex(index);
    
    memcpy(target->detailedIndex.isinCod, pStockIndexInfo->isinCod, STOCK_ISIN_CODE_LENGTH);
    memcpy(target->indexCode, pStockIndexInfo->stockId, STOCK_ID_LENGTH);
    
    GetShareMemCurrentStats()->nextStockIndex ++;
	return index;
}


/*****************************************************************
** ������:      UpdateDetailedIndex
** �� ��:       ��Ҫ���µ�ָ����Ϣ
** �� ��:       TRUE FALSE
** ��������:    �����ڴ��и���ָ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.6
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��

****************************************************************/
BOOL 
UpdateDetailedIndex(DETAILED_INDEX* pStockIndex){
	STOCK_INDEX_CS	*target = NULL;
    
    if(pStockIndex == NULL){
    	ERROR(_moduleName, "UpdateDetailedIndex �����������ΪNULL");
    	return FALSE;	
    }
    
    target = GetStockIndexByIsinCod(pStockIndex->isinCod);
    
    if(target == NULL){
    	ERROR(_moduleName, "UpdateDetailedIndex δ�ҵ���Ӧ��ָ����Ϣ isinCod=[%s]", pStockIndex->isinCod);
    	return FALSE;
    }
    
    memcpy(&(target->detailedIndex), pStockIndex, sizeof(DETAILED_INDEX));
    /*target->detailedIndex.indexHigh /= ONE_HUNDRAD;
    target->detailedIndex.indexLow /= ONE_HUNDRAD;
    target->detailedIndex.latestIndex /= ONE_HUNDRAD;*/
    if(target->detailedIndex.latestClosingIndex < 0){
    	target->detailedIndex.latestClosingIndex = 0;
    }
    if(target->detailedIndex.openingIndex < 0){
    	target->detailedIndex.openingIndex = 0;
    }
    if(target->detailedIndex.tradeMoney < 0){
    	target->detailedIndex.tradeMoney = 0;
    }
    if(target->detailedIndex.indexHigh < 0){
    	target->detailedIndex.indexHigh = 0;
    }
    if(target->detailedIndex.indexLow < 0){
    	target->detailedIndex.indexLow = 0;
    }
    if(target->detailedIndex.latestIndex < 0){
    	target->detailedIndex.latestIndex = 0;
    }
/*** SIR 00016 BEGIN */	
	if(target->detailedIndex.closeIndex < 0 ){
		target->detailedIndex.closeIndex = 0;
	}
/*** SIR 00016 END */	
    if(target->detailedIndex.tradeAmount < 0){
    	target->detailedIndex.tradeAmount = 0;
    }
    if(target->detailedIndex.netChg == -9999999999999ll){
    	target->detailedIndex.netChg = 0;
    }
    if(target->detailedIndex.ratioChg == -9999999999999ll){
    	target->detailedIndex.ratioChg = 0;
    }

    return TRUE;
}



/*****************************************************************
** ������:      PrintStockIndex
** �� ��:       stockIndex
** �� ��:       ��
** ��������:    ���ָ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.8
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��

****************************************************************/
void 
PrintStockIndex(STOCK_INDEX_CS* stockIndex){
	if(stockIndex == NULL){
		return;
	}
	
	TRACE(_moduleName, "[STOCK_INDEX_CS]--isinCod=[%s]--indexCode=[%s]--latestClosingIndex=[%lld]--openingIndex=[%lld]--tradeMoney=[%lld]--indexHigh=[%lld]--indexLow=[%lld]--latestIndex=[%lld]--tradeTime=[%s]--tradeAmount=[%lld]--netChg=[%lld]--ratioChg=[%lld]", 
		stockIndex->detailedIndex.isinCod, 
		stockIndex->indexCode,
		stockIndex->detailedIndex.latestClosingIndex,
		stockIndex->detailedIndex.openingIndex,
		stockIndex->detailedIndex.tradeMoney,
		stockIndex->detailedIndex.indexHigh,
		stockIndex->detailedIndex.indexLow,
		stockIndex->detailedIndex.latestIndex,
		stockIndex->detailedIndex.tradeTime,
		stockIndex->detailedIndex.tradeAmount,
		stockIndex->detailedIndex.netChg,
		stockIndex->detailedIndex.ratioChg);
}



/*****************************************************************
** ������:      PrintStockIndexToFile
** �� ��:       fp
** �� ��:       ��
** ��������:    ���ָ����Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.12.8
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��

****************************************************************/
void 
PrintStockIndexToFile(int fp){
	char			buf[4096];
    STOCK_INDEX_CS*	stockIndex = NULL;
    int         	number = 0;
    int         	i = 0;

    number = GetStockIndexNumber();

    sprintf(buf, "#****ָ����Ϣ****\n#ָ������: %d\n\n", number);
    write(fp, buf, strlen(buf));
    
    for (i = 0; i < number; i++) {
        stockIndex = GetStockIndex(i);
		
		memset(buf, 0, 4096);
    	sprintf(buf, "isinCod=[%16s]--indexCode=[%16s]--latestClosingIndex=[%16lld]--openingIndex=[%16lld]--tradeMoney=[%16lld]--indexHigh=[%16lld]--indexLow=[%16lld]--latestIndex=[%16lld]--tradeTime=[%16s]--tradeAmount=[%16lld]--netChg=[%16lld]--ratioChg=[%16lld]\n", 
			stockIndex->detailedIndex.isinCod, 
			stockIndex->indexCode,
			stockIndex->detailedIndex.latestClosingIndex,
			stockIndex->detailedIndex.openingIndex,
			stockIndex->detailedIndex.tradeMoney,
			stockIndex->detailedIndex.indexHigh,
			stockIndex->detailedIndex.indexLow,
			stockIndex->detailedIndex.latestIndex,
			stockIndex->detailedIndex.tradeTime,
			stockIndex->detailedIndex.tradeAmount,
			stockIndex->detailedIndex.netChg,
			stockIndex->detailedIndex.ratioChg);
    	write(fp, buf, strlen(buf));
    }
}
