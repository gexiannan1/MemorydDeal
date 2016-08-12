/******************************************************************
** 文件名:      category.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.9
** 描 述:       证券品种列表信息定义的头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/category.h,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#ifndef _MODEL_CATEGORY_H
#define _MODEL_CATEGORY_H


#include    "model.h"
#include    "model_mon.h"
#include    "../util/symbol.h"


/*
 * 品种代码定义
 */
#define     CATEGORY_ID_ASHARE              0       /* 品种代码-A股 */
#define     CATEGORY_ID_BSHARE              1       /* 品种代码-B股 */
#define     CATEGORY_ID_CLOSEDEND_FUND      2       /* 品种代码-封闭式基金 */
#define     CATEGORY_ID_ETF                 3       /* 品种代码-ETF */
#define     CATEGORY_ID_OPENED_FUND         4       /* 品种代码-开放式基金 */
#define     CATEGORY_ID_MONETARY_FUND       5       /* 品种代码-货币基金 */
#define     CATEGORY_ID_WARRANT             6       /* 品种代码-权证 */
#define     CATEGORY_ID_BOND                7       /* 品种代码-普通债券 */
#define     CATEGORY_ID_CONVERTIBLE         8       /* 品种代码-可转换债券 */
#define     CATEGORY_ID_REPO                9       /* 品种代码-回购 */


/*
 * 结构体定义
 */

/*
 * 证券品种列表
 */
typedef struct {
    /*
     * 由网络监控子系统维护的信息
     * 为保障结构定义的直观, 未直接引入 CATEGORY_MON 结构
     * <b>
     * 若 CATEGORY_MON 结构发生改变, 需要同步修改该区段
     * </b>
     */
    int     	id;                                     	/* 品种代码 */
    char    	name[CATEGORY_NAME_LENGTH];             	/* 品种名称 */
    
    char		subType[CATEGORY_SUB_TYPE_NUMBER][CATEGORY_SUB_TYPE_LENGTH];/* 包含的产品子类型 */
    /* -------------------------           */

    int     	securitiesNumber;                       	/* 属于该券种的证券数量 */
    long long   tradeAmount;                            	/* 总成交数量 */
    long long   tradeMoney;                             	/* 总成交金额 */	

    /*
     * 结构指针
     */
    int     	firstStock;                             	/* 该券种的第一只股票 */
    int     	lastStock;                              	/* 该券种的最后一只股票 */
    /* -------------------------           */

} CATEGORY_CS;


/*
 * 函数声明
 */
CATEGORY_CS*    GetCategoryList();                      /* 返回Category数组的起始位置 */
CATEGORY_CS*    GetCategory(int index);                 /* 返回指定位置的CATEGORY_CS */
CATEGORY_CS*    GetCategoryById(int id);                /* 返回品种代码对应的CATEGORY_CS */
int				GetStockFromSubType(char* pSubType);	/* 通过产品子类型获得产品类型 */
int             GetCategoryNumber();                    /* 返回Category数量 */
int             AddCategory(CATEGORY_CS*);              /* 向Category数组中增加Category条目 */


int             AddCategoryStock(int, STOCK_CS*);       /* 向指定券种中添加所属证券信息 */

void            PrintCategoryInfoToFile(int fp);        /* 打印券种信息到指定的文件中 */


#endif  /* _MODEL_CATEGORY_H */

