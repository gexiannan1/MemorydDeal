/******************************************************************
** �ļ���:      sort.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       Stock��Ϣ����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/sort.c,v 1.1 2012/03/29 01:19:53 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:53 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include    "sort.h"
#include    "model.h"
#include    "../memory/memory.h"
#include    "../../util/symbol.h"


/*****************************************************************
** ������:      GetAShareWithdrawBuySortList
** �� ��:       ��
** �� ��:       WITHDRAW_BUY_SORT_LIST
** ��������:    ����A�ɵ���ί�����볷������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_BUY_SORT_LIST*
GetAShareWithdrawBuySortList() {
    return &GetMemTradeStats()->aWithdrawBuySortList;
}

/*****************************************************************
** ������:      GetBShareWithdrawBuySortList
** �� ��:       ��
** �� ��:       WITHDRAW_BUY_SORT_LIST
** ��������:    ����B�ɵ���ί�����볷������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_BUY_SORT_LIST*
GetBShareWithdrawBuySortList() {
    return &GetMemTradeStats()->bWithdrawBuySortList;
}

/*****************************************************************
** ������:      GetAShareWithdrawSellSortList
** �� ��:       ��
** �� ��:       WITHDRAW_SELL_SORT_LIST
** ��������:    ����A�ɵ���ί��������������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_SELL_SORT_LIST*
GetAShareWithdrawSellSortList() {
    return &GetMemTradeStats()->aWithdrawSellSortList;
}


/*****************************************************************
** ������:      GetBShareWithdrawSellSortList
** �� ��:       ��
** �� ��:       WITHDRAW_SELL_SORT_LIST
** ��������:    ����B�ɵ���ί��������������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.15
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
WITHDRAW_SELL_SORT_LIST*
GetBShareWithdrawSellSortList() {
    return &GetMemTradeStats()->bWithdrawSellSortList;
}

/*****************************************************************
** ������:      GetShareOrderBuyRanking
** �� ��:       ��
** �� ��:       ��
** ��������:    �����Ĺ�Ʊί�����������б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS**
GetShareOrderBuyRanking(int shareType) {
    static STOCK_CS     *sortedList[SHARE_ORDER_SORT_LENGTH];

    int                 i               = 0;
    int                 j               = 0;
    int                 k               = 0;
    STOCK_CS            *stock          = NULL;
    CATEGORY_CS         *category       = NULL;

    memset(sortedList, 0, sizeof(sortedList));
    
    /*
     * ȡ��ȯ����Ϣ
     */
    category = GetCategory(shareType);

    /*
     * ����Ҫ���͵����ݱ��� (�޳����õĳɽ�����)
     */
    if (category) {
        stock = GetStock(category->firstStock);

        for (i = 0; i < category->securitiesNumber && stock; i++) {
        	
        	if(!CheckTradePhase(stock)){
        		stock = GetStock(stock->nextCategory);
				continue;
        	}
        	
        	if ( stock->orderBuyAmount <= 0) {
        		stock = GetStock(stock->nextCategory);
				continue;
        	}

        	if (sortedList[SHARE_ORDER_SORT_LENGTH - 1] && stock->orderBuyAmount
                	<= sortedList[SHARE_ORDER_SORT_LENGTH - 1]->orderBuyAmount) {
                stock = GetStock(stock->nextCategory);
            	continue;
        	}

        	for (j = 0; j < SHARE_ORDER_SORT_LENGTH; j++) {
            	if (!sortedList[j]) {
                	sortedList[j] = stock;
                	break;
            	} else if (stock->orderBuyAmount > sortedList[j]->orderBuyAmount) {
                	for (k = SHARE_ORDER_SORT_LENGTH - 1; k > j; k--) {
                    	sortedList[k] = sortedList[k - 1];
                	}
                	sortedList[j] = stock;
                	break;
            	}
        	}
        	stock = GetStock(stock->nextCategory);
    	}
    }

    return sortedList;
}


/*****************************************************************
** ������:      GetShareOrderSellRanking
** �� ��:       ��
** �� ��:       ��
** ��������:    �����Ĺ�Ʊί�����������б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.3
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS**
GetShareOrderSellRanking(int shareType) {
    int                 i               = 0;
    int                 j               = 0;
    int                 k               = 0;
    STOCK_CS            *stock          = NULL;
 	CATEGORY_CS         *category       = NULL;
 	static STOCK_CS     *sortedList[SHARE_ORDER_SORT_LENGTH];

    memset(sortedList, 0, sizeof(sortedList));
    
    /*
     * ȡ��ȯ����Ϣ
     */
    category = GetCategory(shareType);

    /*
     * ����Ҫ���͵����ݱ��� (�޳����õĳɽ�����)
     */
    if (category) {
        stock = GetStock(category->firstStock);
        for (i = 0; i < category->securitiesNumber && stock; i++) {
			
			if(!CheckTradePhase(stock)){
        		stock = GetStock(stock->nextCategory);
				continue;
        	}
			
        	if (stock->orderSellAmount <= 0) {
        		stock = GetStock(stock->nextCategory);
            	continue;
       	 	}

        	if (sortedList[SHARE_ORDER_SORT_LENGTH - 1] && stock->orderSellAmount
                	<= sortedList[SHARE_ORDER_SORT_LENGTH - 1]->orderSellAmount) {
            	stock = GetStock(stock->nextCategory);
            	continue;
       	 	}

        	for (j = 0; j < SHARE_ORDER_SORT_LENGTH; j++) {
            	if (!sortedList[j]) {
                	sortedList[j] = stock;
                	break;
            	} else if (stock->orderSellAmount > sortedList[j]->orderSellAmount) {
                	for (k = SHARE_ORDER_SORT_LENGTH - 1; k > j; k--) {
                    	sortedList[k] = sortedList[k - 1];
                	}
                	sortedList[j] = stock;
                	break;
            	}
        	}
        	stock = GetStock(stock->nextCategory);
    	}
    }

    return sortedList;
}


/*****************************************************************
** ������:      GetShareWithdrawBuyRanking
** �� ��:       ��
** �� ��:       ��
** ��������:    �����Ĺ�Ʊ�ۼ�ί�����볷������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.5
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS**
GetShareWithdrawBuyRanking(int shareType) {
    int                 i               = 0;
    int                 j               = 0;
    int                 k               = 0;
	STOCK_CS            *stock          = NULL;
    CATEGORY_CS         *category       = NULL;
    static STOCK_CS     *sortedList[SHARE_WITHDRAW_SORT_LENGTH];

    memset(sortedList, 0, sizeof(sortedList));
    
    /*
     * ȡ��ȯ����Ϣ
     */
    category = GetCategory(shareType);

    /*
     * ����Ҫ���͵����ݱ��� (�޳����õĳɽ�����)
     */
    if (category) {
        stock = GetStock(category->firstStock);
        for (i = 0; i < category->securitiesNumber && stock; i++) {
        	
        	if (stock->withdrawBuyAmount <= 0) {
        		stock = GetStock(stock->nextCategory);
            	continue;
        	}

        	if (sortedList[SHARE_WITHDRAW_SORT_LENGTH - 1] && stock->withdrawBuyAmount
                	<= sortedList[SHARE_WITHDRAW_SORT_LENGTH - 1]->withdrawBuyAmount) {
                stock = GetStock(stock->nextCategory);
            	continue;
       	 	}

        	for (j = 0; j < SHARE_WITHDRAW_SORT_LENGTH; j++) {
            	if (!sortedList[j]) {
                	sortedList[j] = stock;
                	break;
            	} else if (stock->withdrawBuyAmount > sortedList[j]->withdrawBuyAmount) {
                	for (k = SHARE_WITHDRAW_SORT_LENGTH - 1; k > j; k--) {
                    	sortedList[k] = sortedList[k - 1];
                	}
                	sortedList[j] = stock;
               	 break;
            	}
        	}
        	stock = GetStock(stock->nextCategory);
    	}
    }

    return sortedList;
}


/*****************************************************************
** ������:      GetShareWithdrawSellRanking
** �� ��:       ��
** �� ��:       ��
** ��������:    �����Ĺ�Ʊ�ۼ�ί�������������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.5
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
STOCK_CS**
GetShareWithdrawSellRanking(int shareType) {
    int                 i               = 0;
    int                 j               = 0;
    int                 k               = 0;
    STOCK_CS            *stock          = NULL;
	CATEGORY_CS         *category       = NULL;
	static STOCK_CS     *sortedList[SHARE_WITHDRAW_SORT_LENGTH];

    memset(sortedList, 0, sizeof(sortedList));
    
    /*
     * ȡ��ȯ����Ϣ
     */
    category = GetCategory(shareType);

    /*
     * ����Ҫ���͵����ݱ��� (�޳����õĳɽ�����)
     */
    if (category) {
        stock = GetStock(category->firstStock);
        
        for (i = 0; i < category->securitiesNumber && stock; i++) {
        	
        	if ( stock->withdrawSellAmount <= 0) {
        		stock = GetStock(stock->nextCategory);
            	continue;
        	}

        	if (sortedList[SHARE_WITHDRAW_SORT_LENGTH - 1] && stock->withdrawSellAmount
                	<= sortedList[SHARE_WITHDRAW_SORT_LENGTH - 1]->withdrawSellAmount) {
                		stock = GetStock(stock->nextCategory);
            	continue;
        	}

        	for (j = 0; j < SHARE_WITHDRAW_SORT_LENGTH; j++) {
            	if (!sortedList[j]) {
                	sortedList[j] = stock;
                	break;
            	} else if (stock->withdrawSellAmount > sortedList[j]->withdrawSellAmount) {
                	for (k = SHARE_WITHDRAW_SORT_LENGTH - 1; k > j; k--) {
                    	sortedList[k] = sortedList[k - 1];
                	}
                	sortedList[j] = stock;
                	break;
            	}
        	}
        	stock = GetStock(stock->nextCategory);
    	}
	}
    return sortedList;
}


/*****************************************************************
** ������:      PrintShareOrderBuySortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��Ʊί������������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintShareOrderBuySortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i = 0;
    STOCK_CS    **sortList = NULL;
    	
    sortList = GetShareOrderBuyRanking(shareType);

    sprintf(buf, "\n#ί��������������ǰʮֻ��Ʊ\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < SHARE_ORDER_SORT_LENGTH; i++) {
        if (!sortList[i]) {
            break;
        }

        sprintf(buf, "index: %-4d\t" \
                "stockNo: %-4d\t" \
                "stockId: %-8s\t" \
                "stockShortName: %-8s\t" \
                "orderBuyAmount: %lld\n",
                i,
                sortList[i]->stockNo,
                sortList[i]->stockId,
                sortList[i]->stockShortName,
                sortList[i]->orderBuyAmount);
        write(fp, buf, strlen(buf));
    }
   
}


/*****************************************************************
** ������:      PrintShareOrderSellSortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��Ʊί������������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.3
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintShareOrderSellSortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i = 0;
    STOCK_CS    **sortList = NULL;

    sortList = GetShareOrderSellRanking(shareType);

    sprintf(buf, "\n#ί��������������ǰʮֻ��Ʊ\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < SHARE_ORDER_SORT_LENGTH; i++) {
        if (!sortList[i]) {
            break;
        }

        sprintf(buf, "index: %-4d\t" \
                "stockNo: %-4d\t" \
                "stockId: %-4s\t" \
                "stockName: %-8s\t" \
                "orderSellAmount: %lld\n",
                i,
                sortList[i]->stockNo,
                sortList[i]->stockId,
                sortList[i]->stockShortName,
                sortList[i]->orderSellAmount);
        write(fp, buf, strlen(buf));
    }
    
}


/*****************************************************************
** ������:      PrintShareWithdrawBuySortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��Ʊ�ۼƳ���������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintShareWithdrawBuySortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i           = 0;
    STOCK_CS    *stock      = NULL;
    STOCK_CS    **sortList  = NULL;

    sortList = GetShareWithdrawBuyRanking(shareType);

    sprintf(buf, "\n#��������ί���ۼ���������ǰʮֻ��Ʊ\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < SHARE_WITHDRAW_SORT_LENGTH; i++) {
        if (! (stock = sortList[i])) {
            break;
        }

        sprintf(buf, "index: %-4d\t" \
                "stockNo: %-4d\t" \
                "stockId: %-4s\t" \
                "withdrawBuyAmount: %lld\n",
                i,
                stock->stockNo,
                stock->stockId,
                stock->withdrawBuyAmount);
        write(fp, buf, strlen(buf));
    }

}


/*****************************************************************
** ������:      PrintShareWithdrawSellSortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��Ʊ�ۼƳ���������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintShareWithdrawSellSortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i           = 0;
    STOCK_CS    *stock      = NULL;
    STOCK_CS    **sortList  = NULL;

    sortList = GetShareWithdrawSellRanking(shareType);

    sprintf(buf, "\n#��������ί���ۼ���������ǰʮֻ��Ʊ\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < SHARE_WITHDRAW_SORT_LENGTH; i++) {
        if (! (stock = sortList[i])) {
            break;
        }

        sprintf(buf, "index: %-4d\t" \
                "stockNo: %-4d\t" \
                "stockId: %-4s\t" \
                "withdrawBuyAmount: %lld\n",
                i,
                stock->stockNo,
                stock->stockId,
                stock->withdrawSellAmount);
        write(fp, buf, strlen(buf));
    }
  

}


/*****************************************************************
** ������:      PrintWithdrawBuySortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ���ʳ���������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintWithdrawBuySortListToFile(int fp,int shareType) {
    char                    buf[1024];
    int                     i            = 0;
    WITHDRAW_CS             *withdraw    = NULL;
    ORDER_CS                *order       = NULL;
    STOCK_CS                *stock       = NULL;
    WITHDRAW_BUY_SORT_LIST  *sortList    = NULL;

    if (shareType == CATEGORY_ID_ASHARE){
		sortList = GetAShareWithdrawBuySortList();
	} else if(shareType == CATEGORY_ID_BSHARE){
		sortList = GetBShareWithdrawBuySortList();
	}

    sprintf(buf, "\n#����ί����������10��ί�򳷵�\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < WITHDRAW_SORT_LENGTH; i++) {
        if ( (withdraw = GetWithdraw(sortList->withdrawNo[i]))
                && (order = GetOrder(withdraw->orderProcessNo))
                && (stock = GetStock(order->stockNo)) ) {
            sprintf(buf, "index: %-16d\t" \
                    "withdrawNo: %-16d\t" \
                    "orderNo: %-16lld\t" \
                    "withdrawQty: %-16lld\t" \
                    "stockId: %-9s\t" \
                    "orderTime: %-12d\t" \
                    "orderAmount: %-16lld\t" \
                    "orderPrice: %-16lld\n",
                    i,
                    sortList->withdrawNo[i],
                    withdraw->orderNo,
                    withdraw->qty,
                    stock->stockId,
                    order->orderTime,
                    order->orderQty,
                    order->orderPrice);
        } else {
            sprintf(buf, "index: %-4d\t" \
                    "withdrawNo: %d\n",
                    i,
                    sortList->withdrawNo[i]);
        }

        write(fp, buf, strlen(buf));
    }
    

}


/*****************************************************************
** ������:      PrintWithdrawSellSortListToFile
** �� ��:       fp  �ļ�������
** �� ��:       ��
** ��������:    ��ӡ���ʳ���������Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.20
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintWithdrawSellSortListToFile(int fp,int shareType) {
    char                        buf[1024];
    int                         i            = 0;
    WITHDRAW_CS                 *withdraw    = NULL;
    ORDER_CS                    *order       = NULL;
    STOCK_CS                    *stock       = NULL;
    WITHDRAW_SELL_SORT_LIST     *sortList    = NULL;

    if (shareType == CATEGORY_ID_ASHARE){
		sortList = GetAShareWithdrawSellSortList();
	} else if(shareType == CATEGORY_ID_BSHARE){
		sortList = GetBShareWithdrawSellSortList();
	}

    sprintf(buf, "\n#����ί����������10��ί������\n\n");
    write(fp, buf, strlen(buf));

    for (i = 0; i < WITHDRAW_SORT_LENGTH; i++) {
        if ( (withdraw = GetWithdraw(sortList->withdrawNo[i]))
                && (order = GetOrder(withdraw->orderProcessNo))
                && (stock = GetStock(order->stockNo)) ) {
            sprintf(buf, "index: %-16d\t" \
                    "withdrawNo: %-16d\t" \
                    "orderNo: %-16lld\t" \
                    "withdrawQty: %-16lld\t" \
                    "stockId: %-9s\t" \
                    "orderTime: %-12d\t" \
                    "orderAmount: %-16lld\t" \
                    "orderPrice: %-16lld\n",
                    i,
                    sortList->withdrawNo[i],
                    withdraw->orderNo,
                    withdraw->qty,
                    stock->stockId,
                    order->orderTime,
                    order->orderQty,
                    order->orderPrice);
        } else {
            sprintf(buf, "index: %-4d\t" \
                    "withdrawNo: %d\n",
                    i,
                    sortList->withdrawNo[i]);
        }

        write(fp, buf, strlen(buf));
    }
    
}
