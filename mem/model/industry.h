/******************************************************************
** 文件名:      industry.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.9
** 描 述:       行业列表信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   增加返回单边模式的成交信息的接口
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/industry.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _MODEL_INDUSTRY_H
#define _MODEL_INDUSTRY_H


#include    "model.h"
#include    "model_mon.h"
#include    "../util/symbol.h"



/*
 * 结构体定义
 */

/*
 * 行业个股成交金额排名列表（行业成交金额前五位的个股）
 */
typedef struct {
    int     stockNo[INDUSTRY_STOCK_TRADE_SORT_LENGTH];
} INDUSTRY_SHARE_TRADE_SORT_LIST;
/* -------------------------           */


/*
 * 行业列表
 */
typedef struct {
    /*
     * 由网络监控子系统维护的信息
     * 为保障结构定义的直观, 未直接引入 INDUSTRY_MON 结构
     * <b>
     * 若 INDUSTRY_MON 结构发生改变, 需要同步修改该区段
     * </b>
     */
    int                             industryNo;                         /* 行业序号 */
    char                            industryId[INDUSTRY_ID_LENGTH];     /* 行业代码 */
    char                            industryName[INDUSTRY_NAME_LENGTH]; /* 行业名称 */
    /* -------------------------           */

    int                             tradeNumber;                        /* 该行业总成交笔数 */
    long long                       tradeAmount;                        /* 该行业总成交数量 */
    long long                       tradeMoney;                         /* 该行业总成交金额 */
    INDUSTRY_SHARE_TRADE_SORT_LIST  tradeSortList;                      /* 行业个股成交金额排名列表（行业成交金额前五位的个股） */

} INDUSTRY_CS;


/*
 * 函数声明
 */
INDUSTRY_CS*    GetIndustryList(int shareType);                 /* 返回Industry数组的起始位置 */
INDUSTRY_CS*    GetIndustry(int index,int shareType);           /* 返回指定位置的INDUSTRY_CS */
int             GetIndustryNumber(int shareType);               /* 返回Industry数量 */
int             AddIndustry(INDUSTRY_MON*,int shareType);                     /* 向Industry数组中增加Industry条目 */

INDUSTRY_SHARE_TRADE_SORT_LIST*
GetIndustryShareTradeSortList(int index,int shareType);                       /* 返回行业个股成交金额排名列表 */


void            InitSortedIndustryListA();          			/* 初始化排序后的Industry数组 */
void            InitSortedIndustryListB();          			/* 初始化排序后的Industry数组 */
INDUSTRY_CS*    GetSortedIndustry(int index,int shareType);     /* 返回排序后的指定位置的INDUSTRY_CS */

int             AddIndustryStock(char*, STOCK_CS*,int);             /* 向指定行业中添加所属证券信息 */

void            PrintIndustryInfoToFile(int fp,int shareType);                  				/* 打印行业信息到指定的文件中 */
void            PrintSortedIndustryInfoToFile(int fp,int shareType);            /* 打印排序后的行业信息到指定的文件中 */


#endif  /* _MODEL_INDUSTRY_H */
