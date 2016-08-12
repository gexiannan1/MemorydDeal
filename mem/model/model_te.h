/******************************************************************
** �ļ���:      model_te.h
** �� ��:       0.1
** ������:      Song Fang
**              zhaodj
** �� ��:       2005.9.8
** �� ��:       ��TEϵͳ�����Model����
** �޶���¼��
** �޶�����     �޶���          SIR         �޸���bug�����޶�ԭ��
����----------------------------------------------------------------
** 2011.05.10   wangyx          00016       ���ӽ�����ָ��
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
 * TE(����ϵͳ)���ݳ��ȶ���
 */
#define TE_STOCK_ID_LENGTH                  6
#define TE_STOCK_SHORTNAME_LENGTH           8
#define TE_ORDER_FIRM_LENGTH                5
#define IDC_END_OF_DAY_MARKER_LENGTH       12          /* ������־����*/
/* -------------------------           */



/*
 * ��������
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

#define STOCK_TYPE_BOND                     1           /* ծȯ */
#define STOCK_TYPE_ASHARE                   2           /* A�� */
#define STOCK_TYPE_BSHARE                   3           /* B�� */
#define STOCK_TYPE_INDEX                    4           /* ָ�� */
#define STOCK_TYPE_RIGHTS                   5           /* */
#define STOCK_TYPE_RIGHTSHOLD               6           /* */
#define STOCK_TYPE_NIHUIGOU                 7           /* ��ع� */
#define STOCK_TYPE_ETF                      8           /* ETF */
#define STOCK_TYPE_SJSPEISHOU               9           /* ������� */
#define STOCK_TYPE_ZAIZHUANGU               10          /* ծת�� */
#define STOCK_TYPE_XINGU                    11          /* �¹� */
#define STOCK_TYPE_ZHENGHUIGOU              12          /* ���ع� */
#define STOCK_TYPE_KFJIJIN                  13          /*  */
#define STOCK_TYPE_QUANZHENG_JY             14          /*  */
#define STOCK_TYPE_QUANZHENG_EXCUTE         15          /* ԭ����ΪȨ֤��Ȩ�����ڱ�ԤԼ�չ���ҵ���� */
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
#define ORDER_STATUS_PART_WITHDRAWN			'I'			 /*���ֳ�����־*/
#define ORDER_STATUS_CALL_PRICE_OVER        'A'          /*���Ͼ��۽�����־*/

#define ORDER_ORDERTYPE_ICE  			    'I'			//��ɽ����
#define ORDER_ORDERTYPE_MARKET  		    'M'			//M - �м�
#define ORDER_ORDERTYPE_LIMIT  			    'L'			//L - �޼�
#define ORDER_ORDERTYPE_MTOL  			    'T'			//T - �м�ת�޼�
#define ORDER_ORDERTYPE_QUOTE  			    'Q'			//Q - ����
/* -------------------------           */




//#pragma HP_ALIGN NOPADDING PUSH
#pragma pack(push,1)

/*
 * ORDER_TE����TEϵͳ�������Ϣ��
 */
typedef struct {
    unsigned long long		orderNo;                                    /* ������ */
    unsigned long long		orderNoOld;									/* �ɶ����� */
    unsigned long long		orderNoOld1;								/* �ɶ����� */
    int						orderProcessNo;								/* �������к� */
    int						orderProcessNoOld;							/* �ɶ������к� */
    int						orderProcessNoOld1;							/* �ɶ������к� */
    int			            orderTime;                                  /* ��������ʱ�� */
    char					isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ��*/
    char    				pbuId[PBU_FIRM_LENGTH];           			/* pbu�� */
    char    				orderBSType;                            	/* ������־ - BUY �� SELL */
    char    				orderType;                       			/* �������� */
    long long  				orderPrice;                             	/* �۸� */
    long long  				orderQty;                               	/* ���� */
    long long				ordExeQty;									/* ��ִ������ */
    char					trdResTypCod[3];							/* ������������ */
    char					trnTypId;									/* �������� */
    char					orderStatus; 								/* ����״̬ */
    long long				remPeakQty;									/* ʣ���ʾ���� */
    long long				peakSizeQty;								/* ����ʾ���� */
    char					ordrResCod;									/* ������������ */
} ORDER_TE;
/* -------------------------           */

/*
 * �����޸���Ϣ
 */
typedef ORDER_TE ORDER_CHANGE_TE;

/*
 * MARKET_STATUS_TE����TEϵͳ�������Ϣ��
 */
typedef struct {
    char					isinCod[STOCK_ISIN_CODE_LENGTH];		/* ��Ʊ����֤ȯ��ʶ��*/
    char    				tradePhase[6];                         /* tradeʱ�� */
} MARKET_STATUS_TE;
/* -------------------------           */

/*
 * TRADE_TE����TEϵͳ�������Ϣ��
 */
typedef struct {
    unsigned long long			tradeNo;                                    /* ���׺� */
    int				            tradeTime;                                  /* ���׷���ʱ�䣬hhmmss */
    int							tradeProcessNo;								/* �ɽ������ */
    char						isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ��*/
    char    					pbuId[PBU_FIRM_LENGTH];           			/* pbu�� */
    unsigned long long			orderNo;                                    /* ���׶�Ӧ�Ķ����� */
    char						orderType;									/* ���׶�Ӧ�Ķ��������� */
    int							orderProcessNo;								/* �ɽ���Ӧ�Ķ�������� */
    long long					price;                                      /* �۸� */
    long long					qty;                                        /* ���� */
    long long					mktVal;										/* �ɽ���� */
    char						type;                                       /* �������� */
    char						trnTypId;									/* �������� */
    char						tradeBSType;								/* ������������ */
    long long					remPeakQty;									/* ʣ���ʾ���� */
    long long					peakSizeQty;								/* ����ʾ���� */
    unsigned long				ordrEntTim;									/* ��ɽ����������¶ʱ�� */
} TRADE_TE;
/* -------------------------           */


/*
 * ����ָ��������ṹ
 */
typedef struct {
    char			isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ�� */
    long long       latestClosingIndex;                         /* ǰ����ָ�� */
    long long       openingIndex;                               /* ����ָ�� */ 
    long long       tradeMoney;                                 /* ���������Ӧָ���ĳɽ���� */
    long long       indexHigh;                                  /* ���ָ�� */
    long long       indexLow;                                   /* ���ָ�� */
    long long       latestIndex;                                /* ����ָ�� */
/*** SIR 00016 BEGIN */
	long long		closeIndex;									/* ������ָ�� */
/*** SIR 00016 END */
	char			tradeTime[9];								/* ����ʱ�� */
    long long       tradeAmount;                                /* ���������Ӧָ���Ľ������� */
    long long		netChg;										/* ָ���ǵ�ֵ */
    long long		ratioChg;									/* ָ���ǵ����ȣ����� */
} DETAILED_INDEX;
/* -------------------------           */


/*
 * �г�������Ϣ
 */
typedef struct {
	char				isinCod[STOCK_ISIN_CODE_LENGTH];			/* ��Ʊ����֤ȯ��ʶ�� */
	long long			openPrice;									/* ���̼� */
	long long			closePrice;									/* ���̼� */
	long long			prevClosingPrice;							/* ǰ���̼� */
	long long			highPrice;									/* ��߼� */
	long long			lowPrice;									/* ��ͼ� */
	long long			lastPrice;									/* ���¼� */
	long long			netChg;										/* �ǵ�ֵ */
	long long			fixPrc;										/* �ڹ̶�����ʱ���ڵ�ִ�м۸� */
	unsigned long		tradeNumber;								/* �ɽ����� */
	long long			tradeAmount;								/* �ɽ����� */
	long long			tradeMoney;									/* �ɽ��ܶ� */
	char				moiInd;										/* �г������жϱ�ʶ */
	char				prcsStsValCod[6];							/* Ʒ��״̬���� */
	char				bstOrdPrcInd;								/* ���ί�м۸��ʶ */
	long long			iopv;										/* ETF��ֵ */
	long long			totWarExQty;								/* Ȩ֤��Ȩ������ */
	long long			totWarClQty;								/* Ȩ֤ע�������� */
	long long			totWarWrQty;								/* Ȩ֤���������� */
	long long			openInterest;								/* ������/Ŀǰ��Ϣ */
	long long			deriLstStlPrc;								/* ǰһ�ʽ���� */
	long long			virtualOpenPrice;							/* ���⿪�̲ο��۸� */
	long long			virtualMatchAmount;							/* ����ƥ���� */
	long long			virtualUnMatchAmount;						/* ����δƥ���� */
	char				srpBidAskInd;								/* ����δƥ���������� */
	char				volInd;										/* �����Ա�ʶ */
} INSIDE_MARKET;
/* -------------------------           */


/*
 * CS���ѽ��յ���������
 */
typedef struct {
    int     receivedSetDataNumber;					/* CS���ѽ��յ���SET���� */
    int     receivedCommonDataNumber;				/* CS���ѽ��յ��Ĺ���������Ϣ���� */
    
} CS_DATA_SEQUENCE;
/* -------------------------           */

//#pragma HP_ALIGN POP
#pragma pack(pop)

#endif  /* _MODEL_TE_H */
