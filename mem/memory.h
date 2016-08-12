/******************************************************************
** �ļ���:      memory.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.4
** �� ��:       �����ڴ�����������ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
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
 * vender ����״̬����
 */

#define VENDER_SEND_NORMAL     0
#define VENDER_SEND_PAUSED     1
/*
 * �����ڴ��������
 */
typedef struct {
	int		setIdMapMax;								/* setId ����*/
	int		setMax;										/* set ������ */
	int		setSizeMax;									/* ÿ set ���С*/
	int     categoryMax;                                /* ���֤ȯƷ������ */
    int     stockMax;                                   /* ����Ʊ���� */
    #ifdef USE_PBU
	    int		pbuNumberMax;							/* ���pbu���� */
	    int		pbuDataNumberMax;						/* ���pbu�������� */
    #endif
    int     orderMax;                                   /* ���ί������ */
    int     tradeMax;                                   /* ���ɽ����� */
    int     withdrawMax;                                /* ��󳷵����� */
    int     bidMax;                                     /* ���ί���λ���� */
    int     offerMax;                                   /* ���ί����λ���� */
    int     industryMax;                                /* �����ҵ���� */
    int		stockIndexMax;								/* ����Ʊָ������ */
} MEM_CONFIG_SHARE;


/*
 * �����ڴ�������
 */
typedef struct {
    /*
     * ����Ԫ���ڴ�����С
     */
    long    statsSize;                                  /* MEM_STATS ռ�е��ڴ��С */
    long	setIdMapSize;								/* SET_ID_MAP ռ�е��ڴ��С */
    long	setStatsSize;								/* SET_STATS ռ�е��ڴ��С */
    long	setMapSize;									/* SET_MAP ռ�е��ڴ��С */
    long    categorySize;                               /* ֤ȯƷ�� ռ�е��ڴ��С */
    long    stockSize;                                  /* ��Ʊ��Ϣ ռ�е��ڴ��С */
    #ifdef USE_PBU
	    long	pbuFirmSize;							/* pbu���б�ռ�е��ڴ��С */
	    long	pbuTableSize;							/* pbu�����б�ռ�е��ڴ��С */
	    long	pbuBuySortSize;							/* pbu��ί�������б�ռ�е��ڴ��С */
	    long	pbuSellSortSize;						/* pbu��ί�������б�ռ�е��ڴ��С */
	    long	pbuDataSize;							/* pbu������ռ�е��ڴ��С */
    #endif
    long    orderSize;                                  /* Order ռ�е��ڴ��С */
    long    tradeSize;                                  /* Trade ռ�е��ڴ��С */
    long    withdrawSize;                               /* Withdraw ռ�е��ڴ��С */
    long    bidSize;                                    /* ί���λ ռ�е��ڴ��С */
    long    offerSize;                                  /* ί����λ ռ�е��ڴ��С */
    long    aIndustrySize;                              /* a����ҵ ռ�е��ڴ��С */
    long    bIndustrySize;                              /* b����ҵ ռ�е��ڴ��С */
    long	stockIndexSize;								/* ��Ʊָ�� ռ�е��ڴ��С */
	
    /*
     * ����Ԫ����ʼλ�ã�ƫ��λ�ã�
     */
    long    statsOffset;                                /* MEM_STATS ����ʼλ�� */
    long	setIdMapOffset;								/* SET_ID_MAP ����ʼλ�� */
    long	setStatsOffset;								/* setStats ����ʼλ�� */
    long	setMapOffset;								/* setMap ����ʼλ�� */
    long    categoryOffset;                             /* ֤ȯƷ�� ����ʼλ�� */
    long    stockOffset;                                /* ��Ʊ��Ϣ ����ʼλ�� */
    #ifdef USE_PBU
	    long	pbuFirmOffset;							/* pbu�ŵ���ʼλ�� */
	    long	pbuTableOffset;							/* pbu�����б����ʼλ�� */
	    long	pbuBuySortOffset;						/* pbu��ί�������б����ʼλ�� */
	    long	pbuSellSortOffset;						/* pbu��ί�������б����ʼλ�� */
	    long	pbuDataOffset;							/* pbu����������ʼλ�� */
    #endif
    long    orderOffset;                                /* Order ����ʼλ�� */
    long    tradeOffset;                                /* Trade ����ʼλ�� */
    long    withdrawOffset;                             /* Withdraw ����ʼλ�� */
    long    bidOffset;                                  /* ί���λ ����ʼλ�� */
    long    offerOffset;                                /* ί����λ ����ʼλ�� */
    long    aIndustryOffset;                            /* a����ҵ ����ʼλ�� */
    long    bIndustryOffset;                            /* b����ҵ ����ʼλ�� */
	long	stockIndexOffset;							/* ��Ʊָ�� ����ʼλ�� */

} MEM_ALLOCATION_SHARE;


/*
 * �����ڴ浱ǰ״̬
 */
typedef struct {
	char		gwServerIp[32];
    int			gwServerPort;
	int			nextSet;								/* ��һ���е� set ��λ�� */
    int     	nextStock;								/* ��һ���е� Stock �б�λ�� */
    #ifdef USE_PBU
	    int		nextPBUFirm;							/* ��һ���е� pbu ����λ�� */
	    int		nextPBUData;							/* ��һ���е� pbu ����λ��*/
	#endif
    int     	nextOrder;								/* ��һ���е� Order �б�λ�� */
    int     	nextTrade;								/* ��һ���е� Trade �б�λ�� */
    int     	nextWithdraw;							/* ��һ���е� Withdraw �б�λ�� */
    int			nextStockIndex;							/* ��һ���е� stockIndex �б�λ�� */
    int     	nextBid;								/* ��һ���е� Bid �б�λ�� */
    int     	nextOffer;								/* ��һ���е� Offer �б�λ�� */
    int     	nextAIndustry;							/* ��һ���е� nextAIndustry �б�λ�� */
    int     	nextBIndustry;							/* ��һ���е� nextBIndustry �б�λ�� */
    int     	nextCategory;							/* ��һ���е� Category �б�λ�� */
} MEM_CURRENT_SHARE;


typedef struct{
	int							id;						/* ���ݷ�������� */
	long long					maxSnapshotSendTime;	/* ÿ������ͨ������һ����Ϣ�������ʱ�� */
	long long					totalSnapshotSendTime;	/* ÿ������ͨ������һ����Ϣ����Ҫ���ܷ���ʱ�� */
	int							numSendTimeOut;			/* ÿ������ͨ��������Ϣʱ�䳬����ֵ�Ĵ��� */
	int							numStatistics;			/* ÿ������ͨ��ͳ�Ʒ���ʱ��Ĵ��� */
} CHANNEL_SEND_INFO;



/*
 * ��ǰ�������
 */
typedef struct {
    /* ���и����ֵ��MEM_CURRENT_SHARE�ж�Ӧ��ֵ��ͬ���ݲ�ά��
    int     stockNumber;                                / * ��ǰ�ܹ�Ʊ�� * /
    int     orderNumber;                                / * ��ǰ��ί���� * /
    int     tradeNumber;                                / * ��ǰ�ܳɽ��� * /
    int     withdrawNumber;                             / * ��ǰ�ܳ����� * /
    int     bidNumber;                                  / * ��ǰ��ί���λ�� * /
    int     offerNumber;                                / * ��ǰ��ί����λ�� * /
    */
    int     isStockReceived;                            /* ��Ʒ������Ϣ�Ѽ�����ϱ�־ */
    int     isStockIndustryLoaded;                      /* ��Ʊ��ҵ�����Ѽ�����ϱ�־ */
    int		isCommonBondLoaded;
    int		isConvertibleBondLoaded;
    int		isControlSegmentLoaded;						/* ��ƷȺ������Ϣ�Ѽ�����ϱ�־ */
    int		ocallFlag;									/* ���м��Ͼ��۱�� -1: ��ʼ״̬ 0�����м��Ͼ����ڼ� 1�����м��Ͼ��۽��� */
	
//	CS_DATA_SEQUENCE csDataSequence;					/* CS���ѽ��յ��������� */
	
    int     workedOrderNumber;                          /* ��ǰ�Ѵ������ί���� */
    int     workedTradeNumber;                          /* ��ǰ�Ѵ�����ĳɽ��� */
    int     workedWithdrawNumber;                       /* ��ǰ�Ѵ�����ĳ����� */

    int     sentTradeNumber;                            /* ��ǰ�ѷ��͵ĳɽ����� */

    /*
     * ����ͳ����Ϣ
     */
    long long   tradeAmount;                            /* �ܳɽ����� */
    long long	bTradeAmount;							/* b���ܳɽ�����*/
    long long	notBTradeAmount;						/* ��b���ܳɽ�����*/
    long long   tradeMoney;                             /* �ܳɽ���� */
	long long	bTradeMoney;							/* b���ܳɽ����*/
	long long	notBTradeMoney;							/* ��b���ܳɽ����*/
    /*
     * ������Ϣ
     */
    //WITHDRAW_BUY_SORT_LIST      aWithdrawBuySortList;	/* ����ί�����볷������������ί����������10��ί�򳷵��� */
    //WITHDRAW_SELL_SORT_LIST     aWithdrawSellSortList;	/* ����ί��������������������ί����������10��ί�������� */
	//WITHDRAW_BUY_SORT_LIST      bWithdrawBuySortList;	/* ����ί�����볷������������ί����������10��ί�򳷵��� */
    //WITHDRAW_SELL_SORT_LIST     bWithdrawSellSortList;	/* ����ί��������������������ί����������10��ί�������� */
	
	/* 
	 * ����ͳ����Ϣ 
	 */
//	CHANNEL_SEND_INFO			channelSendInfo[VENDER_NUMBER];			/* ����ͨ��ͳ����Ϣ */
	int							lastMarketTime;							/* ��������ʱ�� */
	
} MEM_TRADE_STATS;
/* -------------------------           */


/*
 * CSʱ����TEʱ���¼
 */
typedef struct {
    int     teTime;                                     /* TE ʱ�� */
    int     csTime;                                     /* CS ʱ�� */
    int     windage;                                    /* CS��TE��ʱ��ƫ�� */
} MEM_TIME;
/* -------------------------           */


/*
 * �����ڴ�״̬
 */
typedef struct {
    long                    totalSize;                  /* �����ڴ���ܴ�С */

    int                     venderStatus;
    long long               gwHeartbeat;
    
    /*
     * ϵͳȫ������
     */
   // GLOBAL_DATA             globalData;                 /* ϵͳȫ������ */

    /*
     * �ڴ�״̬
     */
    MEM_CONFIG_SHARE        shareConfig;               /* �����ڴ�������� */
    MEM_ALLOCATION_SHARE    shareAllocation;           /* �����ڴ������� */
    MEM_CURRENT_SHARE       shareCurrent;              /* �����ڴ浱ǰ״̬ */
    MEM_TIME                shareTime;                 /* CSʱ����TEʱ���¼ */
    MEM_TRADE_STATS         shareTradeStats;           /* ��ǰ������� */

} MEM_STATS;
/* -------------------------           */


/*
 * �����ڴ��������
 */
typedef struct {
    MEM_CONFIG_SHARE        shareConfig;               /* �����ڴ�������� */
} MEM_CONFIG_ALL;

/*
 * �����ڴ�������
 */
typedef struct {
    long                    totalSize;                  /* �����ڴ���ܴ�С */
    MEM_ALLOCATION_SHARE    shareAllocation;           /* �����ڴ������� */
} MEM_ALLOCATION_ALL;
/* -------------------------           */


/*
 * ��������
 */

int                    InitShareMemory(const MEM_CONFIG_ALL*);                 /* ��ʼ��ϵͳ�����ڴ� */
void                    ReleaseShareMemory();                                   /* �ͷ�ϵͳ�����ڴ� */
MEM_ALLOCATION_ALL*     EvaluateMemAllocationStats(const MEM_CONFIG_ALL*);      /* ���������ڴ��ʹ����� */

int                    IsShareMemoryReady();                                   /* ����ϵͳ�����ڴ��Ƿ���׼������ */
void*                   GetShareMemory();                                       /* ���ع����ڴ�����ݶο�ʼ��ַ */


MEM_STATS*              GetMemStats();                                          /* ���� MEM_STATS �ṹ */
MEM_CONFIG_SHARE*       GetShareMemConfigStats();                               /* ���� MEM_CONFIG_SHARE �ṹ */
MEM_ALLOCATION_SHARE*   GetShareMemAllocationStats();                           /* ���� MEM_ALLOCATION_SHARE �ṹ */
MEM_CURRENT_SHARE*      GetShareMemCurrentStats();                              /* ���� MEM_CURRENT_SHARE �ṹ */
MEM_TIME*               GetMemTime();                                           /* ���� MEM_TIME �ṹ */
MEM_TRADE_STATS*        GetMemTradeStats();                                     /* ���� MEM_TRADE_STATS �ṹ */

//CS_DATA_SEQUENCE*       GetCsSequence();                                        /* ����CS���ѽ��յ��������� */

void                    UpdateMemTime(int teTime);                              /* ���� MEM_TIME */
int                     GetTeTime();                                            /* ����TE����ʱ�� */
void                PrintEvaluateMemAllocationStats(const MEM_CONFIG_ALL*);     /* ��ӡ�����ڴ�ʹ��������� */
void                PrintMemoryStats();                                         /* ��ӡ�����ڴ�״̬ */
void                PrintMemoryConfigStats();                                   /* ��ӡ�����ڴ�������� */
void                PrintMemoryAllocationStats();                               /* ��ӡ�����ڴ������Ϣ */
void                PrintMemoryCurrentStats();                                  /* ��ӡ�����ڴ浱ǰ״̬ */
void                PrintMemoryTimeStats();                                     /* ��ӡ�����ڴ��е�CSʱ����TEʱ���¼ */
void                PrintMemoryTradeStats();                                    /* ��ӡ�����ڴ浱ǰ������� */


#endif  /* _MEMORY_MANAGER_H */
