/******************************************************************
** 文件名:      stock.h
** 版 本:       0.1
** 创建人:      Song Fang
**              zhaodj
** 日 期:       2005.9.6
** 描 述:       Stock信息定义的头文件
** 修订记录：
** 修订日期     修订人          SIR         修复的bug或者修订原因
**-----------------------------------------------------------------
** 2006.1.4     Song Fang                   增加返回单边模式的成交信息的接口
** 2006.03.23   Wei Huanyin                 1、增加lastPrice字段处理
** 2006.04.15   Wei Huanyin                 1、增加导入债券静态数据标志处理函数
** 2006.05.30   Wei Huanyin                 1、增加集合竞价结束时相关数据
** 2006.06.15   zhuhui 		                1、增加虚拟集合竞价相关数据
** 2006.06.16   zhuhui		                1、增加更新虚拟集合竞价信息函数
** 2007.04.04	Li Bo		                在STOCK_INFO和STOCK_CS中增加setId
** 2011.05.10   Hu Ping         00016       增加实时计算指标最长买卖成交需要时间
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/stock.h,v 1.1 2012/03/29 01:19:54 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:54 $
 */

#ifndef _MODEL_STOCK_H
#define _MODEL_STOCK_H

#include    "model_te.h"
#include    "model_mon.h"
//#include    "../../public/static_file/stock_industry.h"
#include    "../util/symbol.h"



/*
 * 是否使用PBU排名,如果定义了则使用，没定义则不使用 
 */
//#define				USE_PBU


/*
 * 常量定义
 */
#define     ORDER_TRADING_RESTRICTION_OA  			"OA"			//只参加开盘集合竞价
#define     ORDER_TRADING_RESTRICTION_AU			"AU"			//集合竞价期间有效
#define     ORDER_TRADING_RESTRICTION_SU   			"SU"			//在订单薄平衡时段输入
#define     ORDER_TRADING_RESTRICTION_FP 			"FP"			//在固定价格时段有效
#define     ORDER_TRADING_RESTRICTION_SPACE 		"  "			//无限制

#define		STOCK_TRADING_RESTRICTION_PRETR			"PRETR"			//PRETR - 盘前
#define     STOCK_TRADING_RESTRICTION_OCALL			"OCALL"			//OCALL - 开市集合竞价
#define     STOCK_TRADING_RESTRICTION_ICALL			"ICALL" 		//ICALL - 盘中集合竞价
#define     STOCK_TRADING_RESTRICTION_IOBB 			"IOBB"  		//IOBB - 盘中订单簿平衡，
#define     STOCK_TRADING_RESTRICTION_IPOBB 		"IPOBB" 		//IPOBB - 盘中订单簿平衡前期，
#define     STOCK_TRADING_RESTRICTION_OOBB 			"OOBB" 			//OOBB  - 开市订单簿平衡
#define     STOCK_TRADING_RESTRICTION_OPOBB 		"OPOBB" 		//OPOBB - 开市订单簿平衡前期
#define     STOCK_TRADING_RESTRICTION_FCALL 		"FCALL" 		//FCALL - 固定价格集合竞价
#define     STOCK_TRADING_RESTRICTION_TRADE 		"TRADE" 		//TRADE - 连续交易
#define     STOCK_TRADING_RESTRICTION_BREAK 		"BREAK" 		//BREAK - 休市
#define     STOCK_TRADING_RESTRICTION_HALT 			"HALT" 			//HALT - 停盘
#define     STOCK_TRADING_RESTRICTION_SUSP 			"SUSP" 			//SUSP - 停牌
#define     STOCK_TRADING_RESTRICTION_CLOSE 		"CLOSE" 		//CLOSE - 闭市
#define     STOCK_TRADING_RESTRICTION_POSTR			"POSTR" 		//POSTR - 盘后处理
#define     STOCK_TRADING_RESTRICTION_ENDTR			"ENDTR" 		//ENDTR - 结束交易
#define     STOCK_TRADING_RESTRICTION_BETW			"BETW" 			//BETW - 交易间

#define		OCALL_FLAG_INIT							-1				//开市集合竞价标记初始状态
#define		OCALL_FLAG_PHASE						0				//开市集合竞价标记期间
#define		OCALL_FLAG_END							1				//开市集合竞价标记结束

#define		PROCESS_STUB							0				//接收与加工进程的编号（获取软件锁时使用）
#define		PROCESS_VENDER							1				//发送进程的编号（获取软件锁时使用）

/*
 * STOCK_INFO
 */
typedef struct {
	int				setId;									/* 证券所属set号 */
	char			isinCod[STOCK_ISIN_CODE_LENGTH];			/* 股票国际证券标识码 */
	int				isinIndex;									/* 股票国际证券标识码索引 */
	char			stockId[STOCK_ID_LENGTH];               	/* 证券代码 */
    char			stockShortName[STOCK_SHORTNAME_LENGTH]; 	/* 证券简称 */
    char			conSegCode[5];								/* 产品群 */
    int				lotSize;                                	/* 交易单位大小 */
} STOCK_INFO;
/* ------------------------------------------------- */

/*
 * 股票统计信息
 */
typedef struct {
	int				number;                         /* 笔数 */
    long long		amount;                         /* 数量 */
    long long		money;                          /* 金额 */
} STOCK_STAT;


/*
 * 债券信息
 */
typedef struct {
	int			cashDate;					/* 到期兑付日-*/
	int			cashFrequency;				/* 付息频率*/
	int			interestpaymentDate;		/* 付息日*/
	int			bondValue;					/* 债券面值*/
	int			boundInterestRate;			/* 债券年利率*/
	int			startDate;					/* 起息日-*/
	int			bondPublishPrice;			/* 债券发行价*/
	int			bondType;					/* 债券类型0:固定利率1：浮动利率2：零息债券3：一次性还本付息债券*/
	int			preInterestpaymentDate;		/* 上一理论付息日*/
	int			nextInterestpaymentDate;	/* 下一理论付息日*/
	int			categoryType;				/* 债券品种类型*/
	int			workDate;					/* 当前工作日*/
	int 		yieldToMaturity;			/* 到期收益率*/
	int			preLastPrice;				/* 上一次净价*/
} BOND_CS;
/*
 * Stock信息
 */
typedef struct {
    /*
     * 从TE系统导入的信息
     * 为保障结构定义的直观, 未直接引入 STOCK_TE 结构
     * <b>
     * 若 STOCK_TE 结构发生改变, 需要同步修改该区段
     * </b>
     */
    int				stockNo;								/* TE stock表入口（从0开始） */
	char			isinCod[STOCK_ISIN_CODE_LENGTH];		/* 股票国际证券标识码 */
    int				isinIndex;								/* 股票国际证券标识码索引 */
    char			stockId[STOCK_ID_LENGTH];				/* 证券代码 */
    char			stockShortName[STOCK_SHORTNAME_LENGTH];	/* 证券简称 */
    int				setId;									/* 证券所属set号 */
    int				stockType;								/* 证券类型 */
   
    int				lotSize;								/* 交易单位大小 */
    char			currencyType[CURRENCY_TYPE_LENGTH];		/* 货币类型 */
    char			conSegCode[5];							/* 产品群 */
    char			matchingPriority;						/* 撮合优先级 */
    /* -------------------------           */

    int				categoryNo;								/* 品种表入口 */
    int				industryNo;								/* 行业表入口 */
    long long		openPrice;								/* 开盘价 */
    long long		closePrice;								/* 收盘价 */
    long long		prevClosingPrice;						/* 前收盘价 */
    long long		highPrice;								/* 最高价 */
    long long		lowPrice;								/* 最低价 */
    long long		lastPrice;								/* 最新价 */
    long long		netChg;									/* 涨跌值 */
   	long long		fixPrc;									/* 在固定竞价时段内的执行价格 */
    BOND_CS			bond;									/* 债券信息 */
    int				tradeNumber;							/* 成交笔数 */
    long long   	tradeAmount;							/* 成交数量 */
    long long   	tradeMoney;								/* 成交金额 */
    int				marketTradeNumber;						/* 市价成交笔数 */
    long long   	marketTradeAmount;						/* 市价成交数量 */
    long long   	marketTradeMoney;						/* 市价成交金额 */
    int				marketTradeBuyNumber;					/* 市价买成交笔数 */
    long long		marketTradeBuyAmount;					/* 市价买成交数量 */
    long long		marketTradeBuyMoney;					/* 市价买成交金额 */
    int				marketTradeSellNumber;					/* 市价卖成交笔数 */
   	long long		marketTradeSellAmount;					/* 市价卖成交数量 */
   	long long		marketTradeSellMoney;					/* 市价卖成交金额 */
   
    char			moiInd;									/* 市场订单中断标识 */
    char			commonPrcsStsValCod[6];					/* 公共行情市场状态代码 */
    char			prcsStsValCod[6];						/* 市场状态代码 */
    char			prePrcsStsValCod[6];					/* 上一个市场状态代码 */
    char			volaIndicator[3];						/* Vola波动性标识 */
    char			bstOrdPrcInd;							/* 最好委托价格标识 */
    long long		iopv;									/* ETF净值 */
    
    //2010.5.5 增加etf申购赎回笔数、数量、金额
    int				etfBuyNumber;							/* ETF申购笔数 */
    long long		etfBuyAmount;							/* ETF申购数量 */
    long long		etfBuyMoney;							/* ETF申购金额 */
    int				etfSellNumber;							/* ETF赎回笔数 */
    long long		etfSellAmount;							/* ETF赎回数量 */
    long long		etfSellMoney;							/* ETF赎回金额 */
   
    long long		totWarExQty;							/* 权证行权总数量 */
    long long		totWarClQty;							/* 权证注销总数量 */
    long long		totWarWrQty;							/* 权证创设总数量 */
    int	            lastTradeTime;							/* 最后一笔交易的时间*/
/*** SIR 00016 BEGIN */
    int             bidTradeMaxDuration;                    /* 最长买成交需要时间*/
    int             offerTradeMaxDuration;                  /* 最长卖成交需要时间*/
/*** SIR 00016 END */   
    long long		openInterest;							/* 开仓量/目前利息 */
    long long		deriLstStlPrc;							/* 前一笔交割价 */

   
    long long       lstAuctPrc;                           	/* 最新竞价价格*/
    long long       lstAuctQty;                         	/* 最新竞价数量*/
    long long       surplusQty;                    			/* 数量盈余*/
    char       	    surplusBidAskIndicator;    				/* 买卖盈余方向*/
   
   
    int				orderBuyNumber;                         /* 当前交易时段委托买入笔数 */
    long long		orderBuyAmount;                         /* 当前交易时段委托买入数量 */
    long long		orderBuyMoney;                          /* 当前交易时段委托买入金额 */
    int				orderSellNumber;                        /* 当前交易时段委托卖出笔数 */
    long long		orderSellAmount;                        /* 当前交易时段委托卖出数量 */
    long long		orderSellMoney;                         /* 当前交易时段委托卖出金额 */
    int				withdrawBuyNumber;                      /* 买入撤单笔数 */
    long long		withdrawBuyAmount;                      /* 买入撤单数量 */
    long long		withdrawBuyMoney;                       /* 买入撤单金额 */
    int				withdrawSellNumber;                     /* 卖出撤单笔数 */
    long long		withdrawSellAmount;                     /* 卖出撤单数量 */
    long long		withdrawSellMoney;                      /* 卖出撤单金额 */

    int				bidNumber;                              /* 委买价位个数 */
    int				offerNumber;                            /* 委卖价位个数 */

    /* -------------------------           */

    /*
     * 结构指针
     */
    int         nextCategory;								/* 同券种下一只股票 */
   
    //level2三期>>>>
    int         channelId;                                  /* 本股票发送通道的编号 */
    int         channelNextStock;                           /* 同一个通道下一只股票 */

    int         firstOrderBuy;								/* 第一个买单 */
    int         lastOrderBuy;								/* 最后一个买单 */

    int         firstOrderSell;								/* 第一个卖单 */
    int         lastOrderSell;								/* 最后一个卖单 */

    int         firstTrade;									/* 第一笔交易 */
    int         lastTrade;									/* 最后一笔交易 */

    int         firstWithdraw;								/* 第一笔撤单 */
    int         lastWithdraw;								/* 最后一笔撤单 */

    int         highestBid;									/* 最高竞买价 */
    int         lowestBid;									/* 最低竞买价 */

    int         lowestOffer;								/* 最低竞卖价 */
    int         highestOffer;								/* 最高竞卖价 */

	#ifdef USE_PBU
		int			pbuTradeBuySort[PBU_TRADE_SORT_LENGTH];		/* pbu买成交排名 */
		int			pbuTradeSellSort[PBU_TRADE_SORT_LENGTH];	/* pbu卖成交排名 */
		int			lastBuyOrderSortOffset;						/* 最后一个已存放pbu买委托排名的位置 */
		int			lastSellOrderSortOffset;					/* 最后一个已存放pbu卖委托排名的位置 */
	#endif
    /* -------------------------           */
   
	STOCK_STAT	tradePhaseStat[8];							/* 交易时段统计信息  0,1为OA，2,3为AU，4,5为SU，6,7为FP */

	BOOL		lockFLag[2];								/* 软件锁 */
	int			turn;										/* 需要获取锁的进程编号 */
} STOCK_CS;


/*
 * 函数声明
 */
STOCK_CS*   GetStockList();                                 /* 返回Stock数组的起始位置 */
STOCK_CS*   GetStock(int index);                            /* 返回指定位置的STOCK_CS */
STOCK_CS*   GetStockByIsinCod(char *isinCod);               /* 返回指定isinCod对应的STOCK_CS */
STOCK_CS*	GetStockByStockId(char *stockId);				/* 返回指定stockId对应的STOCK_CS */
STOCK_CS*	GetStockByIsinIndex(int isinIndex);				/* 返回指定isinIndex对应的STOCK_CS */
int         GetStockNumber();                               /* 返回Stock数量 */

int         AddStock(STOCK_INFO*);                            				/* 向Stock数组中增加Stock条目 */
BOOL        UpdateStockCategory(STOCK_CATEGORY_MON*);       				/* 更新证券品种配置 */
//BOOL        UpdateStockIndustry(STOCK_INDUSTRY*,int shareType);             /* 更新证券行业配置 */

BOOL        IsShare(STOCK_CS*);                             /* 返回stockNo对应的证券类型是否是股票 */
BOOL        IsAShare(STOCK_CS*);                             /* 返回stockNo对应的证券类型是否是A股 */
BOOL        IsBShare(STOCK_CS*);                             /* 返回stockNo对应的证券类型是否是B股 */
BOOL        IsIndex(STOCK_CS*);                             /* 返回stockNo对应的证券类型是否是指数 */
BOOL        IsValidStock(STOCK_CS*);                  		/* 返回stockNo对应的证券是否有效产品品种 */
BOOL    	IsTradePhaseBB(STOCK_CS*);						/* 返回是否是产品交易时段 */
BOOL    	IsTradePhaseTr(STOCK_CS*);						/* 返回是否是产品交易时段 */
BOOL		CheckTradePhase(STOCK_CS*);						/* 检测证券的交易时段是否需要可以发送（是否需要检测倒挂） */
BOOL		IsPriceAcross(STOCK_CS*);						/* 检测证券中是否存在价格交叉（是否倒挂） */

BOOL        IsStockReceived();                              /* 返回产品基础信息配置是否已加载完毕 */
void        SetStockReceived(BOOL);                         /* 设置产品基础信息配置是否已加载完毕 */



BOOL        IsStockIndustryLoaded();                        /* 返回股票行业配置是否已加载完毕 */
void        SetStockIndustryLoaded(BOOL);                   /* 设置股票行业配置是否已加载完毕 */

BOOL		IsCommonBondLoaded();							/* 返回普通债券数据是否已加载完毕 */
void		SetCommonBondLoaded(BOOL);						/* 设置普通债券数据是否已加载完毕 */
BOOL		IsConvertibleBondLoaded();						/* 返回可转换债券数据是否已加载完毕 */
void		SetConvertibleBondLoaded(BOOL);					/* 设置可转换债券是否已加载完毕 */

BOOL        IsControlSegmentLoaded();                       /* 返回产品群基础信息配置是否已加载完毕 */
void        SetControlSegmentLoaded(BOOL);                  /* 设置产品群基础信息配置是否已加载完毕 */
int   		IsvaidOfferNumber(STOCK_CS*);					/* 当前交易时段的有效价位个数卖 */
int			IsvaidBidNumber(STOCK_CS*); 					/* 当前交易时段的有效价位个数买 */

void		PrintStock(STOCK_CS*);							/* 打印证券信息到日志中 */	

void		StockLock(STOCK_CS*, int);						/* 获取软件锁 */
void		StockUnlock(STOCK_CS*, int);					/* 释放软件锁 */

BOOL        PrintStockInfoToFile(int);		                /* 打印股票信息到指定的文件中 */
BOOL        PrintStockToFile();		                   		/* 打印股票信息到指定的文件中 */
BOOL		PrintInsideMarketToFile();						/* 打印市场行情信息到指定的文件 */
BOOL		PrintShow2007SnapshotToFile();					/* 打印show2007快照 */

#endif  /* _MODEL_STOCK_H */
