/******************************************************************
** 文件名:      stock_index.h
** 版 本:       0.1
** 创建人:      Li Bo
** 日 期:       2006.9.7
** 描 述:       市场行情信息 头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/inside_market.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _INSIDE_MARKET_H
#define _INSIDE_MARKET_H

#include    "model_length_define.h"
#include	"model_te.h"
#include    "../util/symbol.h"


/*
 * 结构体定义
 */


/*
 * 函数声明
 */
int						UpdateInsideMarket(INSIDE_MARKET*);				/* 向共享内存中更新市场行情信息 */

#endif /* _INSIDE_MARKET_H*/
