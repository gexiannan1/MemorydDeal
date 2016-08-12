/******************************************************************
** 文件名:      memory.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.4
** 描 述:       共享内存分配与管理函数头文件
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/memory/memory.h,v 1.1 2012/03/29 01:19:50 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:50 $
 */

#ifndef _MEMORY_MANAGER_H
#define _MEMORY_MANAGER_H


#include    "shm_define.h"
//#include    "../model/sort.h"
//#include	"../model/global_data.h"
//#include    "../model/model_te.h"
//#include    "../model/model_mon.h"
//#include    "../../util/symbol.h"


/*
 * vender 发送状态定义
 */

#define VENDER_SEND_NORMAL     0
#define VENDER_SEND_PAUSED     1
/*
 * 共享内存参数配置
 */
typedef struct {
	int		setIdMapMax;								/* setId 数量*/
	int		setMax;										/* set 块数量 */
	int		setSizeMax;									/* 每 set 块大小*/
	int     categoryMax;                                /* 最大证券品种数量 */
    int     stockMax;                                   /* 最大股票数量 */
    #ifdef USE_PBU
	    int		pbuNumberMax;							/* 最大pbu数量 */
	    int		pbuDataNumberMax;						/* 最大pbu数据数量 */
    #endif
    int     orderMax;                                   /* 最大委托数量 */
    int     tradeMax;                                   /* 最大成交数量 */
    int     withdrawMax;                                /* 最大撤单数量 */
    int     bidMax;                                     /* 最大委买价位数量 */
    int     offerMax;                                   /* 最大委卖价位数量 */
    int     industryMax;                                /* 最大行业数量 */
    int		stockIndexMax;								/* 最大股票指数数量 */
} MEM_CONFIG_SHARE;


/*
 * 共享内存分配情况
 */
typedef struct {
    /*
     * 各单元的内存分配大小
     */
    long    statsSize;                                  /* MEM_STATS 占有的内存大小 */
    long	setIdMapSize;								/* SET_ID_MAP 占有的内存大小 */
    long	setStatsSize;								/* SET_STATS 占有的内存大小 */
    long	setMapSize;									/* SET_MAP 占有的内存大小 */
    long    categorySize;                               /* 证券品种 占有的内存大小 */
    long    stockSize;                                  /* 股票信息 占有的内存大小 */
    #ifdef USE_PBU
	    long	pbuFirmSize;							/* pbu号列表占有的内存大小 */
	    long	pbuTableSize;							/* pbu索引列表占有的内存大小 */
	    long	pbuBuySortSize;							/* pbu买委托排名列表占有的内存大小 */
	    long	pbuSellSortSize;						/* pbu卖委托排名列表占有的内存大小 */
	    long	pbuDataSize;							/* pbu数据区占有的内存大小 */
    #endif
    long    orderSize;                                  /* Order 占有的内存大小 */
    long    tradeSize;                                  /* Trade 占有的内存大小 */
    long    withdrawSize;                               /* Withdraw 占有的内存大小 */
    long    bidSize;                                    /* 委买价位 占有的内存大小 */
    long    offerSize;                                  /* 委卖价位 占有的内存大小 */
    long    aIndustrySize;                              /* a股行业 占有的内存大小 */
    long    bIndustrySize;                              /* b股行业 占有的内存大小 */
    long	stockIndexSize;								/* 股票指数 占有的内存大小 */
	
    /*
     * 各单元的起始位置（偏移位置）
     */
    long    statsOffset;                                /* MEM_STATS 的起始位置 */
    long	setIdMapOffset;								/* SET_ID_MAP 的起始位置 */
    long	setStatsOffset;								/* setStats 的起始位置 */
    long	setMapOffset;								/* setMap 的起始位置 */
    long    categoryOffset;                             /* 证券品种 的起始位置 */
    long    stockOffset;                                /* 股票信息 的起始位置 */
    #ifdef USE_PBU
	    long	pbuFirmOffset;							/* pbu号的起始位置 */
	    long	pbuTableOffset;							/* pbu索引列表的起始位置 */
	    long	pbuBuySortOffset;						/* pbu买委托排名列表的起始位置 */
	    long	pbuSellSortOffset;						/* pbu卖委托排名列表的起始位置 */
	    long	pbuDataOffset;							/* pbu数据区的起始位置 */
    #endif
    long    orderOffset;                                /* Order 的起始位置 */
    long    tradeOffset;                                /* Trade 的起始位置 */
    long    withdrawOffset;                             /* Withdraw 的起始位置 */
    long    bidOffset;                                  /* 委买价位 的起始位置 */
    long    offerOffset;                                /* 委卖价位 的起始位置 */
    long    aIndustryOffset;                            /* a股行业 的起始位置 */
    long    bIndustryOffset;                            /* b股行业 的起始位置 */
	long	stockIndexOffset;							/* 股票指数 的起始位置 */

} MEM_ALLOCATION_SHARE;


/*
 * 共享内存当前状态
 */
typedef struct {
	char		gwServerIp[32];
    int			gwServerPort;
	int			nextSet;								/* 下一空闲的 set 块位置 */
    int     	nextStock;								/* 下一空闲的 Stock 列表位置 */
    #ifdef USE_PBU
	    int		nextPBUFirm;							/* 下一空闲的 pbu 号码位置 */
	    int		nextPBUData;							/* 下一空闲的 pbu 数据位置*/
	#endif
    int     	nextOrder;								/* 下一空闲的 Order 列表位置 */
    int     	nextTrade;								/* 下一空闲的 Trade 列表位置 */
    int     	nextWithdraw;							/* 下一空闲的 Withdraw 列表位置 */
    int			nextStockIndex;							/* 下一空闲的 stockIndex 列表位置 */
    int     	nextBid;								/* 下一空闲的 Bid 列表位置 */
    int     	nextOffer;								/* 下一空闲的 Offer 列表位置 */
    int     	nextAIndustry;							/* 下一空闲的 nextAIndustry 列表位置 */
    int     	nextBIndustry;							/* 下一空闲的 nextBIndustry 列表位置 */
    int     	nextCategory;							/* 下一空闲的 Category 列表位置 */
} MEM_CURRENT_SHARE;


typedef struct{
	int							id;						/* 数据发布器编号 */
	long long					maxSnapshotSendTime;	/* 每个发送通道发送一轮消息的最大发送时间 */
	long long					totalSnapshotSendTime;	/* 每个发送通道发送一轮消息所需要的总发送时间 */
	int							numSendTimeOut;			/* 每个发送通道发送消息时间超过阀值的次数 */
	int							numStatistics;			/* 每个发送通道统计发送时间的次数 */
} CHANNEL_SEND_INFO;



/*
 * 当前交易情况
 */
typedef struct {
    /* 下列各项的值与MEM_CURRENT_SHARE中对应的值相同，暂不维护
    int     stockNumber;                                / * 当前总股票数 * /
    int     orderNumber;                                / * 当前总委托数 * /
    int     tradeNumber;                                / * 当前总成交数 * /
    int     withdrawNumber;                             / * 当前总撤单数 * /
    int     bidNumber;                                  / * 当前总委买价位数 * /
    int     offerNumber;                                / * 当前总委卖价位数 * /
    */
    int     isStockReceived;                            /* 产品基础信息已加载完毕标志 */
    int     isStockIndustryLoaded;                      /* 股票行业配置已加载完毕标志 */
    int		isCommonBondLoaded;
    int		isConvertibleBondLoaded;
    int		isControlSegmentLoaded;						/* 产品群基础信息已加载完毕标志 */
    int		ocallFlag;									/* 开市集合竞价标记 -1: 初始状态 0：开市集合竞价期间 1：开市集合竞价结束 */
	
//	CS_DATA_SEQUENCE csDataSequence;					/* CS端已接收的数据数量 */
	
    int     workedOrderNumber;                          /* 当前已处理过的委托数 */
    int     workedTradeNumber;                          /* 当前已处理过的成交数 */
    int     workedWithdrawNumber;                       /* 当前已处理过的撤单数 */

    int     sentTradeNumber;                            /* 当前已发送的成交数量 */

    /*
     * 汇总统计信息
     */
    long long   tradeAmount;                            /* 总成交数量 */
    long long	bTradeAmount;							/* b股总成交数量*/
    long long	notBTradeAmount;						/* 非b股总成交数量*/
    long long   tradeMoney;                             /* 总成交金额 */
	long long	bTradeMoney;							/* b股总成交金额*/
	long long	notBTradeMoney;							/* 非b股总成交金额*/
    /*
     * 排名信息
     */
    //WITHDRAW_BUY_SORT_LIST      aWithdrawBuySortList;	/* 单笔委托买入撤单排名（单笔委托数量最大的10笔委买撤单） */
    //WITHDRAW_SELL_SORT_LIST     aWithdrawSellSortList;	/* 单笔委托卖出撤单排名（单笔委托数量最大的10笔委卖撤单） */
	//WITHDRAW_BUY_SORT_LIST      bWithdrawBuySortList;	/* 单笔委托买入撤单排名（单笔委托数量最大的10笔委买撤单） */
    //WITHDRAW_SELL_SORT_LIST     bWithdrawSellSortList;	/* 单笔委托卖出撤单排名（单笔委托数量最大的10笔委卖撤单） */
	
	/* 
	 * 发送统计信息 
	 */
//	CHANNEL_SEND_INFO			channelSendInfo[VENDER_NUMBER];			/* 发送通道统计信息 */
	int							lastMarketTime;							/* 最新行情时间 */
	
} MEM_TRADE_STATS;
/* -------------------------           */


/*
 * CS时间与TE时间记录
 */
typedef struct {
    int     teTime;                                     /* TE 时间 */
    int     csTime;                                     /* CS 时间 */
    int     windage;                                    /* CS与TE的时间偏差 */
} MEM_TIME;
/* -------------------------           */


/*
 * 共享内存状态
 */
typedef struct {
    long                    totalSize;                  /* 共享内存的总大小 */

    int                     venderStatus;
    long long               gwHeartbeat;
    
    /*
     * 系统全局数据
     */
   // GLOBAL_DATA             globalData;                 /* 系统全局数据 */

    /*
     * 内存状态
     */
    MEM_CONFIG_SHARE        shareConfig;               /* 共享内存参数配置 */
    MEM_ALLOCATION_SHARE    shareAllocation;           /* 共享内存分配情况 */
    MEM_CURRENT_SHARE       shareCurrent;              /* 共享内存当前状态 */
    MEM_TIME                shareTime;                 /* CS时间与TE时间记录 */
    MEM_TRADE_STATS         shareTradeStats;           /* 当前交易情况 */

} MEM_STATS;
/* -------------------------           */


/*
 * 共享内存参数配置
 */
typedef struct {
    MEM_CONFIG_SHARE        shareConfig;               /* 共享内存参数配置 */
} MEM_CONFIG_ALL;

/*
 * 共享内存分配情况
 */
typedef struct {
    long                    totalSize;                  /* 共享内存的总大小 */
    MEM_ALLOCATION_SHARE    shareAllocation;           /* 共享内存分配情况 */
} MEM_ALLOCATION_ALL;
/* -------------------------           */


/*
 * 函数声明
 */

int                    InitShareMemory(const MEM_CONFIG_ALL*);                 /* 初始化系统共享内存 */
void                    ReleaseShareMemory();                                   /* 释放系统共享内存 */
MEM_ALLOCATION_ALL*     EvaluateMemAllocationStats(const MEM_CONFIG_ALL*);      /* 评估共享内存的使用情况 */

int                    IsShareMemoryReady();                                   /* 返回系统共享内存是否已准备就绪 */
void*                   GetShareMemory();                                       /* 返回共享内存的数据段开始地址 */


MEM_STATS*              GetMemStats();                                          /* 返回 MEM_STATS 结构 */
MEM_CONFIG_SHARE*       GetShareMemConfigStats();                               /* 返回 MEM_CONFIG_SHARE 结构 */
MEM_ALLOCATION_SHARE*   GetShareMemAllocationStats();                           /* 返回 MEM_ALLOCATION_SHARE 结构 */
MEM_CURRENT_SHARE*      GetShareMemCurrentStats();                              /* 返回 MEM_CURRENT_SHARE 结构 */
MEM_TIME*               GetMemTime();                                           /* 返回 MEM_TIME 结构 */
MEM_TRADE_STATS*        GetMemTradeStats();                                     /* 返回 MEM_TRADE_STATS 结构 */

//CS_DATA_SEQUENCE*       GetCsSequence();                                        /* 返回CS端已接收的数据数量 */

void                    UpdateMemTime(int teTime);                              /* 更新 MEM_TIME */
int                     GetTeTime();                                            /* 返回TE主机时间 */
void                PrintEvaluateMemAllocationStats(const MEM_CONFIG_ALL*);     /* 打印共享内存使用情况评估 */
void                PrintMemoryStats();                                         /* 打印共享内存状态 */
void                PrintMemoryConfigStats();                                   /* 打印共享内存参数配置 */
void                PrintMemoryAllocationStats();                               /* 打印共享内存分配信息 */
void                PrintMemoryCurrentStats();                                  /* 打印共享内存当前状态 */
void                PrintMemoryTimeStats();                                     /* 打印共享内存中的CS时间与TE时间记录 */
void                PrintMemoryTradeStats();                                    /* 打印共享内存当前交易情况 */


#endif  /* _MEMORY_MANAGER_H */
