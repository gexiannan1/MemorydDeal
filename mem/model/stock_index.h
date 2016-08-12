/******************************************************************
** 文件名:      stock_index.h
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.8.30
** 描 述:       股票指数 头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock_index.h,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#ifndef _STOCK_INDEX_H
#define _STOCK_INDEX_H

#include    "model_length_define.h"
#include	"stock.h"
#include    "../util/symbol.h"


/*
 * 结构体定义
 */

/*
 * 分类指数报文体结构
 */
typedef	struct{
	DETAILED_INDEX	detailedIndex;										/* 分类指数报文 */
	char			indexCode[STOCK_ID_LENGTH];							/* 股票指数分类代码 */
} STOCK_INDEX_CS;


/*
 * 函数声明
 */
STOCK_INDEX_CS*		GetStockIndexHead();								/* 获得股票指数存储起始位置 */
STOCK_INDEX_CS*		GetStockIndex(int);									/* 返回指定位置的STOCK_INDEX_CS */
STOCK_INDEX_CS*		GetStockIndexByIsinCod(char*);						/* 根据分类代码获得股票指数 */
int                 GetStockIndexNumber();               				/* 返回StockIndex数量 */
int					AddStockIndexInfo(STOCK_INFO*);						/* 添加指数信息到共享内存 */
BOOL				UpdateDetailedIndex(DETAILED_INDEX*);				/* 向共享内存中更新指数信息 */

void				PrintStockIndex(STOCK_INDEX_CS*);					/* 输出指数信息 */
void				PrintStockIndexToFile(int);							/* 输出指数信息到文件 */

#endif /* _STOCK_INDEX_H */
