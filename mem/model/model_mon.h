/******************************************************************
** �ļ���:      model_mon.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.13
** �� ��:       ����������ϵͳ(monitor)���õ�Model����
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006-5-30     zhuhui      ���Ӽ�ؾ�̬�ļ����ش���ṹ��
** 2007-4-4      wugb        ���Ӽ�ض�ȡsetId
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
 * ���ݳ��ȶ���
 * (Ϊ�������ݿ�϶�����г��ȶ���Ϊ4�ı���)
 */
#define MON_STOCK_NAME_LENGTH               32          /* ��Ʊ���Ƴ��� */
#define MON_USERNAME_LENGTH                 32          /* �û������� */
#define MON_PASSWORD_LENGTH                 40          /* ���볤�� */
#define MON_VENDER_TAG_LENGTH               32          /* ���ݷ�������ǩ���� */
#define MON_VENDER_NAME_LENGTH              64          /* ���ݷ��������Ƴ��� */
#define MON_VENDER_IP_LENGTH                32          /* �鲥��ַ���� */
#define MON_VENDER_MAX_PROCESSORS_NUMBER    64          /* ����������� */

#define MON_STATIC_FILE_LOAD_SUCCESS_STATUS	1          /* ��̬���ݼ��سɹ� */
#define MON_STATIC_FILE_WAIT_LOAD_STATUS	0          /* ��̬���ݼ��صȴ� */
#define MON_STATIC_FILE_LOAD_ERROR_STATUS	-1         /* ��̬���ݼ���ʧ�� */

/*
 * ��������ͨ������������
 */
#define VENDER_NUMBER						50			/* ���ݷ�������ͨ����������� */
#define VENDER_SEND_TIMEOUT					3000000		/* ����һ����������Ҫ��ʱ��ķ�ֵ */
/*
 * �ṹ�嶨��
 */

/*
 * (֤ȯ)Ʒ�ֶ���
 */
typedef struct {
    int     id;                                     /* Ʒ�ִ��� */
    char    name[CATEGORY_NAME_LENGTH];             /* Ʒ������ */
} CATEGORY_MON;
/* -------------------------           */


/*
 * ��ҵ����
 */
typedef struct {
    int     industryNo;                             /* ��ҵ��� */
    char    industryId[INDUSTRY_ID_LENGTH];         /* ��ҵ���� */
    char    industryName[INDUSTRY_NAME_LENGTH];     /* ��ҵ���� */
} INDUSTRY_MON;
/* -------------------------           */


/*
 * ֤ȯƷ������
 */
typedef struct {
    char    stockId[STOCK_ID_LENGTH];               /* ֤ȯ���� */
    char    stockName[MON_STOCK_NAME_LENGTH];       /* ֤ȯ���� */
    int     categoryId;                             /* Ʒ������ */
} STOCK_CATEGORY_MON;
/* -------------------------           */


/*
 * ���ݷ���������
 */
typedef struct {
    int     index;                                  /* ���ݷ�������ţ��豣֤ID�붨��˳��һ�£� */
    int     id;                                     /* ���ݷ�������� */
    char    tag[MON_VENDER_TAG_LENGTH];             /* ���ݷ�������ǩ */
    char    name[MON_VENDER_NAME_LENGTH];           /* ���ݷ��������� */
    char    ip[MON_VENDER_IP_LENGTH];               /* �鲥��ַ������Ӧ��������ʱ���û��޸ģ� */
    int     port;                                   /* �鲥�˿ڣ�����Ӧ��������ʱ���û��޸ģ� */
    BOOL    enable;                                 /* ʹ�ܣ�TRUEΪ��Ч��FALSEΪ��Ч��������Ӧ��������ʱ���û��޸ģ� */
    int     interval;                               /* ���ݷ������ʱ�䣨���룬0Ϊ���ԣ� */
} VENDER_CONFIG_ENTRY;
/* -------------------------           */


/*
 * �û�Ȩ������
 */
typedef struct {
    char     username[MON_USERNAME_LENGTH];         /* �û��� */
    char     password[MON_PASSWORD_LENGTH];         /* ���� */
    int      role;                                  /* �û���ɫ */
} USER_CONFIG_MON;
/* -------------------------           */


/*
 * ǰ�û�������ͨ�����Ϣ
 */
typedef struct {
    int      timestamp;                             /* ����ʱ��*/
    int      gwToQETeStatus;                        /* QE��GWǰ�û�������ͨ״̬ */
    int      gwToEAIStatus;                         /* EAI����������ͨ״̬ */
} NET_CONNECT_INFO_GW;
/* -------------------------           */

/*
 * ��������������ͨ�����Ϣ
 */
typedef struct {
    int      timestamp;                             /* ����ʱ��*/
    int      gwToQETeStatus;                         /* QE��GWǰ�û�������ͨ״̬ */
} NET_CONNECT_INFO_QE;
/* -------------------------           */

/*
 * ��������ϵͳ���������Ϣ
 */
typedef struct {
    int      timestamp;                             /* CS����ʱ��*/
    int      lastTeActiveTimestamp;                 /* TE�����������ʱ��*/
    int      chiefProcessStatus;                    /* Ӧ��ϵͳ��������״̬ */
    int      stubTeProcessStatus;                   /* ���ݽ�����ӹ���ϵͳ��������״̬ */
    int      dataVenderProcessStatus;               /* ���ݷ�����ϵͳ��������״̬ */
    int      dataRebuilderProcessStatus;            /* �����ش���ϵͳ��������״̬ */
    int      stubMonitorProcessStatus;              /* ��ط�����ϵͳ��������״̬ */

} SYSTEM_RUNNING_INFO_QE;
/* -------------------------           */

/*
 * ǰ�û�ϵͳ���������Ϣ
 */
typedef struct {
    int      timestamp;                             /* CS����ʱ��*/
    int      lastTeActiveTimestamp;                 /* TE�����������ʱ��*/
    int      chiefProcessStatus;                    /* Ӧ��ϵͳ��������״̬ */
    int      stubTeProcessStatus;                   /* ���ݽ�����ӹ���ϵͳ��������״̬ */
    int      processorProcessStatus;            /* ���ݼӹ���ϵͳ��������״̬ */
    int      venderProcessStatus;                   /* ������ϵͳ��������״̬ */
    int      monitorProcessStatus;              /* ��ط�����ϵͳ��������״̬ */

} SYSTEM_RUNNING_INFO_GW;
/* -------------------------           */


/*
 * QE��̬�ļ����������Ϣ
 */
typedef struct {
    int      stockIndustryStaticFileStatus;         /* ֤ȯ��ҵ��Ӧ��ϵ��̬�ļ�״̬ */
    int      commonBondStaticFileStatus;            /* ��ͨծȯ��̬�ļ�״̬ */
    int      stockInfoStaticFileStatus;             /* ��Ʒ������Ϣ��̬�ļ�״̬ */
    int      controlSegmentStaticFileStatus;        /* ��ƷȺ������Ϣ��̬�ļ�״̬ */

} STATIC_FILE_INFO_QE;
/*
 * GW��̬�ļ����������Ϣ
 */
typedef struct {
    int      nextDayOrderStatus;                    /* ���ն�����̬�ļ�״̬ */

} STATIC_FILE_INFO_GW;
/* -------------------------           */


/*
 * �鲥����
 */
typedef struct {
    int      delaytime;                             /* �鲥�����ӳ�ʱ�� */
} MULTICAST_CONFIG_MON;
/* -------------------------           */


//#pragma HP_ALIGN NOPADDING PUSH
#pragma pack(push,1)

/*
 * ��������ѯ֤ȯ���
 */
typedef struct {
	int 			setId;                                          /* ֤ȯ����set�� */                                         
    int             openPrice;                              		/* ���̼� */
    int             highPrice;                              		/* ��߼� */
    int             lowPrice;                               		/* ��ͼ� */
    int             tradeNumber;                            		/* �ɽ����� */
    long long       tradeAmount;                            		/* �ɽ����� */
    long long       tradeMoney;                             		/* �ɽ���� */
    int             orderBuyNumber;                         		/* ί��������� */
    long long       orderBuyAmount;                         		/* ί���������� */
    long long       orderBuyMoney;                          		/* ί�������� */
    int             orderSellNumber;                        		/* ί���������� */
    long long       orderSellAmount;                        		/* ί���������� */
    long long       orderSellMoney;                         		/* ί��������� */
    int             withdrawBuyNumber;                      		/* ���볷������ */
    long long       withdrawBuyAmount;                      		/* ���볷������ */
    long long       withdrawBuyMoney;                       		/* ���볷����� */
    int             withdrawSellNumber;                     		/* ������������ */
    long long       withdrawSellAmount;                     		/* ������������ */
    long long       withdrawSellMoney;                      		/* ����������� */
    int             bidNumber;                              		/* ί���λ���� */
    int             offerNumber;                            		/* ί����λ���� */
    int             priceLevelNumber;                           	/* ��λ����*/
                                                                	/* ��������ΪpriceLevelNumber��
                                                                       PRICE_INFO_ORDER_BODY_MON�ṹ*/
} PRICE_INFO_MON;
/* -------------------------           */


/*
 * ��������ѯ�������
 */
typedef struct {
    int             orderPrice;                                 /* ί�м۸�*/
    long long       orderAmount;                                /* ί������*/
    int             orderNumber;                                /* ί�б���*/
} PRICE_INFO_ORDER_BODY_MON;
/* -------------------------           */

/*
 * ϵͳ״̬���
 */
typedef struct {
    int             cpuFreeRatio;                                 /* cpu������*/
    int             memFreeVol;                                  /* �ڴ��������*/
} OP_SYSTEM_STATUS_MON;
/* -------------------------           */

/*
 * ͨ������ʱ��
 */
typedef struct {
    int     id;                                         /* ���ݷ�������� */
    int     averageProcessTime;                         /* ƽ������ʱ�� */
    int     maxPeocessTime;                             /* �����ʱ�� */
    int		numProcessTimeOut;						    /* ʱ�ӳ�����ֵ�Ĵ��� */
    int     warnTimeDelay;                          /* ʱ�ӵķ�ֵ��ʱ���ʽ��HHMMSSmm�����5���¼Ϊ500mm */
	int		numStatistics;							    /* ͳ�ƴ��� */    
}CHANNEL_PROCESS_TIME_MON;
/* -------------------------           */
//#pragma HP_ALIGN POP
#pragma pack(pop)

#endif  /* _MODEL_MONITOR_H */
