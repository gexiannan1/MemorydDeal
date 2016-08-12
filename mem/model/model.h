/******************************************************************
** 文件名:      model.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.6
** 描 述:       Model定义汇总
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/model.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _MODEL_H
#define _MODEL_H


#include    "model_length_define.h"


/*
 * 常量定义
 */
#define		NULL_INDEX				-1
#define		REPO_PRICE 				1000000

//程序中是否使用测试工具（如果定义了，使用测试工具，未定义则不使用测试工具）
//#define		TEST_TOOLS_USING

//#define		GW_RECEIVE_SPEED_TEST              /*是否测试前置机消息的处理速度*/

/*
 * Model头文件引入
 */
#include	"stock_pbu.h"
#include    "withdraw.h"
#include    "price.h"
#include    "category.h"
#include    "industry.h"
#include    "sort.h"
#include    "global_data.h"
#include	"stock_index.h"
#include	"inside_market.h"
#include	"set_map.h"


#endif  /* _MODEL_H */

