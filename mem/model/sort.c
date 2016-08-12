/******************************************************************
** 文件名:      sort.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       Stock信息定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
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
** 函数名:      GetAShareWithdrawBuySortList
** 输 入:       无
** 输 出:       WITHDRAW_BUY_SORT_LIST
** 功能描述:    返回A股单笔委托买入撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_BUY_SORT_LIST*
GetAShareWithdrawBuySortList() {
    return &GetMemTradeStats()->aWithdrawBuySortList;
}

/*****************************************************************
** 函数名:      GetBShareWithdrawBuySortList
** 输 入:       无
** 输 出:       WITHDRAW_BUY_SORT_LIST
** 功能描述:    返回B股单笔委托买入撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_BUY_SORT_LIST*
GetBShareWithdrawBuySortList() {
    return &GetMemTradeStats()->bWithdrawBuySortList;
}

/*****************************************************************
** 函数名:      GetAShareWithdrawSellSortList
** 输 入:       无
** 输 出:       WITHDRAW_SELL_SORT_LIST
** 功能描述:    返回A股单笔委托卖出撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_SELL_SORT_LIST*
GetAShareWithdrawSellSortList() {
    return &GetMemTradeStats()->aWithdrawSellSortList;
}


/*****************************************************************
** 函数名:      GetBShareWithdrawSellSortList
** 输 入:       无
** 输 出:       WITHDRAW_SELL_SORT_LIST
** 功能描述:    返回B股单笔委托卖出撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.15
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
WITHDRAW_SELL_SORT_LIST*
GetBShareWithdrawSellSortList() {
    return &GetMemTradeStats()->bWithdrawSellSortList;
}

/*****************************************************************
** 函数名:      GetShareOrderBuyRanking
** 输 入:       无
** 输 出:       无
** 功能描述:    排序后的股票委托买入排名列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 取得券种信息
     */
    category = GetCategory(shareType);

    /*
     * 计算要发送的数据笔数 (剔除无用的成交数据)
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
** 函数名:      GetShareOrderSellRanking
** 输 入:       无
** 输 出:       无
** 功能描述:    排序后的股票委托卖出排名列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.3
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 取得券种信息
     */
    category = GetCategory(shareType);

    /*
     * 计算要发送的数据笔数 (剔除无用的成交数据)
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
** 函数名:      GetShareWithdrawBuyRanking
** 输 入:       无
** 输 出:       无
** 功能描述:    排序后的股票累计委托买入撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.5
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 取得券种信息
     */
    category = GetCategory(shareType);

    /*
     * 计算要发送的数据笔数 (剔除无用的成交数据)
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
** 函数名:      GetShareWithdrawSellRanking
** 输 入:       无
** 输 出:       无
** 功能描述:    排序后的股票累计委托买出撤单排名
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.5
** 修 改:
** 日 期:
** 版本:        0.1
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
     * 取得券种信息
     */
    category = GetCategory(shareType);

    /*
     * 计算要发送的数据笔数 (剔除无用的成交数据)
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
** 函数名:      PrintShareOrderBuySortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印股票委托买入排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.20
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintShareOrderBuySortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i = 0;
    STOCK_CS    **sortList = NULL;
    	
    sortList = GetShareOrderBuyRanking(shareType);

    sprintf(buf, "\n#委托买入数量最多的前十只股票\n\n");
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
** 函数名:      PrintShareOrderSellSortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印股票委托卖出排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.3
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintShareOrderSellSortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i = 0;
    STOCK_CS    **sortList = NULL;

    sortList = GetShareOrderSellRanking(shareType);

    sprintf(buf, "\n#委托卖出数量最多的前十只股票\n\n");
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
** 函数名:      PrintShareWithdrawBuySortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印股票累计撤单排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.20
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintShareWithdrawBuySortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i           = 0;
    STOCK_CS    *stock      = NULL;
    STOCK_CS    **sortList  = NULL;

    sortList = GetShareWithdrawBuyRanking(shareType);

    sprintf(buf, "\n#撤销买入委托累计数量最多的前十只股票\n\n");
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
** 函数名:      PrintShareWithdrawSellSortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印股票累计撤单排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.20
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintShareWithdrawSellSortListToFile(int fp,int shareType) {
    char        buf[1024];
    int         i           = 0;
    STOCK_CS    *stock      = NULL;
    STOCK_CS    **sortList  = NULL;

    sortList = GetShareWithdrawSellRanking(shareType);

    sprintf(buf, "\n#撤销卖出委托累计数量最多的前十只股票\n\n");
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
** 函数名:      PrintWithdrawBuySortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印单笔撤单排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.20
** 修 改:
** 日 期:
** 版本:        0.1
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

    sprintf(buf, "\n#单笔委托数量最大的10笔委买撤单\n\n");
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
** 函数名:      PrintWithdrawSellSortListToFile
** 输 入:       fp  文件描述符
** 输 出:       无
** 功能描述:    打印单笔撤单排名信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.20
** 修 改:
** 日 期:
** 版本:        0.1
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

    sprintf(buf, "\n#单笔委托数量最大的10笔委卖撤单\n\n");
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
