/******************************************************************
** 文件名:      model_te.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.8
** 描 述:       从TE系统导入的Model定义
** 修订记录：
** 修订日期     修订人          SIR         修复的bug或者修订原因
××----------------------------------------------------------------
** 2011.05.10   wangyx          00016       增加今收盘指数
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/model_te.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */
 


#ifndef _MODEL_TE_H
#define _MODEL_TE_H


#include    "model_length_define.h"


/*
 * TE(交易系统)数据长度定义
 */
#define TE_STOCK_ID_LENGTH                  6
#define TE_STOCK_SHORTNAME_LENGTH           8
#define TE_ORDER_FIRM_LENGTH                5
#define IDC_END_OF_DAY_MARKER_LENGTH       12          /* 结束标志长度*/
/* -------------------------           */



/*
 * 常量定义
 */
/*
 * Set
 */
#define SET_TYPE_ORDER						1
#define SET_TYPE_TRADE              		2
#define SET_TYPE_ORDER_CHANGE        		3
#define SET_TYPE_MARKET_STATUS				4
#define SNAP_TYPE_CSSTATUS					8
#define INVALID								0
/*
 * COMMON
 */
#define	COMMON_TYPE_DETAILED_INDEX			5
#define	COMMON_TYPE_INSIDE_MARKET			6
#define COMMON_TYPE_MATSTATE_CHANGE			7

/*
 * Stock
 */
#define STOCK_STATUS_SUSPENDED              'S'
#define STOCK_STATUS_NORMAL                 'N'
#define STOCK_STATUS_DELETED                'D'

#define STOCK_TYPE_BOND                     1           /* 债券 */
#define STOCK_TYPE_ASHARE                   2           /* A股 */
#define STOCK_TYPE_BSHARE                   3           /* B股 */
#define STOCK_TYPE_INDEX                    4           /* 指数 */
#define STOCK_TYPE_RIGHTS                   5           /* */
#define STOCK_TYPE_RIGHTSHOLD               6           /* */
#define STOCK_TYPE_NIHUIGOU                 7           /* 逆回购 */
#define STOCK_TYPE_ETF                      8           /* ETF */
#define STOCK_TYPE_SJSPEISHOU               9           /* 深交所配售 */
#define STOCK_TYPE_ZAIZHUANGU               10          /* 债转股 */
#define STOCK_TYPE_XINGU                    11          /* 新股 */
#define STOCK_TYPE_ZHENGHUIGOU              12          /* 正回购 */
#define STOCK_TYPE_KFJIJIN                  13          /*  */
#define STOCK_TYPE_QUANZHENG_JY             14          /*  */
#define STOCK_TYPE_QUANZHENG_EXCUTE         15          /* 原定义为权证行权，现在被预约收购等业务复用 */
#define STOCK_TYPE_QUANZHENG_CALL_CREAT     16          /*  */
#define STOCK_TYPE_QUANZHENG_PUT_CREAT      17          /*  */
/* -------------------------           */


/*
 * Order
 */
#define ORDER_TYPE_BUY                      '1'
#define ORDER_TYPE_SELL                     '2'

#define ORDER_MARKET_LIMIT_MARKET           'M'
#define ORDER_MARKET_LIMIT_LIMIT            'L'
#define ORDER_MARKET_LIMIT_FILL             'F'
#define ORDER_MARKET_LIMIT_OPEN_PERIOD      'O'

#define ORDER_STATUS_MATCHED                'M'
#define ORDER_STATUS_OPEN                   'O'
#define ORDER_STATUS_WITHDRAWN              'W'
#define ORDER_STATUS_PART_WITHDRAWN			'I'			 /*部分撤单标志*/
#define ORDER_STATUS_CALL_PRICE_OVER        'A'          /*集合竞价结束标志*/

#define ORDER_ORDERTYPE_ICE  			    'I'			//冰山订单
#define ORDER_ORDERTYPE_MARKET  		    'M'			//M - 市价
#define ORDER_ORDERTYPE_LIMIT  			    'L'			//L - 限价
#define ORDER_ORDERTYPE_MTOL  			    'T'			//T - 市价转限价
#define ORDER_ORDERTYPE_QUOTE  			    'Q'			//Q - 报价
/* -------------------------           */




//#pragma HP_ALIGN NOPADDING PUSH
#pragma pack(push,1)

/*
 * ORDER_TE（从TE系统导入的信息）
 */
typedef struct {
    unsigned long long		orderNo;                                    /* 订单号 */
    unsigned long long		orderNoOld;									/* 旧订单号 */
    unsigned long long		orderNoOld1;								/* 旧订单号 */
    int						orderProcessNo;								/* 订单序列号 */
    int						orderProcessNoOld;							/* 旧订单序列号 */
    int						orderProcessNoOld1;							/* 旧订单序列号 */
    int			            orderTime;                                  /* 订单接收时间 */
    char					isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码*/
    char    				pbuId[PBU_FIRM_LENGTH];           			/* pbu号 */
    char    				orderBSType;                            	/* 买卖标志 - BUY 或 SELL */
    char    				orderType;                       			/* 订单类型 */
    long long  				orderPrice;                             	/* 价格 */
    long long  				orderQty;                               	/* 数量 */
    long long				ordExeQty;									/* 已执行数量 */
    char					trdResTypCod[3];							/* 交易限制类型 */
    char					trnTypId;									/* 事务类型 */
    char					orderStatus; 								/* 订单状态 */
    long long				remPeakQty;									/* 剩余揭示数量 */
    long long				peakSizeQty;								/* 最大揭示数量 */
    char					ordrResCod;									/* 订单限制类型 */
} ORDER_TE;
/* -------------------------           */

/*
 * 订单修改信息
 */
typedef ORDER_TE ORDER_CHANGE_TE;

/*
 * MARKET_STATUS_TE（从TE系统导入的信息）
 */
typedef struct {
    char					isinCod[STOCK_ISIN_CODE_LENGTH];		/* 股票国际证券标识码*/
    char    				tradePhase[6];                         /* trade时段 */
} MARKET_STATUS_TE;
/* -------------------------           */

/*
 * TRADE_TE（从TE系统导入的信息）
 */
typedef struct {
    unsigned long long			tradeNo;                                    /* 交易号 */
    int				            tradeTime;                                  /* 交易发生时间，hhmmss */
    int							tradeProcessNo;								/* 成交处理号 */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码*/
    char    					pbuId[PBU_FIRM_LENGTH];           			/* pbu号 */
    unsigned long long			orderNo;                                    /* 交易对应的订单号 */
    char						orderType;									/* 交易对应的订单的类型 */
    int							orderProcessNo;								/* 成交对应的订单处理号 */
    long long					price;                                      /* 价格 */
    long long					qty;                                        /* 数量 */
    long long					mktVal;										/* 成交金额 */
    char						type;                                       /* 交易类型 */
    char						trnTypId;									/* 事务类型 */
    char						tradeBSType;								/* 交易买卖类型 */
    long long					remPeakQty;									/* 剩余揭示数量 */
    long long					peakSizeQty;								/* 最大揭示数量 */
    unsigned long				ordrEntTim;									/* 冰山订单重新披露时间 */
} TRADE_TE;
/* -------------------------           */


/*
 * 分类指数报文体结构
 */
typedef struct {
    char			isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码 */
    long long       latestClosingIndex;                         /* 前收盘指数 */
    long long       openingIndex;                               /* 今开盘指数 */ 
    long long       tradeMoney;                                 /* 参与计算相应指数的成交金额 */
    long long       indexHigh;                                  /* 最高指数 */
    long long       indexLow;                                   /* 最低指数 */
    long long       latestIndex;                                /* 最新指数 */
/*** SIR 00016 BEGIN */
	long long		closeIndex;									/* 今收盘指数 */
/*** SIR 00016 END */
	char			tradeTime[9];								/* 交易时间 */
    long long       tradeAmount;                                /* 参与计算相应指数的交易数量 */
    long long		netChg;										/* 指数涨跌值 */
    long long		ratioChg;									/* 指数涨跌幅度（％） */
} DETAILED_INDEX;
/* -------------------------           */


/*
 * 市场行情信息
 */
typedef struct {
	char				isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码 */
	long long			openPrice;									/* 开盘价 */
	long long			closePrice;									/* 收盘价 */
	long long			prevClosingPrice;							/* 前收盘价 */
	long long			highPrice;									/* 最高价 */
	long long			lowPrice;									/* 最低价 */
	long long			lastPrice;									/* 最新价 */
	long long			netChg;										/* 涨跌值 */
	long long			fixPrc;										/* 在固定竞价时段内的执行价格 */
	unsigned long		tradeNumber;								/* 成交笔数 */
	long long			tradeAmount;								/* 成交总量 */
	long long			tradeMoney;									/* 成交总额 */
	char				moiInd;										/* 市场订单中断标识 */
	char				prcsStsValCod[6];							/* 品种状态代码 */
	char				bstOrdPrcInd;								/* 最好委托价格标识 */
	long long			iopv;										/* ETF净值 */
	long long			totWarExQty;								/* 权证行权总数量 */
	long long			totWarClQty;								/* 权证注销总数量 */
	long long			totWarWrQty;								/* 权证创设总数量 */
	long long			openInterest;								/* 开仓量/目前利息 */
	long long			deriLstStlPrc;								/* 前一笔交割价 */
	long long			virtualOpenPrice;							/* 虚拟开盘参考价格 */
	long long			virtualMatchAmount;							/* 虚拟匹配量 */
	long long			virtualUnMatchAmount;						/* 虚拟未匹配量 */
	char				srpBidAskInd;								/* 虚拟未匹配买卖方向 */
	char				volInd;										/* 波动性标识 */
} INSIDE_MARKET;
/* -------------------------           */


/*
 * CS端已接收的数据数量
 */
typedef struct {
    int     receivedSetDataNumber;					/* CS端已接收到的SET数量 */
    int     receivedCommonDataNumber;				/* CS端已接收到的公共数据信息数量 */
    
} CS_DATA_SEQUENCE;
/* -------------------------           */

//#pragma HP_ALIGN POP
#pragma pack(pop)

#endif  /* _MODEL_TE_H */
