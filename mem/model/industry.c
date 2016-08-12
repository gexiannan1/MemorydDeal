/******************************************************************
** �ļ���:      industry.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       Industry��Ϣ����
** �޶���¼��	
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
** 2006.1.4     Song Fang   ���ӷ��ص���ģʽ�ĳɽ���Ϣ�Ľӿ�
** 2006.9.4     wugb        ������ҵ����A��B��
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/industry.c,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include    "industry.h"
#include    "category.h"
#include    "model.h"
#include    "../memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"

static const char   _moduleName[] = "industry";
/*
 * ��������
 */
#define MAX_INDUSTRY_LIST_LENGTH        100         /* �����ҵ�� */


/*
 * ����
 */
static int  _sortedIndustryListA[MAX_INDUSTRY_LIST_LENGTH];
static int  _sortedIndustryListB[MAX_INDUSTRY_LIST_LENGTH];


/*
 * �ڲ���������
 */
static void     InitIndustry(INDUSTRY_CS*); 


/*****************************************************************
** ������:      GetIndustryList
** �� ��:       ��
** �� ��:       INDUSTRY_CS[]
** ��������:    ����Industry�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
INDUSTRY_CS*
GetIndustryList(int shareType) {
	
	if(shareType==CATEGORY_ID_ASHARE)
    	return (INDUSTRY_CS*) GetShareMemory() + GetShareMemAllocationStats()->aIndustryOffset;
    else
    	return (INDUSTRY_CS*) GetShareMemory() + GetShareMemAllocationStats()->bIndustryOffset;
}


/*****************************************************************
** ������:      GetIndustry
** �� ��:       ��
** �� ��:       INDUSTRY_CS*
** ��������:    ����ָ��λ�õ�INDUSTRY_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:		
** �� ��:		
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ����shareType ��ҵ����A��B��
****************************************************************/
INDUSTRY_CS*
GetIndustry(int index,int shareType) {
	
    if (index < 0 || index >= GetShareMemConfigStats()->industryMax) {
        return NULL;
    }
    return GetIndustryList(shareType) + index;
}


/*****************************************************************
** ������:      GetIndustryNumber
** �� ��:       ��
** �� ��:       Industry����
** ��������:    ����Industry����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
int
GetIndustryNumber(int shareType) {
	
	if(shareType==CATEGORY_ID_ASHARE)
    	return GetShareMemCurrentStats()->nextAIndustry;
    else if(shareType==CATEGORY_ID_BSHARE)
    	return GetShareMemCurrentStats()->nextBIndustry;
    else
    	return -1;
    	
}  


/*****************************************************************
** ������:      AddIndustry
** �� ��:       ��
** �� ��:       ������Industry��λ��
** ��������:    ��Industry����������Industry��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:		
** �� ��:		
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
int
AddIndustry(INDUSTRY_MON *industry,int shareType) {
    INDUSTRY_CS *target = NULL;
    int         index ;

	if(shareType==CATEGORY_ID_ASHARE)
    	index=GetShareMemCurrentStats()->nextAIndustry;
    else
    	index=GetShareMemCurrentStats()->nextBIndustry;
	/*
     * ����ڴ��Ƿ�Խ��
     */
    if (index >= GetShareMemConfigStats()->industryMax) {
    	FATAL("Industry.AddIndustry()", "��ҵ��������Ԥ����������ҵ������ Current Number: %d"
    			, index);
    	return -1;
    }

    target = GetIndustry(index,shareType);
    memcpy((char*) target, (char*) industry, sizeof(INDUSTRY_MON));
    InitIndustry(target);
	
	if(shareType==CATEGORY_ID_ASHARE)
    	GetShareMemCurrentStats()->nextAIndustry++;
    else if(shareType==CATEGORY_ID_BSHARE)
    	GetShareMemCurrentStats()->nextBIndustry++;
	
    return index;
}


/*****************************************************************
** ������:      GetIndustryShareTradeSortList
** �� ��:       ��
** �� ��:       INDUSTRY_SHARE_TRADE_SORT_LIST
** ��������:    ������ҵ���ɳɽ���������б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
INDUSTRY_SHARE_TRADE_SORT_LIST*
GetIndustryShareTradeSortList(int index,int shareType) {
    return &GetIndustry(index,shareType)->tradeSortList;
}


/*****************************************************************
** ������:      InitSortedIndustryListA
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ��������A��Industry����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.11
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
void
InitSortedIndustryListA() {
    int     i               = 0;
    int     j               = 0;
    int     k               = 0;
    int     industryNumber  = GetIndustryNumber(CATEGORY_ID_ASHARE);
    
    for (i = 0; i < industryNumber; i++) {
        k = i;
        for (j = 0; j < i; j++) {
            if (GetIndustry(i,CATEGORY_ID_ASHARE)->tradeMoney > GetSortedIndustry(j,CATEGORY_ID_ASHARE)->tradeMoney) {
                k = j;
                break;
            }
        }
        if (k != i) {
            for (j = i; j > k; j--) {
                _sortedIndustryListA[j] = _sortedIndustryListA[j - 1];
            }
        }
        _sortedIndustryListA[k] = i;
    }
}

/*****************************************************************
** ������:      InitSortedIndustryListB
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ��������B��Industry����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.11
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
void
InitSortedIndustryListB() {
    int     i               = 0;
    int     j               = 0;
    int     k               = 0;
    int     industryNumber  = GetIndustryNumber(CATEGORY_ID_BSHARE);
    
    for (i = 0; i < industryNumber; i++) {
        k = i;
        for (j = 0; j < i; j++) {
            if (GetIndustry(i,CATEGORY_ID_BSHARE)->tradeMoney > GetSortedIndustry(j,CATEGORY_ID_BSHARE)->tradeMoney) {
                k = j;
                break;
            }
        }
        if (k != i) {
            for (j = i; j > k; j--) {
                _sortedIndustryListB[j] = _sortedIndustryListB[j - 1];
            }
        }
        _sortedIndustryListB[k] = i;
    }
}

/*****************************************************************
** ������:      GetSortedIndustry
** �� ��:       ��
** �� ��:       INDUSTRY_CS*
** ��������:    ����������ָ��λ�õ�INDUSTRY_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.11
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
INDUSTRY_CS*
GetSortedIndustry(int index,int shareType) {
	if(shareType==CATEGORY_ID_ASHARE)
    	return GetIndustry(_sortedIndustryListA[index],shareType);
    else
    	return GetIndustry(_sortedIndustryListB[index],shareType);
}


/*****************************************************************
** ������:      AddIndustryStock
** �� ��:       ��
** �� ��:       Industry��λ��
** ��������:    ��ָ����ҵ���������֤ȯ��Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.13
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
int
AddIndustryStock(char *industryId, STOCK_CS *stock,int shareType) {
    int         i           = 0;
    INDUSTRY_CS *industry   = NULL;

    if (! industryId ) {
        return -1;
    }

    for (i = 0; i < GetIndustryNumber(shareType); i++) {
        industry = GetIndustry(i,shareType);
        if (strncmp(industry->industryId, industryId, INDUSTRY_ID_LENGTH) == 0) {
            stock->industryNo = industry->industryNo;
            return i;
        }
    }
    return -1;
}


/*****************************************************************
** ������:      InitIndustry
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ����ҵ��Ϣ�б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
InitIndustry(INDUSTRY_CS *industry) {
    int i = 0;

    /*
     * ��ʼ�������б�
     */
    for (i = 0; i < INDUSTRY_STOCK_TRADE_SORT_LENGTH; i++) {
        industry->tradeSortList.stockNo[i] = -1;
    }
}


/*****************************************************************
** ������:      PrintIndustryInfoToFile
** �� ��:       fp �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��ҵ��Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       weihy
** �� ��:       2005.9.11
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
** 2007.3.16    Li Bo		�ɽ��������A��B��
****************************************************************/
void
PrintIndustryInfoToFile(int fp,int shareType) {
    INDUSTRY_CS    *industry    = NULL;
    char            buf[1024];
    int             number      = 0;
    int             i           = 0;

    number = GetIndustryNumber(shareType);

	if(shareType == CATEGORY_ID_ASHARE){
		sprintf(buf, "#****A����ҵ��Ϣ****\n#��ҵ����: %d\nA���г��ܳɽ���˫�ߣ�: %lld\n\n",
            number, GetMemTradeStats()->tradeMoney);
    	write(fp, buf, strlen(buf));
	}
	else if(shareType == CATEGORY_ID_BSHARE){
		sprintf(buf, "#****B����ҵ��Ϣ****\n#��ҵ����: %d\nB���г��ܳɽ���˫�ߣ�: %lld\n\n",
            number, GetMemTradeStats()->bTradeMoney);
    	write(fp, buf, strlen(buf));
	}
    

    for (i = 0; i < number; i++) {
        industry = GetIndustry(i,shareType);

        sprintf(buf, "index: %-4d\t" \
                "industryNo: %-4d\t" \
                "industryId: %-4s\t" \
                "industryName: %-16s\t" \
                "tradeNumber: %-8d\t" \
                "tradeAmount: %-8lld\t" \
                "tradeMoney: %-8lld\n"
                , i
                , industry->industryNo
                , industry->industryId
                , industry->industryName
                , industry->tradeNumber
                , industry->tradeAmount
                , industry->tradeMoney);

        write(fp, buf, strlen(buf));
    }
}


/*****************************************************************
** ������:      PrintSortedIndustryInfoToFile
** �� ��:       fp �ļ�������
** �� ��:       ��
** ��������:    ��ӡ��������ҵ��Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.4
** �� ��:
** �� ��:
** �汾:        0.1
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
** 2006.9.4     wugb        ������ҵ����A��B��
****************************************************************/
void
PrintSortedIndustryInfoToFile(int fp,int shareType) {
    INDUSTRY_CS     *industry   = NULL;
    STOCK_CS        *stock      = NULL;
    char            buf[1024];
    char            buf2[256];
    int             number      = 0;
    int             i           = 0;
    int             j           = 0;

    /*
     * ��ʼ��������Industry����
     */

	if(shareType==CATEGORY_ID_ASHARE)
    	InitSortedIndustryListA();
    else
    	InitSortedIndustryListB();
    
    number = GetIndustryNumber(shareType);

    sprintf(buf, "#****��ҵ������Ϣ****\n#��ҵ����: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        industry = GetSortedIndustry(i,shareType);

        sprintf(buf, "index: %-4d\t" \
                "industryNo: %-4d\t" \
                "industryId: %-4s\t" \
                "industryName: %-16s\t" \
                "tradeNumber: %-8d\t" \
                "tradeAmount: %-8lld\t" \
                "tradeMoney: %-8lld\n"
                , i
                , industry->industryNo
                , industry->industryId
                , industry->industryName
                , industry->tradeNumber
                , industry->tradeAmount
                , industry->tradeMoney);

        for (j = 0; j < INDUSTRY_STOCK_TRADE_SORT_LENGTH; j++) {
            if ((stock = GetStock(industry->tradeSortList.stockNo[j]))) {
                sprintf(buf2, "\t----[%d]  stockNo: %-4d,\tstockId: %-8s,\ttradeMoney: %-8lld\n",
                        j, stock->stockNo, stock->stockId, stock->tradeMoney);
                strcat(buf, buf2);
            } else {
                sprintf(buf2, "\t----[%d]  stockNo: %-4d\n",
                        j, industry->tradeSortList.stockNo[j]);
                strcat(buf, buf2);
            }
        }

        write(fp, buf, strlen(buf));
    }
}
