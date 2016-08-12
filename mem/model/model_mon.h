/******************************************************************
** 文件名:      model_mon.h
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.13
** 描 述:       与网络监控子系统(monitor)共用的Model定义
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
** 2006-5-30     zhuhui      增加监控静态文件加载处理结构体
** 2007-4-4      wugb        增加监控端取setId
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/model_mon.h,v 1.1 2012/03/29 01:19:52 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:52 $
 */

#ifndef _MODEL_MONITOR_H
#define _MODEL_MONITOR_H


#include    "model_length_define.h"
#include    "../util/symbol.h"


/*
 * 数据长度定义
 * (为避免数据空隙，所有长度定义为4的倍数)
 */
#define MON_STOCK_NAME_LENGTH               32          /* 股票名称长度 */
#define MON_USERNAME_LENGTH                 32          /* 用户名长度 */
#define MON_PASSWORD_LENGTH                 40          /* 密码长度 */
#define MON_VENDER_TAG_LENGTH               32          /* 数据发布器标签长度 */
#define MON_VENDER_NAME_LENGTH              64          /* 数据发布器名称长度 */
#define MON_VENDER_IP_LENGTH                32          /* 组播地址长度 */
#define MON_VENDER_MAX_PROCESSORS_NUMBER    64          /* 最大处理器数量 */

#define MON_STATIC_FILE_LOAD_SUCCESS_STATUS	1          /* 静态数据加载成功 */
#define MON_STATIC_FILE_WAIT_LOAD_STATUS	0          /* 静态数据加载等待 */
#define MON_STATIC_FILE_LOAD_ERROR_STATUS	-1         /* 静态数据加载失败 */

/*
 * 发布器（通道）数量定义
 */
#define VENDER_NUMBER						50			/* 数据发布器（通道）最大数量 */
#define VENDER_SEND_TIMEOUT					3000000		/* 发送一轮数据所需要的时间的阀值 */
/*
 * 结构体定义
 */

/*
 * (证券)品种定义
 */
typedef struct {
    int     id;                                     /* 品种代码 */
    char    name[CATEGORY_NAME_LENGTH];             /* 品种名称 */
} CATEGORY_MON;
/* -------------------------           */


/*
 * 行业定义
 */
typedef struct {
    int     industryNo;                             /* 行业序号 */
    char    industryId[INDUSTRY_ID_LENGTH];         /* 行业代码 */
    char    industryName[INDUSTRY_NAME_LENGTH];     /* 行业名称 */
} INDUSTRY_MON;
/* -------------------------           */


/*
 * 证券品种配置
 */
typedef struct {
    char    stockId[STOCK_ID_LENGTH];               /* 证券代码 */
    char    stockName[MON_STOCK_NAME_LENGTH];       /* 证券名称 */
    int     categoryId;                             /* 品种名称 */
} STOCK_CATEGORY_MON;
/* -------------------------           */


/*
 * 数据发布器配置
 */
typedef struct {
    int     index;                                  /* 数据发布器序号（需保证ID与定义顺序一致） */
    int     id;                                     /* 数据发布器编号 */
    char    tag[MON_VENDER_TAG_LENGTH];             /* 数据发布器标签 */
    char    name[MON_VENDER_NAME_LENGTH];           /* 数据发布器名称 */
    char    ip[MON_VENDER_IP_LENGTH];               /* 组播地址（允许应用在运行时配置或修改） */
    int     port;                                   /* 组播端口（允许应用在运行时配置或修改） */
    BOOL    enable;                                 /* 使能（TRUE为无效、FALSE为有效）（允许应用在运行时配置或修改） */
    int     interval;                               /* 数据发布间隔时间（毫秒，0为忽略） */
} VENDER_CONFIG_ENTRY;
/* -------------------------           */


/*
 * 用户权限配置
 */
typedef struct {
    char     username[MON_USERNAME_LENGTH];         /* 用户名 */
    char     password[MON_PASSWORD_LENGTH];         /* 密码 */
    int      role;                                  /* 用户角色 */
} USER_CONFIG_MON;
/* -------------------------           */


/*
 * 前置机网络连通情况信息
 */
typedef struct {
    int      timestamp;                             /* 主机时间*/
    int      gwToQETeStatus;                        /* QE与GW前置机网络连通状态 */
    int      gwToEAIStatus;                         /* EAI总线网络连通状态 */
} NET_CONNECT_INFO_GW;
/* -------------------------           */

/*
 * 行情引擎网络连通情况信息
 */
typedef struct {
    int      timestamp;                             /* 主机时间*/
    int      gwToQETeStatus;                         /* QE与GW前置机网络连通状态 */
} NET_CONNECT_INFO_QE;
/* -------------------------           */

/*
 * 行情引擎系统运行情况信息
 */
typedef struct {
    int      timestamp;                             /* CS主机时间*/
    int      lastTeActiveTimestamp;                 /* TE主机最近连接时间*/
    int      chiefProcessStatus;                    /* 应用系统进程运行状态 */
    int      stubTeProcessStatus;                   /* 数据接收与加工子系统进程运行状态 */
    int      dataVenderProcessStatus;               /* 数据发布子系统进程运行状态 */
    int      dataRebuilderProcessStatus;            /* 数据重传子系统进程运行状态 */
    int      stubMonitorProcessStatus;              /* 监控服务子系统进程运行状态 */

} SYSTEM_RUNNING_INFO_QE;
/* -------------------------           */

/*
 * 前置机系统运行情况信息
 */
typedef struct {
    int      timestamp;                             /* CS主机时间*/
    int      lastTeActiveTimestamp;                 /* TE主机最近连接时间*/
    int      chiefProcessStatus;                    /* 应用系统进程运行状态 */
    int      stubTeProcessStatus;                   /* 数据接收与加工子系统进程运行状态 */
    int      processorProcessStatus;            /* 数据加工子系统进程运行状态 */
    int      venderProcessStatus;                   /* 发布子系统进程运行状态 */
    int      monitorProcessStatus;              /* 监控服务子系统进程运行状态 */

} SYSTEM_RUNNING_INFO_GW;
/* -------------------------           */


/*
 * QE静态文件加载情况信息
 */
typedef struct {
    int      stockIndustryStaticFileStatus;         /* 证券行业对应关系静态文件状态 */
    int      commonBondStaticFileStatus;            /* 普通债券静态文件状态 */
    int      stockInfoStaticFileStatus;             /* 产品基础信息静态文件状态 */
    int      controlSegmentStaticFileStatus;        /* 产品群基础信息静态文件状态 */

} STATIC_FILE_INFO_QE;
/*
 * GW静态文件加载情况信息
 */
typedef struct {
    int      nextDayOrderStatus;                    /* 隔日订单静态文件状态 */

} STATIC_FILE_INFO_GW;
/* -------------------------           */


/*
 * 组播配置
 */
typedef struct {
    int      delaytime;                             /* 组播发送延迟时间 */
} MULTICAST_CONFIG_MON;
/* -------------------------           */


//#pragma HP_ALIGN NOPADDING PUSH
#pragma pack(push,1)

/*
 * 订单簿查询证券结果
 */
typedef struct {
	int 			setId;                                          /* 证券所属set号 */                                         
    int             openPrice;                              		/* 开盘价 */
    int             highPrice;                              		/* 最高价 */
    int             lowPrice;                               		/* 最低价 */
    int             tradeNumber;                            		/* 成交笔数 */
    long long       tradeAmount;                            		/* 成交数量 */
    long long       tradeMoney;                             		/* 成交金额 */
    int             orderBuyNumber;                         		/* 委托买入笔数 */
    long long       orderBuyAmount;                         		/* 委托买入数量 */
    long long       orderBuyMoney;                          		/* 委托买入金额 */
    int             orderSellNumber;                        		/* 委托卖出笔数 */
    long long       orderSellAmount;                        		/* 委托卖出数量 */
    long long       orderSellMoney;                         		/* 委托卖出金额 */
    int             withdrawBuyNumber;                      		/* 买入撤单笔数 */
    long long       withdrawBuyAmount;                      		/* 买入撤单数量 */
    long long       withdrawBuyMoney;                       		/* 买入撤单金额 */
    int             withdrawSellNumber;                     		/* 卖出撤单笔数 */
    long long       withdrawSellAmount;                     		/* 卖出撤单数量 */
    long long       withdrawSellMoney;                      		/* 卖出撤单金额 */
    int             bidNumber;                              		/* 委买价位个数 */
    int             offerNumber;                            		/* 委卖价位个数 */
    int             priceLevelNumber;                           	/* 价位数量*/
                                                                	/* 后续报文为priceLevelNumber个
                                                                       PRICE_INFO_ORDER_BODY_MON结构*/
} PRICE_INFO_MON;
/* -------------------------           */


/*
 * 订单簿查询订单结果
 */
typedef struct {
    int             orderPrice;                                 /* 委托价格*/
    long long       orderAmount;                                /* 委托数量*/
    int             orderNumber;                                /* 委托笔数*/
} PRICE_INFO_ORDER_BODY_MON;
/* -------------------------           */

/*
 * 系统状态结果
 */
typedef struct {
    int             cpuFreeRatio;                                 /* cpu空闲率*/
    int             memFreeVol;                                  /* 内存空闲容量*/
} OP_SYSTEM_STATUS_MON;
/* -------------------------           */

/*
 * 通道发送时间
 */
typedef struct {
    int     id;                                         /* 数据发布器编号 */
    int     averageProcessTime;                         /* 平均处理时间 */
    int     maxPeocessTime;                             /* 最大处理时间 */
    int		numProcessTimeOut;						    /* 时延超过阀值的次数 */
    int     warnTimeDelay;                          /* 时延的阀值，时间格式是HHMMSSmm，因此5秒记录为500mm */
	int		numStatistics;							    /* 统计次数 */    
}CHANNEL_PROCESS_TIME_MON;
/* -------------------------           */
//#pragma HP_ALIGN POP
#pragma pack(pop)

#endif  /* _MODEL_MONITOR_H */
