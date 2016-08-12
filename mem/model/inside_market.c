/******************************************************************
** �ļ���:      inside_market.c
** �� ��:       0.1
** ������:      Li Bo
** �� ��:       2006.9.7
** �� ��:       �г�������Ϣ������
** �޶���¼��
** �޶�����     �޶���    SIR    �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2012.12.12   LIBO      00380  ȯ�ֿ�ָ����õ��½����˳�
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/inside_market.c,v 1.2 2012/12/12 03:27:22 tc\boli Exp $
 * $Revision: 1.2 $
 * $Date: 2012/12/12 03:27:22 $
 */

#include    <string.h>
#include	"inside_market.h"
#include	"model_length_define.h"
#include	"category.h"
#include    "../memory/memory.h"
#include    "../../util/log/log.h"



/*
 * ��������
 */
 
/*
 * ģ������
 */
static const char   _moduleName[] = "inside_market";

/*
 * ����
 */


/*
 * �ڲ���������
 */
 
/*****************************************************************
** ������:      UpdateInsideMarket
** �� ��:       ��Ҫ���µ��г�������Ϣ
** �� ��:       ���µ������ڴ��λ��
** ��������:    �����ڴ��и����г�������Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.6
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2009.11.26	Li Bo		
****************************************************************/
int 
UpdateInsideMarket(INSIDE_MARKET* pInsideMarket){
	STOCK_CS* target = NULL;
/****SIR 00380 BEGIN ****/
	CATEGORY_CS* pCategory = NULL;
/****SIR 00380 END   ****/
	

	target = GetStockByIsinCod(pInsideMarket->isinCod);

	if(target == NULL){
		ERROR(_moduleName, "�ӹ����ڴ���Stock��Ϣʧ��isinCod=[%s]", pInsideMarket->isinCod);
		return -1;
	}

	target->volaIndicator[0]		= pInsideMarket->volInd;
	if(pInsideMarket->openPrice > 0){
		target->openPrice = pInsideMarket->openPrice / ONE_HUNDRAD;
	}
	if(pInsideMarket->closePrice > 0){
		target->closePrice = pInsideMarket->closePrice / ONE_HUNDRAD;
		//2011.5.31 �������鲻�ٸ������¼�
		//target->lastPrice = target->closePrice;
	}
	if(pInsideMarket->prevClosingPrice > 0){
		target->prevClosingPrice = pInsideMarket->prevClosingPrice / ONE_HUNDRAD;
	}
	if(pInsideMarket->fixPrc > 0){
		target->fixPrc = pInsideMarket->fixPrc / ONE_HUNDRAD;
	}
	target->moiInd					= pInsideMarket->moiInd;
	target->bstOrdPrcInd			= pInsideMarket->bstOrdPrcInd;		
	target->openInterest			= pInsideMarket->openInterest;
	target->deriLstStlPrc			= pInsideMarket->deriLstStlPrc;

	/*
	 * 2006.11.26
	 * ���ӹ��������г�״̬��ȡ��������⼯�Ͼ��ۼ۸񲻴���0�����������⼯�Ͼ��ۼ۸�ֵΪ0
	 */
	memset(target->commonPrcsStsValCod, 0, 6);
	memcpy(target->commonPrcsStsValCod, pInsideMarket->prcsStsValCod, 6);
	if(pInsideMarket->virtualOpenPrice > 0){
		target->lstAuctPrc = pInsideMarket->virtualOpenPrice / ONE_HUNDRAD;
	}
	else{
		target->lstAuctPrc = 0;
	}
	
	target->lstAuctQty				= pInsideMarket->virtualMatchAmount;
	target->surplusQty				= pInsideMarket->virtualUnMatchAmount;
	target->surplusBidAskIndicator	= pInsideMarket->srpBidAskInd;
	
	if((pInsideMarket->highPrice > 0) && (target->highPrice < pInsideMarket->highPrice / ONE_HUNDRAD)){
		target->highPrice = pInsideMarket->highPrice / ONE_HUNDRAD;
	}
	if((target->lowPrice == 0) && (pInsideMarket->lowPrice > 0)){
		target->lowPrice = pInsideMarket->lowPrice / ONE_HUNDRAD;
	}
	else if((pInsideMarket->lowPrice > 0) && (target->lowPrice > pInsideMarket->lowPrice / ONE_HUNDRAD)){
		target->lowPrice = pInsideMarket->lowPrice / ONE_HUNDRAD;
	}
/****SIR 00380 BEGIN ****/	
	pCategory = GetCategory(target->categoryNo);
	if(pCategory)
	{
		if(pCategory->id == CATEGORY_ID_ETF){
			if(pInsideMarket->iopv > 0){
				target->iopv = pInsideMarket->iopv / ONE_HUNDRAD;
			}
		}
		else if (pCategory->id == CATEGORY_ID_WARRANT){
			target->totWarExQty = pInsideMarket->totWarExQty;
			target->totWarClQty = pInsideMarket->totWarClQty / ONE_HUNDRAD;
			target->totWarWrQty = pInsideMarket->totWarWrQty / ONE_HUNDRAD;
		}
	}
/****SIR 00380 END   ****/	
	/////////////////////////
	//PrintStock(target);
	/////////////////////////
	return target - GetStockList();
}
