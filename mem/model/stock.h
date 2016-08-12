/******************************************************************
** �ļ���:      stock.h
** �� ��:       0.1
** ������:      Song Fang
**              zhaodj
** �� ��:       2005.9.6
** �� ��:       Stock��Ϣ�����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���          SIR         �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.1.4     Song Fang                   ���ӷ��ص���ģʽ�ĳɽ���Ϣ�Ľӿ�
** 2006.03.23   Wei Huanyin                 1������lastPrice�ֶδ���
** 2006.04.15   Wei Huanyin                 1�����ӵ���ծȯ��̬���ݱ�־������
** 2006.05.30   Wei Huanyin                 1�����Ӽ��Ͼ��۽���ʱ�������
** 2006.06.15   zhuhui 		                1���������⼯�Ͼ����������
** 2006.06.16   zhuhui		                1�����Ӹ������⼯�Ͼ�����Ϣ����
** 2007.04.04	Li Bo		                ��STOCK_INFO��STOCK_CS������setId
** 2011.05.10   Hu Ping         00016       ����ʵʱ����ָ��������ɽ���Ҫʱ��
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
 * �Ƿ�ʹ��PBU����,�����������ʹ�ã�û������ʹ�� 
 */
//#define				USE_PBU


/*
 * ��������
 */
#define     ORDER_TRADING_RESTRICTION_OA  			"OA"			//ֻ�μӿ��̼��Ͼ���
#define     ORDER_TRADING_RESTRICTION_AU			"AU"			//���Ͼ����ڼ���Ч
#define     ORDER_TRADING_RESTRICTION_SU   			"SU"			//�ڶ�����ƽ��ʱ������
#define     ORDER_TRADING_RESTRICTION_FP 			"FP"			//�ڹ̶��۸�ʱ����Ч
#define     ORDER_TRADING_RESTRICTION_SPACE 		"  "			//������

#define		STOCK_TRADING_RESTRICTION_PRETR			"PRETR"			//PRETR - ��ǰ
#define     STOCK_TRADING_RESTRICTION_OCALL			"OCALL"			//OCALL - ���м��Ͼ���
#define     STOCK_TRADING_RESTRICTION_ICALL			"ICALL" 		//ICALL - ���м��Ͼ���
#define     STOCK_TRADING_RESTRICTION_IOBB 			"IOBB"  		//IOBB - ���ж�����ƽ�⣬
#define     STOCK_TRADING_RESTRICTION_IPOBB 		"IPOBB" 		//IPOBB - ���ж�����ƽ��ǰ�ڣ�
#define     STOCK_TRADING_RESTRICTION_OOBB 			"OOBB" 			//OOBB  - ���ж�����ƽ��
#define     STOCK_TRADING_RESTRICTION_OPOBB 		"OPOBB" 		//OPOBB - ���ж�����ƽ��ǰ��
#define     STOCK_TRADING_RESTRICTION_FCALL 		"FCALL" 		//FCALL - �̶��۸񼯺Ͼ���
#define     STOCK_TRADING_RESTRICTION_TRADE 		"TRADE" 		//TRADE - ��������
#define     STOCK_TRADING_RESTRICTION_BREAK 		"BREAK" 		//BREAK - ����
#define     STOCK_TRADING_RESTRICTION_HALT 			"HALT" 			//HALT - ͣ��
#define     STOCK_TRADING_RESTRICTION_SUSP 			"SUSP" 			//SUSP - ͣ��
#define     STOCK_TRADING_RESTRICTION_CLOSE 		"CLOSE" 		//CLOSE - ����
#define     STOCK_TRADING_RESTRICTION_POSTR			"POSTR" 		//POSTR - �̺���
#define     STOCK_TRADING_RESTRICTION_ENDTR			"ENDTR" 		//ENDTR - ��������
#define     STOCK_TRADING_RESTRICTION_BETW			"BETW" 			//BETW - ���׼�

#define		OCALL_FLAG_INIT							-1				//���м��Ͼ��۱�ǳ�ʼ״̬
#define		OCALL_FLAG_PHASE						0				//���м��Ͼ��۱���ڼ�
#define		OCALL_FLAG_END							1				//���м��Ͼ��۱�ǽ���

#define		PROCESS_STUB							0				//������ӹ����̵ı�ţ���ȡ�����ʱʹ�ã�
#define		PROCESS_VENDER							1				//���ͽ��̵ı�ţ���ȡ�����ʱʹ�ã�

/*
 * STOCK_INFO
 */
typedef struct {
	int				setId;									/* ֤ȯ����set�� */
	char			isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ�� */
	int				isinIndex;									/* ��Ʊ����֤ȯ��ʶ������ */
	char			stockId[STOCK_ID_LENGTH];               	/* ֤ȯ���� */
    char			stockShortName[STOCK_SHORTNAME_LENGTH]; 	/* ֤ȯ��� */
    char			conSegCode[5];								/* ��ƷȺ */
    int				lotSize;                                	/* ���׵�λ��С */
} STOCK_INFO;
/* ------------------------------------------------- */

/*
 * ��Ʊͳ����Ϣ
 */
typedef struct {
	int				number;                         /* ���� */
    long long		amount;                         /* ���� */
    long long		money;                          /* ��� */
} STOCK_STAT;


/*
 * ծȯ��Ϣ
 */
typedef struct {
	int			cashDate;					/* ���ڶҸ���-*/
	int			cashFrequency;				/* ��ϢƵ��*/
	int			interestpaymentDate;		/* ��Ϣ��*/
	int			bondValue;					/* ծȯ��ֵ*/
	int			boundInterestRate;			/* ծȯ������*/
	int			startDate;					/* ��Ϣ��-*/
	int			bondPublishPrice;			/* ծȯ���м�*/
	int			bondType;					/* ծȯ����0:�̶�����1����������2����Ϣծȯ3��һ���Ի�����Ϣծȯ*/
	int			preInterestpaymentDate;		/* ��һ���۸�Ϣ��*/
	int			nextInterestpaymentDate;	/* ��һ���۸�Ϣ��*/
	int			categoryType;				/* ծȯƷ������*/
	int			workDate;					/* ��ǰ������*/
	int 		yieldToMaturity;			/* ����������*/
	int			preLastPrice;				/* ��һ�ξ���*/
} BOND_CS;
/*
 * Stock��Ϣ
 */
typedef struct {
    /*
     * ��TEϵͳ�������Ϣ
     * Ϊ���Ͻṹ�����ֱ��, δֱ������ STOCK_TE �ṹ
     * <b>
     * �� STOCK_TE �ṹ�����ı�, ��Ҫͬ���޸ĸ�����
     * </b>
     */
    int				stockNo;								/* TE stock����ڣ���0��ʼ�� */
	char			isinCod[STOCK_ISIN_CODE_LENGTH];		/* ��Ʊ����֤ȯ��ʶ�� */
    int				isinIndex;								/* ��Ʊ����֤ȯ��ʶ������ */
    char			stockId[STOCK_ID_LENGTH];				/* ֤ȯ���� */
    char			stockShortName[STOCK_SHORTNAME_LENGTH];	/* ֤ȯ��� */
    int				setId;									/* ֤ȯ����set�� */
    int				stockType;								/* ֤ȯ���� */
   
    int				lotSize;								/* ���׵�λ��С */
    char			currencyType[CURRENCY_TYPE_LENGTH];		/* �������� */
    char			conSegCode[5];							/* ��ƷȺ */
    char			matchingPriority;						/* ������ȼ� */
    /* -------------------------           */

    int				categoryNo;								/* Ʒ�ֱ���� */
    int				industryNo;								/* ��ҵ����� */
    long long		openPrice;								/* ���̼� */
    long long		closePrice;								/* ���̼� */
    long long		prevClosingPrice;						/* ǰ���̼� */
    long long		highPrice;								/* ��߼� */
    long long		lowPrice;								/* ��ͼ� */
    long long		lastPrice;								/* ���¼� */
    long long		netChg;									/* �ǵ�ֵ */
   	long long		fixPrc;									/* �ڹ̶�����ʱ���ڵ�ִ�м۸� */
    BOND_CS			bond;									/* ծȯ��Ϣ */
    int				tradeNumber;							/* �ɽ����� */
    long long   	tradeAmount;							/* �ɽ����� */
    long long   	tradeMoney;								/* �ɽ���� */
    int				marketTradeNumber;						/* �м۳ɽ����� */
    long long   	marketTradeAmount;						/* �м۳ɽ����� */
    long long   	marketTradeMoney;						/* �м۳ɽ���� */
    int				marketTradeBuyNumber;					/* �м���ɽ����� */
    long long		marketTradeBuyAmount;					/* �м���ɽ����� */
    long long		marketTradeBuyMoney;					/* �м���ɽ���� */
    int				marketTradeSellNumber;					/* �м����ɽ����� */
   	long long		marketTradeSellAmount;					/* �м����ɽ����� */
   	long long		marketTradeSellMoney;					/* �м����ɽ���� */
   
    char			moiInd;									/* �г������жϱ�ʶ */
    char			commonPrcsStsValCod[6];					/* ���������г�״̬���� */
    char			prcsStsValCod[6];						/* �г�״̬���� */
    char			prePrcsStsValCod[6];					/* ��һ���г�״̬���� */
    char			volaIndicator[3];						/* Vola�����Ա�ʶ */
    char			bstOrdPrcInd;							/* ���ί�м۸��ʶ */
    long long		iopv;									/* ETF��ֵ */
    
    //2010.5.5 ����etf�깺��ر��������������
    int				etfBuyNumber;							/* ETF�깺���� */
    long long		etfBuyAmount;							/* ETF�깺���� */
    long long		etfBuyMoney;							/* ETF�깺��� */
    int				etfSellNumber;							/* ETF��ر��� */
    long long		etfSellAmount;							/* ETF������� */
    long long		etfSellMoney;							/* ETF��ؽ�� */
   
    long long		totWarExQty;							/* Ȩ֤��Ȩ������ */
    long long		totWarClQty;							/* Ȩ֤ע�������� */
    long long		totWarWrQty;							/* Ȩ֤���������� */
    int	            lastTradeTime;							/* ���һ�ʽ��׵�ʱ��*/
/*** SIR 00016 BEGIN */
    int             bidTradeMaxDuration;                    /* ���ɽ���Ҫʱ��*/
    int             offerTradeMaxDuration;                  /* ����ɽ���Ҫʱ��*/
/*** SIR 00016 END */   
    long long		openInterest;							/* ������/Ŀǰ��Ϣ */
    long long		deriLstStlPrc;							/* ǰһ�ʽ���� */

   
    long long       lstAuctPrc;                           	/* ���¾��ۼ۸�*/
    long long       lstAuctQty;                         	/* ���¾�������*/
    long long       surplusQty;                    			/* ����ӯ��*/
    char       	    surplusBidAskIndicator;    				/* ����ӯ�෽��*/
   
   
    int				orderBuyNumber;                         /* ��ǰ����ʱ��ί��������� */
    long long		orderBuyAmount;                         /* ��ǰ����ʱ��ί���������� */
    long long		orderBuyMoney;                          /* ��ǰ����ʱ��ί�������� */
    int				orderSellNumber;                        /* ��ǰ����ʱ��ί���������� */
    long long		orderSellAmount;                        /* ��ǰ����ʱ��ί���������� */
    long long		orderSellMoney;                         /* ��ǰ����ʱ��ί��������� */
    int				withdrawBuyNumber;                      /* ���볷������ */
    long long		withdrawBuyAmount;                      /* ���볷������ */
    long long		withdrawBuyMoney;                       /* ���볷����� */
    int				withdrawSellNumber;                     /* ������������ */
    long long		withdrawSellAmount;                     /* ������������ */
    long long		withdrawSellMoney;                      /* ����������� */

    int				bidNumber;                              /* ί���λ���� */
    int				offerNumber;                            /* ί����λ���� */

    /* -------------------------           */

    /*
     * �ṹָ��
     */
    int         nextCategory;								/* ͬȯ����һֻ��Ʊ */
   
    //level2����>>>>
    int         channelId;                                  /* ����Ʊ����ͨ���ı�� */
    int         channelNextStock;                           /* ͬһ��ͨ����һֻ��Ʊ */

    int         firstOrderBuy;								/* ��һ���� */
    int         lastOrderBuy;								/* ���һ���� */

    int         firstOrderSell;								/* ��һ������ */
    int         lastOrderSell;								/* ���һ������ */

    int         firstTrade;									/* ��һ�ʽ��� */
    int         lastTrade;									/* ���һ�ʽ��� */

    int         firstWithdraw;								/* ��һ�ʳ��� */
    int         lastWithdraw;								/* ���һ�ʳ��� */

    int         highestBid;									/* ��߾���� */
    int         lowestBid;									/* ��;���� */

    int         lowestOffer;								/* ��;����� */
    int         highestOffer;								/* ��߾����� */

	#ifdef USE_PBU
		int			pbuTradeBuySort[PBU_TRADE_SORT_LENGTH];		/* pbu��ɽ����� */
		int			pbuTradeSellSort[PBU_TRADE_SORT_LENGTH];	/* pbu���ɽ����� */
		int			lastBuyOrderSortOffset;						/* ���һ���Ѵ��pbu��ί��������λ�� */
		int			lastSellOrderSortOffset;					/* ���һ���Ѵ��pbu��ί��������λ�� */
	#endif
    /* -------------------------           */
   
	STOCK_STAT	tradePhaseStat[8];							/* ����ʱ��ͳ����Ϣ  0,1ΪOA��2,3ΪAU��4,5ΪSU��6,7ΪFP */

	BOOL		lockFLag[2];								/* ����� */
	int			turn;										/* ��Ҫ��ȡ���Ľ��̱�� */
} STOCK_CS;


/*
 * ��������
 */
STOCK_CS*   GetStockList();                                 /* ����Stock�������ʼλ�� */
STOCK_CS*   GetStock(int index);                            /* ����ָ��λ�õ�STOCK_CS */
STOCK_CS*   GetStockByIsinCod(char *isinCod);               /* ����ָ��isinCod��Ӧ��STOCK_CS */
STOCK_CS*	GetStockByStockId(char *stockId);				/* ����ָ��stockId��Ӧ��STOCK_CS */
STOCK_CS*	GetStockByIsinIndex(int isinIndex);				/* ����ָ��isinIndex��Ӧ��STOCK_CS */
int         GetStockNumber();                               /* ����Stock���� */

int         AddStock(STOCK_INFO*);                            				/* ��Stock����������Stock��Ŀ */
BOOL        UpdateStockCategory(STOCK_CATEGORY_MON*);       				/* ����֤ȯƷ������ */
//BOOL        UpdateStockIndustry(STOCK_INDUSTRY*,int shareType);             /* ����֤ȯ��ҵ���� */

BOOL        IsShare(STOCK_CS*);                             /* ����stockNo��Ӧ��֤ȯ�����Ƿ��ǹ�Ʊ */
BOOL        IsAShare(STOCK_CS*);                             /* ����stockNo��Ӧ��֤ȯ�����Ƿ���A�� */
BOOL        IsBShare(STOCK_CS*);                             /* ����stockNo��Ӧ��֤ȯ�����Ƿ���B�� */
BOOL        IsIndex(STOCK_CS*);                             /* ����stockNo��Ӧ��֤ȯ�����Ƿ���ָ�� */
BOOL        IsValidStock(STOCK_CS*);                  		/* ����stockNo��Ӧ��֤ȯ�Ƿ���Ч��ƷƷ�� */
BOOL    	IsTradePhaseBB(STOCK_CS*);						/* �����Ƿ��ǲ�Ʒ����ʱ�� */
BOOL    	IsTradePhaseTr(STOCK_CS*);						/* �����Ƿ��ǲ�Ʒ����ʱ�� */
BOOL		CheckTradePhase(STOCK_CS*);						/* ���֤ȯ�Ľ���ʱ���Ƿ���Ҫ���Է��ͣ��Ƿ���Ҫ��⵹�ң� */
BOOL		IsPriceAcross(STOCK_CS*);						/* ���֤ȯ���Ƿ���ڼ۸񽻲棨�Ƿ񵹹ң� */

BOOL        IsStockReceived();                              /* ���ز�Ʒ������Ϣ�����Ƿ��Ѽ������ */
void        SetStockReceived(BOOL);                         /* ���ò�Ʒ������Ϣ�����Ƿ��Ѽ������ */



BOOL        IsStockIndustryLoaded();                        /* ���ع�Ʊ��ҵ�����Ƿ��Ѽ������ */
void        SetStockIndustryLoaded(BOOL);                   /* ���ù�Ʊ��ҵ�����Ƿ��Ѽ������ */

BOOL		IsCommonBondLoaded();							/* ������ͨծȯ�����Ƿ��Ѽ������ */
void		SetCommonBondLoaded(BOOL);						/* ������ͨծȯ�����Ƿ��Ѽ������ */
BOOL		IsConvertibleBondLoaded();						/* ���ؿ�ת��ծȯ�����Ƿ��Ѽ������ */
void		SetConvertibleBondLoaded(BOOL);					/* ���ÿ�ת��ծȯ�Ƿ��Ѽ������ */

BOOL        IsControlSegmentLoaded();                       /* ���ز�ƷȺ������Ϣ�����Ƿ��Ѽ������ */
void        SetControlSegmentLoaded(BOOL);                  /* ���ò�ƷȺ������Ϣ�����Ƿ��Ѽ������ */
int   		IsvaidOfferNumber(STOCK_CS*);					/* ��ǰ����ʱ�ε���Ч��λ������ */
int			IsvaidBidNumber(STOCK_CS*); 					/* ��ǰ����ʱ�ε���Ч��λ������ */

void		PrintStock(STOCK_CS*);							/* ��ӡ֤ȯ��Ϣ����־�� */	

void		StockLock(STOCK_CS*, int);						/* ��ȡ����� */
void		StockUnlock(STOCK_CS*, int);					/* �ͷ������ */

BOOL        PrintStockInfoToFile(int);		                /* ��ӡ��Ʊ��Ϣ��ָ�����ļ��� */
BOOL        PrintStockToFile();		                   		/* ��ӡ��Ʊ��Ϣ��ָ�����ļ��� */
BOOL		PrintInsideMarketToFile();						/* ��ӡ�г�������Ϣ��ָ�����ļ� */
BOOL		PrintShow2007SnapshotToFile();					/* ��ӡshow2007���� */

#endif  /* _MODEL_STOCK_H */
