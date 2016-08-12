/******************************************************************
** 文件名:      industry.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       Industry信息定义
** 修订记录：	
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   增加返回单边模式的成交信息的接口
** 2006.9.4     wugb        区分行业类型A、B股
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/industry.c,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include    "industry.h"
#include    "category.h"
#include    "model.h"
#include    "../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"

static const char   _moduleName[] = "industry";
/*
 * 常量定义
 */
#define MAX_INDUSTRY_LIST_LENGTH        100         /* 最大行业数 */


/*
 * 属性
 */
static int  _sortedIndustryListA[MAX_INDUSTRY_LIST_LENGTH];
static int  _sortedIndustryListB[MAX_INDUSTRY_LIST_LENGTH];


/*
 * 内部函数声明
 */
static void     InitIndustry(INDUSTRY_CS*); 


/*****************************************************************
** 函数名:      GetIndustryList
** 输 入:       无
** 输 出:       INDUSTRY_CS[]
** 功能描述:    返回Industry数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
INDUSTRY_CS*
GetIndustryList(int shareType) {
	
	if(shareType==CATEGORY_ID_ASHARE)
    	return (INDUSTRY_CS*) GetShareMemory() + GetShareMemAllocationStats()->aIndustryOffset;
    else
    	return (INDUSTRY_CS*) GetShareMemory() + GetShareMemAllocationStats()->bIndustryOffset;
}


/*****************************************************************
** 函数名:      GetIndustry
** 输 入:       无
** 输 出:       INDUSTRY_CS*
** 功能描述:    返回指定位置的INDUSTRY_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:		
** 日 期:		
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        传递shareType 行业类型A、B股
****************************************************************/
INDUSTRY_CS*
GetIndustry(int index,int shareType) {
	
    if (index < 0 || index >= GetShareMemConfigStats()->industryMax) {
        return NULL;
    }
    return GetIndustryList(shareType) + index;
}


/*****************************************************************
** 函数名:      GetIndustryNumber
** 输 入:       无
** 输 出:       Industry总数
** 功能描述:    返回Industry数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
int
GetIndustryNumber(int shareType) {
	
	if(shareType==CATEGORY_ID_ASHARE)
    	return GetShareMemCurrentStats()->nextAIndustry;
    else if(shareType==CATEGORY_ID_BSHARE)
    	return GetShareMemCurrentStats()->nextBIndustry;
    else
    	return -1;
    	
}  


/*****************************************************************
** 函数名:      AddIndustry
** 输 入:       无
** 输 出:       新增的Industry的位置
** 功能描述:    向Industry数组中增加Industry条目
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:		
** 日 期:		
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
int
AddIndustry(INDUSTRY_MON *industry,int shareType) {
    INDUSTRY_CS *target = NULL;
    int         index ;

	if(shareType==CATEGORY_ID_ASHARE)
    	index=GetShareMemCurrentStats()->nextAIndustry;
    else
    	index=GetShareMemCurrentStats()->nextBIndustry;
	/*
     * 检查内存是否越界
     */
    if (index >= GetShareMemConfigStats()->industryMax) {
    	FATAL("Industry.AddIndustry()", "行业数量超出预定义的最大行业数量！ Current Number: %d"
    			, index);
    	return -1;
    }

    target = GetIndustry(index,shareType);
    memcpy((char*) target, (char*) industry, sizeof(INDUSTRY_MON));
    InitIndustry(target);
	
	if(shareType==CATEGORY_ID_ASHARE)
    	GetShareMemCurrentStats()->nextAIndustry++;
    else if(shareType==CATEGORY_ID_BSHARE)
    	GetShareMemCurrentStats()->nextBIndustry++;
	
    return index;
}


/*****************************************************************
** 函数名:      GetIndustryShareTradeSortList
** 输 入:       无
** 输 出:       INDUSTRY_SHARE_TRADE_SORT_LIST
** 功能描述:    返回行业个股成交金额排名列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
INDUSTRY_SHARE_TRADE_SORT_LIST*
GetIndustryShareTradeSortList(int index,int shareType) {
    return &GetIndustry(index,shareType)->tradeSortList;
}


/*****************************************************************
** 函数名:      InitSortedIndustryListA
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化排序后的A股Industry数组
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.11
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
void
InitSortedIndustryListA() {
    int     i               = 0;
    int     j               = 0;
    int     k               = 0;
    int     industryNumber  = GetIndustryNumber(CATEGORY_ID_ASHARE);
    
    for (i = 0; i < industryNumber; i++) {
        k = i;
        for (j = 0; j < i; j++) {
            if (GetIndustry(i,CATEGORY_ID_ASHARE)->tradeMoney > GetSortedIndustry(j,CATEGORY_ID_ASHARE)->tradeMoney) {
                k = j;
                break;
            }
        }
        if (k != i) {
            for (j = i; j > k; j--) {
                _sortedIndustryListA[j] = _sortedIndustryListA[j - 1];
            }
        }
        _sortedIndustryListA[k] = i;
    }
}

/*****************************************************************
** 函数名:      InitSortedIndustryListB
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化排序后的B股Industry数组
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.11
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
void
InitSortedIndustryListB() {
    int     i               = 0;
    int     j               = 0;
    int     k               = 0;
    int     industryNumber  = GetIndustryNumber(CATEGORY_ID_BSHARE);
    
    for (i = 0; i < industryNumber; i++) {
        k = i;
        for (j = 0; j < i; j++) {
            if (GetIndustry(i,CATEGORY_ID_BSHARE)->tradeMoney > GetSortedIndustry(j,CATEGORY_ID_BSHARE)->tradeMoney) {
                k = j;
                break;
            }
        }
        if (k != i) {
            for (j = i; j > k; j--) {
                _sortedIndustryListB[j] = _sortedIndustryListB[j - 1];
            }
        }
        _sortedIndustryListB[k] = i;
    }
}

/*****************************************************************
** 函数名:      GetSortedIndustry
** 输 入:       无
** 输 出:       INDUSTRY_CS*
** 功能描述:    返回排序后的指定位置的INDUSTRY_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.11
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
INDUSTRY_CS*
GetSortedIndustry(int index,int shareType) {
	if(shareType==CATEGORY_ID_ASHARE)
    	return GetIndustry(_sortedIndustryListA[index],shareType);
    else
    	return GetIndustry(_sortedIndustryListB[index],shareType);
}


/*****************************************************************
** 函数名:      AddIndustryStock
** 输 入:       无
** 输 出:       Industry的位置
** 功能描述:    向指定行业中添加所属证券信息
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.13
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
int
AddIndustryStock(char *industryId, STOCK_CS *stock,int shareType) {
    int         i           = 0;
    INDUSTRY_CS *industry   = NULL;

    if (! industryId ) {
        return -1;
    }

    for (i = 0; i < GetIndustryNumber(shareType); i++) {
        industry = GetIndustry(i,shareType);
        if (strncmp(industry->industryId, industryId, INDUSTRY_ID_LENGTH) == 0) {
            stock->industryNo = industry->industryNo;
            return i;
        }
    }
    return -1;
}


/*****************************************************************
** 函数名:      InitIndustry
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化行业信息列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InitIndustry(INDUSTRY_CS *industry) {
    int i = 0;

    /*
     * 初始化排名列表
     */
    for (i = 0; i < INDUSTRY_STOCK_TRADE_SORT_LENGTH; i++) {
        industry->tradeSortList.stockNo[i] = -1;
    }
}


/*****************************************************************
** 函数名:      PrintIndustryInfoToFile
** 输 入:       fp 文件描述符
** 输 出:       无
** 功能描述:    打印行业信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       weihy
** 日 期:       2005.9.11
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
** 2007.3.16    Li Bo		成交金额区分A、B股
****************************************************************/
void
PrintIndustryInfoToFile(int fp,int shareType) {
    INDUSTRY_CS    *industry    = NULL;
    char            buf[1024];
    int             number      = 0;
    int             i           = 0;

    number = GetIndustryNumber(shareType);

	if(shareType == CATEGORY_ID_ASHARE){
		sprintf(buf, "#****A股行业信息****\n#行业总数: %d\nA股市场总成交金额（双边）: %lld\n\n",
            number, GetMemTradeStats()->tradeMoney);
    	write(fp, buf, strlen(buf));
	}
	else if(shareType == CATEGORY_ID_BSHARE){
		sprintf(buf, "#****B股行业信息****\n#行业总数: %d\nB股市场总成交金额（双边）: %lld\n\n",
            number, GetMemTradeStats()->bTradeMoney);
    	write(fp, buf, strlen(buf));
	}
    

    for (i = 0; i < number; i++) {
        industry = GetIndustry(i,shareType);

        sprintf(buf, "index: %-4d\t" \
                "industryNo: %-4d\t" \
                "industryId: %-4s\t" \
                "industryName: %-16s\t" \
                "tradeNumber: %-8d\t" \
                "tradeAmount: %-8lld\t" \
                "tradeMoney: %-8lld\n"
                , i
                , industry->industryNo
                , industry->industryId
                , industry->industryName
                , industry->tradeNumber
                , industry->tradeAmount
                , industry->tradeMoney);

        write(fp, buf, strlen(buf));
    }
}


/*****************************************************************
** 函数名:      PrintSortedIndustryInfoToFile
** 输 入:       fp 文件描述符
** 输 出:       无
** 功能描述:    打印排序后的行业信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.4
** 修 改:
** 日 期:
** 版本:        0.1
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006.9.4     wugb        区分行业类型A、B股
****************************************************************/
void
PrintSortedIndustryInfoToFile(int fp,int shareType) {
    INDUSTRY_CS     *industry   = NULL;
    STOCK_CS        *stock      = NULL;
    char            buf[1024];
    char            buf2[256];
    int             number      = 0;
    int             i           = 0;
    int             j           = 0;

    /*
     * 初始化排序后的Industry数组
     */

	if(shareType==CATEGORY_ID_ASHARE)
    	InitSortedIndustryListA();
    else
    	InitSortedIndustryListB();
    
    number = GetIndustryNumber(shareType);

    sprintf(buf, "#****行业排名信息****\n#行业总数: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        industry = GetSortedIndustry(i,shareType);

        sprintf(buf, "index: %-4d\t" \
                "industryNo: %-4d\t" \
                "industryId: %-4s\t" \
                "industryName: %-16s\t" \
                "tradeNumber: %-8d\t" \
                "tradeAmount: %-8lld\t" \
                "tradeMoney: %-8lld\n"
                , i
                , industry->industryNo
                , industry->industryId
                , industry->industryName
                , industry->tradeNumber
                , industry->tradeAmount
                , industry->tradeMoney);

        for (j = 0; j < INDUSTRY_STOCK_TRADE_SORT_LENGTH; j++) {
            if ((stock = GetStock(industry->tradeSortList.stockNo[j]))) {
                sprintf(buf2, "\t----[%d]  stockNo: %-4d,\tstockId: %-8s,\ttradeMoney: %-8lld\n",
                        j, stock->stockNo, stock->stockId, stock->tradeMoney);
                strcat(buf, buf2);
            } else {
                sprintf(buf2, "\t----[%d]  stockNo: %-4d\n",
                        j, industry->tradeSortList.stockNo[j]);
                strcat(buf, buf2);
            }
        }

        write(fp, buf, strlen(buf));
    }
}
