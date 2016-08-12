/******************************************************************
** �ļ���:      memory.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.4
** �� ��:       �����ڴ�����������
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
   2006.8.31    Li Bo       �ڹ����ڴ�������stockIndex����
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/memory/memory.c,v 1.1 2012/03/29 01:19:50 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:50 $
 */


#include    <stdio.h>
#include    <stdlib.h>
#include    <memory.h>
#include    <errno.h>
#include    <sys/types.h>
#include    <sys/ipc.h>
#include    <sys/sem.h>
#include    <sys/shm.h>
#include    <string.h>
#include    "memory.h"
#include    "./model/model.h"
//#include    "../model/model_te.h"
//#include    "../model/stock_index.h"
#include    "./env/env.h"
//#include    "../config/mem_config.h"
//#include    "./util/symbol.h"
//#include    "./util/time/common_time.h"
//#include    "./util/lib/common_lib.h"
#include    "./util/log/log.h"

/*
 * ��������
 */


/*
 * ģ������
 */
static const char   _moduleName[] = "memory";


/*
 * ����
 */
static void     *_shareMemoryAddress    = NULL;                             /* ��¼�����ڴ�Ŀ�ʼ��ַ */


/*
 * �ڲ���������
 */
static int      BuildMemKey();
static void*    BuildShareMemory(const MEM_CONFIG_ALL*);
static void     InitMemoryData(void*, MEM_STATS*);


/*****************************************************************
** ������:      InitShareMemory
** �� ��:
** �� ��:       TRUE:   �ɹ�;
**              FALSE:  ʧ��
** ��������:    ��ʼ��ϵͳ�����ڴ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
InitShareMemory(const MEM_CONFIG_ALL *memConfig) {
    int     memId = 0;

    _shareMemoryAddress = SHM_FAILED;

    if (memConfig) {
        _shareMemoryAddress = BuildShareMemory(memConfig);
    }
printf("I'm your God   hahahhahahahhahahahh  _shareMemoryAddress %0x \n", _shareMemoryAddress);
    if (!memConfig  || _shareMemoryAddress == SHM_FAILED) {
        if ((memId = shmget(BuildMemKey(), 0, SHM_MODE_RW)) < 0) {
          ERROR(_moduleName, "��ù����ڴ�Idʧ��! error: %s", strerror(errno));
            return 0;
        }

        if ((_shareMemoryAddress = shmat(memId, NULL, 0)) == SHM_FAILED) {
          ERROR(_moduleName, "���ӹ����ڴ�ʧ��! error: %s", strerror(errno));
            return 0;
        }
    }
 //   printf("ordermax=%lld\n", GetShareMemConfigStats()->orderMax);
    if (memConfig){
    	//��ʼ��SET_ID_MAP��SET_STATS    
        //InitSetMap();
	    //��ʼ�����м��Ͼ��۱��
          GetMemTradeStats()->ocallFlag = -1;
	    //��ʼ��pbu�ڴ���
	    #ifdef USE_PBU
	    	InitPBU();
	    #endif

    }
    PrintEvaluateMemAllocationStats(memConfig) ;
    system("ipcs -m");
    return 1;
}


/*****************************************************************
** ������:      ReleaseShareMemory
** �� ��:
** �� ��:       ��
** ��������:    �ͷ�ϵͳ�����ڴ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
ReleaseShareMemory() {
    int     i       = 0;
    int     memId   = 0;

    for (i = 0; i < 9 && (memId = shmget(BuildMemKey(), 0, 0)) > 0; i++) {
        shmctl(memId, IPC_RMID, NULL);
    }
}


/*****************************************************************
** ������:      EvaluateMemAllocationStats
** �� ��:       MEM_CONFIG_SHARE
** �� ��:       MEM_ALLOCATION_SHARE
** ��������:    ���������ڴ��ʹ�����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.1
** �� ��:		�ڹ����ڴ�������stockIndex����
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_ALLOCATION_ALL*
EvaluateMemAllocationStats(const MEM_CONFIG_ALL *memConfig) {
    static MEM_ALLOCATION_ALL   allocation;
    long                        size = 0;

   const MEM_CONFIG_SHARE      *shareConfig = &memConfig->shareConfig;

    MEM_ALLOCATION_SHARE        *shareAllocation = &allocation.shareAllocation;

    bzero(&allocation, sizeof(MEM_ALLOCATION_ALL));

    /*
     * �����Model������ռ��С
     */
    shareAllocation->statsSize    = sizeof(MEM_STATS);

	shareAllocation->setIdMapSize = (long) sizeof(SET_ID_MAP)	* (shareConfig->setIdMapMax + 1);
	shareAllocation->setStatsSize = (long) sizeof(SET_STATS)	* (shareConfig->setMax + 1);
	shareAllocation->setMapSize   = (long) sizeof(SET_MAP)		* (shareConfig->setMax * shareConfig->setSizeMax + 1);
    shareAllocation->categorySize = (long) sizeof(CATEGORY_CS)	* (shareConfig->categoryMax + 1);
    shareAllocation->stockSize    = (long) sizeof(STOCK_CS)		* (shareConfig->stockMax + 1);
    #ifdef USE_PBU
	    shareAllocation->pbuFirmSize  = (long) sizeof(PBU_FIRM) 	* (shareConfig->pbuNumberMax + 1);
	    shareAllocation->pbuTableSize = (long) sizeof(PBU_HEAD)		* (shareConfig->pbuNumberMax * shareConfig->stockMax + 1);
	    shareAllocation->pbuBuySortSize =(long) sizeof(int)			* (shareConfig->pbuNumberMax * shareConfig->stockMax + 1);
	    shareAllocation->pbuSellSortSize=(long) sizeof(int)			* (shareConfig->pbuNumberMax * shareConfig->stockMax + 1);
	    shareAllocation->pbuDataSize  = (long) sizeof(PBU_DATA)		* (shareConfig->pbuDataNumberMax + 1);
    #endif
  shareAllocation->orderSize    = (long) sizeof(ORDER_CS)		* (shareConfig->orderMax + 1);
    shareAllocation->tradeSize    = (long) sizeof(TRADE_CS)		* (shareConfig->tradeMax + 1);
  shareAllocation->withdrawSize = (long) sizeof(WITHDRAW_CS)	* (shareConfig->withdrawMax + 1);
   shareAllocation->bidSize      = (long) sizeof(BID_CS)		* (shareConfig->bidMax + 1);
   shareAllocation->offerSize    = (long) sizeof(OFFER_CS)		* (shareConfig->offerMax + 1);
   shareAllocation->aIndustrySize  = (long) sizeof(INDUSTRY_CS) * (shareConfig->industryMax + 1);
  shareAllocation->bIndustrySize  = (long) sizeof(INDUSTRY_CS) * (shareConfig->industryMax + 1);
  shareAllocation->stockIndexSize = (long) sizeof(STOCK_INDEX_CS) * (shareConfig->stockIndexMax + 1);

    
    /*
     * �����Model��ƫ��λ����������ռ��С
     */

    /*
     * ���õ�Ԫ
     */
    shareAllocation->statsOffset = 0;
    size = shareAllocation->statsSize;

    shareAllocation->setIdMapOffset = size / sizeof(SET_ID_MAP) + 1;
    size += shareAllocation->setIdMapSize;
    
    shareAllocation->setStatsOffset = size / sizeof(SET_STATS) + 1;
    size += shareAllocation->setStatsSize;
    
  shareAllocation->setMapOffset = size / sizeof(SET_MAP) + 1;
    size += shareAllocation->setMapSize;
    
	shareAllocation->categoryOffset = size / sizeof(CATEGORY_CS) + 1;
    size += shareAllocation->categorySize;
    
   shareAllocation->stockOffset = size / sizeof(STOCK_CS) + 1;
    size += shareAllocation->stockSize;
    
    #ifdef USE_PBU
	    shareAllocation->pbuFirmOffset = size / sizeof(PBU_FIRM) + 1;
	    size += shareAllocation->pbuFirmSize;
	
		shareAllocation->pbuTableOffset = size / sizeof(PBU_HEAD) + 1;
	    size += shareAllocation->pbuTableSize;
	    
	   shareAllocation->pbuBuySortOffset = size / sizeof(int) + 1;
	    size += shareAllocation->pbuBuySortSize;
	    
	    shareAllocation->pbuSellSortOffset = size / sizeof(int) + 1;
	    size += shareAllocation->pbuSellSortSize;
	    
	    shareAllocation->pbuDataOffset = size / sizeof(PBU_DATA) + 1;
	    size += shareAllocation->pbuDataSize;
	#endif

   shareAllocation->orderOffset = size / sizeof(ORDER_CS) + 1;
    size += shareAllocation->orderSize;

   shareAllocation->tradeOffset = size / sizeof(TRADE_CS) + 1;
    size += shareAllocation->tradeSize;

 shareAllocation->withdrawOffset = size / sizeof(WITHDRAW_CS) + 1;
    size += shareAllocation->withdrawSize;

   shareAllocation->bidOffset = size / sizeof(BID_CS) + 1;
    size += shareAllocation->bidSize;

  shareAllocation->offerOffset = size / sizeof(OFFER_CS) + 1;
    size += shareAllocation->offerSize;

   shareAllocation->aIndustryOffset = size / sizeof(INDUSTRY_CS) + 1;
    size += shareAllocation->aIndustrySize;
    
    
   shareAllocation->bIndustryOffset = size / sizeof(INDUSTRY_CS) + 1;
    size += shareAllocation->bIndustrySize;
	
	shareAllocation->stockIndexOffset = size / sizeof(STOCK_INDEX_CS) + 1;
    size += shareAllocation->stockIndexSize;

    /*
     * ������ʹ�ÿռ�
     */
  //  allocation.totalSize = size;��������Ĺ����ڴ���13216642158B
  allocation.totalSize = size ;

    return &allocation;
}


/*****************************************************************
** ������:      IsShareMemoryReady
** �� ��:
** �� ��:       TRUE:   �ɹ�;
**              FALSE:  ʧ��
** ��������:    ����ϵͳ�����ڴ��Ƿ���׼������
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.14
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
IsShareMemoryReady() {
    return _shareMemoryAddress == SHM_FAILED ? 0 : 1;
}


/*****************************************************************
** ������:      GetShareMemory
** �� ��:
** �� ��:
** ��������:    ���ع����ڴ�����ݶο�ʼ��ַ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void*
GetShareMemory() {
    return _shareMemoryAddress;
}

/*****************************************************************
** ������:      GetMemStats
** �� ��:
** �� ��:
** ��������:    ���� MEM_STATS �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_STATS*
GetMemStats() {
    return (MEM_STATS*) GetShareMemory();
}


/*****************************************************************
** ������:      GetShareMemConfigStats
** �� ��:
** �� ��:
** ��������:    ���� MEM_CONFIG_SHARE �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_CONFIG_SHARE*
GetShareMemConfigStats() {
        return &GetMemStats()->shareConfig;
}


/*****************************************************************
** ������:      GetShareMemAllocationStats
** �� ��:
** �� ��:
** ��������:    ���� MEM_ALLOCATION_SHARE �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_ALLOCATION_SHARE*
GetShareMemAllocationStats() {
    return &GetMemStats()->shareAllocation;
}


/*****************************************************************
** ������:      GetShareMemCurrentStats
** �� ��:
** �� ��:
** ��������:    ���� MEM_CURRENT_SHARE �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_CURRENT_SHARE*
GetShareMemCurrentStats() {
    return &GetMemStats()->shareCurrent;
}



/*****************************************************************
** ������:      GetMemTime
** �� ��:
** �� ��:
** ��������:    ���� MEM_TIME �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_TIME*
GetMemTime() {
   
    return &GetMemStats()->shareTime;
}


/*****************************************************************
** ������:      GetMemTradeStats
** �� ��:
** �� ��:
** ��������:    ���� MEM_TRADE_STATS �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.9
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
MEM_TRADE_STATS*
GetMemTradeStats() {
    return &GetMemStats()->shareTradeStats;
}


/*****************************************************************
** ������:      GetCsSequence
** �� ��:       ��
** �� ��:       CS_DATA_SEQUENCE
** ��������:    ����CS���ѽ��յ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Li Bo
** �� ��:       2006.9.16
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
/*CS_DATA_SEQUENCE*
GetCsSequence() {
    return &GetMemTradeStats()->csDataSequence;
}*/

/*****************************************************************
** ������:      UpdateMemTime
** �� ��:       TE����ʱ��
** �� ��:       ��
** ��������:    ���� MEM_TIME
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.17
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
UpdateMemTime(int teTime) {
  //  MEM_TIME    *memTime = GetMemTime();

   // memTime->teTime = teTime;
   // memTime->csTime = GetIntTime();
   // memTime->windage = DiffSecondInt(memTime->teTime, memTime->csTime);
}


/*****************************************************************
** ������:      GetTeTime
** �� ��:       ��
** �� ��:       TE����ʱ��
** ��������:    ����TE����ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.25
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetTeTime() {
    ///return GetIntTimeFromSeconds(
         //   GetSecondsFromInt(GetIntTime()) - GetMemTime()->windage);
         return 0;
}


/*****************************************************************
** ������:      BuildMemKey
** �� ��:
** �� ��:
** ��������:    ���ع����ڴ��MemKey
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static int
BuildMemKey() {
    return ftok(".", CS_SHARE_MEMORY_ID);
 return 0;
}


/*****************************************************************
** ������:      BuildShareMemory
** �� ��:       MEM_ALLOCATION_ALL
** �� ��:       n           : �ɹ�
**              SHM_FAILED  : ʧ��
** ��������:    ����ϵͳ�����ڴ�
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.9.7
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void*
BuildShareMemory(const MEM_CONFIG_ALL *memConfig) {
    int                     memId           = 0;
    void                    *memAddr        = NULL;
    MEM_ALLOCATION_ALL      *allocationAll  = NULL;

    MEM_STATS               stats;
    MEM_CONFIG_SHARE        *shareConfig           = &stats.shareConfig;
    MEM_ALLOCATION_SHARE    *shareAllocation       = &stats.shareAllocation;
    
    allocationAll = EvaluateMemAllocationStats(memConfig);

    bzero(&stats, sizeof(MEM_STATS));
    stats.totalSize = allocationAll->totalSize;
    memcpy(shareConfig, &memConfig->shareConfig, sizeof(MEM_CONFIG_SHARE));
    memcpy(shareAllocation, &allocationAll->shareAllocation, sizeof(MEM_ALLOCATION_SHARE));

   TRACE(_moduleName, "��ʼ���칲���ڴ�...");
   TRACE(_moduleName, "�������ڴ��С: %ld", stats.totalSize);

   TRACE(_moduleName, "���õ�Ԫ�ڴ�������Ϣ -- statsSize: %ld",
           shareAllocation->statsSize);
   TRACE(_moduleName, "�ڴ�������Ϣ -- stockSize: %ld; orderSize: %ld; tradeSize: %ld; withdrawSize: %ld; bidSize: %ld; offerSize: %ld; aIndustrySize: %ld; bIndustrySize: %ld",
            shareAllocation->stockSize,
            shareAllocation->orderSize,
            shareAllocation->tradeSize,
            shareAllocation->withdrawSize,
            shareAllocation->bidSize,
            shareAllocation->offerSize,
            shareAllocation->aIndustrySize,
            shareAllocation->aIndustrySize);
    #ifdef USE_PBU
    	TRACE(_moduleName, "PBU�ڴ�������Ϣ -- pbuFirmSize: %ld; pbuTableSize: %ld; pbuBuySortSize: %ld; pbuSellSortSize: %ld; pbuDataSize: %ld;",
    		shareAllocation->pbuTableSize,
    		shareAllocation->pbuTableSize,
    		shareAllocation->pbuBuySortSize,
    		shareAllocation->pbuSellSortSize,
    		shareAllocation->pbuDataSize);
    #endif
    TRACE(_moduleName, "���õ�Ԫ�ڴ�ƫ������Ϣ -- statsOffset: %ld",
          shareAllocation->statsOffset);
    TRACE(_moduleName, "�ڴ�ƫ������Ϣ -- stockOffset: %ld; orderOffset: %ld; tradeOffset: %ld; withdrawOffset: %ld; bidOffset: %ld; offerOffset: %ld; aIndustryOffset: %ld; aIndustryOffset: %ld",
            shareAllocation->stockOffset,
            shareAllocation->orderOffset,
            shareAllocation->tradeOffset,
            shareAllocation->withdrawOffset,
            shareAllocation->bidOffset,
            shareAllocation->offerOffset,
            shareAllocation->aIndustryOffset,
            shareAllocation->bIndustryOffset);
	#ifdef USE_PBU
		TRACE(_moduleName, "PBU�ڴ�ƫ����Ϣ -- pbuFirmOffset: %ld; pbuTableOffset: %ld; pbuBuySortOffset: %ld; pbuSellSortOffset: %ld; pbuDataOffset: %ld;",
			shareAllocation->pbuFirmOffset,
			shareAllocation->pbuTableOffset,
			shareAllocation->pbuBuySortOffset,
			shareAllocation->pbuSellSortOffset,
			shareAllocation->pbuDataOffset);
	#endif
    /*
     * ���빲���ڴ�
     */

    if ((memId = shmget(BuildMemKey(), stats.totalSize, 0777 | IPC_CREAT)) < 0) {
      WARN(_moduleName, "���빲���ڴ�ʧ��! -- size: %ld; error: %s",
             stats.totalSize, strerror(errno));
        return SHM_FAILED;
    }

    if ((memAddr = shmat(memId, NULL, 0)) == SHM_FAILED) {
      ERROR(_moduleName, "���ӹ����ڴ�ʧ��! error: %s", strerror(errno));
        return SHM_FAILED;
    }

printf("ket_t %d size %d flag %d memid %d  total size [%ld]\n",BuildMemKey() ,stats.totalSize , SHM_MODE_RW | IPC_CREAT , memId ,  stats.totalSize );
 /*
     * ��ʼ���ڴ�����
     */
    InitMemoryData(memAddr, &stats);
    INFO(_moduleName, "���칲���ڴ�ɹ�! �����ڴ��ܴ�СΪ[%ld]�ֽ�", stats.totalSize);
    return memAddr;
}


/*****************************************************************
** ������:      InitMemoryData
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ʼ�������б�
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.19
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
static void
InitMemoryData(void *memAddr, MEM_STATS *stats) {
  int             i                   = 0;
   MEM_TRADE_STATS *shareTradeStats   = NULL;

   shareTradeStats = &stats->shareTradeStats;

    /*
     * ��ʼ�������б�
     */
   /* for (i = 0; i < WITHDRAW_SORT_LENGTH; i++) {
        shareTradeStats->aWithdrawBuySortList.withdrawNo[i] = -1;
        shareTradeStats->aWithdrawSellSortList.withdrawNo[i] = -1;
        shareTradeStats->bWithdrawBuySortList.withdrawNo[i] = -1;
        shareTradeStats->bWithdrawSellSortList.withdrawNo[i] = -1;
   }*/
    stats->gwHeartbeat  = -1;
   stats->venderStatus = VENDER_SEND_NORMAL;
   bzero(memAddr, stats->totalSize);
    memcpy(memAddr, (char*) stats, sizeof(MEM_STATS));
}


/*****************************************************************
** ������:      PrintEvaluateMemAllocationStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ�ʹ���������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.11.14
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintEvaluateMemAllocationStats(const MEM_CONFIG_ALL *memConfig) {
    MEM_ALLOCATION_ALL      *allocationAll  = NULL;

    MEM_STATS               stats;
    MEM_CONFIG_SHARE        *shareConfig           = &stats.shareConfig;
    MEM_ALLOCATION_SHARE    *shareAllocation       = &stats.shareAllocation;

    allocationAll = EvaluateMemAllocationStats(memConfig);

    bzero(&stats, sizeof(MEM_STATS));
    stats.totalSize = allocationAll->totalSize;
    memcpy(shareConfig, &memConfig->shareConfig, sizeof(MEM_CONFIG_SHARE));
    memcpy(shareAllocation, &allocationAll->shareAllocation, sizeof(MEM_ALLOCATION_SHARE));

    
    printf("INFO: �����ڴ�ʹ��������� ...\n");
    printf("INFO: �������ڴ��С total memory size : %ld\n", stats.totalSize);

    printf("INFO: ���õ�Ԫ�ڴ�������Ϣ \n");
    printf("      --  statsSize: %ld \n", shareAllocation->statsSize);
    
    printf("INFO: �ڴ�������Ϣ \n");
    printf("      --  categorySize: %ld \n", shareAllocation->categorySize);
    printf("      --  stockSize: %ld \n", shareAllocation->stockSize);
    printf("      --  orderSize: %ld \n", shareAllocation->orderSize);
    printf("      --  tradeSize: %ld \n", shareAllocation->tradeSize);
    printf("      --  withdrawSize: %ld \n", shareAllocation->withdrawSize);
    printf("      --  bidSize: %ld \n", shareAllocation->bidSize);
    printf("      --  offerSize: %ld \n", shareAllocation->offerSize);
    printf("      --  aIndustrySize: %ld \n", shareAllocation->aIndustrySize);
	printf("      --  bIndustrySize: %ld \n", shareAllocation->bIndustrySize);
	printf("      --  stockIndexSize: %ld \n", shareAllocation->stockIndexSize);
    #ifdef USE_PBU
	printf("      --  pbuFirmSize: %ld \n", shareAllocation->pbuFirmSize);
	printf("      --  pbuTableSize: %ld \n", shareAllocation->pbuTableSize);
	printf("      --  pbuBuySortSize: %ld \n", shareAllocation->pbuBuySortSize);
	printf("      --  pbuSellSortSize: %ld \n", shareAllocation->pbuSellSortSize);
	printf("      --  pbuDataSize: %ld \n", shareAllocation->pbuDataSize);
    #endif
    
    printf("\n");
}


/*****************************************************************
** ������:      PrintMemoryStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ�״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryStats() {
    MEM_STATS   *stats  = GetMemStats();

    printf("MEM STATS: totalSize:    %ld\n", stats->totalSize);

    //PrintGlobalData();
   PrintMemoryConfigStats();
   // PrintMemoryAllocationStats();
    //PrintMemoryCurrentStats();
    //PrintMemoryTimeStats();
   // PrintMemoryTradeStats();
}


/*****************************************************************
** ������:      PrintMemoryConfigStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ��������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryConfigStats() {
    MEM_STATS           *stats          = GetMemStats();
    MEM_CONFIG_SHARE    *shareConfig   = &stats->shareConfig;
   
    printf("MEM  CONFIG: categoryMax:    %d\n",shareConfig->categoryMax);
    printf("MEM  CONFIG: stockMax:       %d\n", shareConfig->stockMax);
    printf("MEM  CONFIG: orderMax:       %d\n", shareConfig->orderMax);
    printf("MEM  CONFIG: tradeMax:       %d\n", shareConfig->tradeMax);
    printf("MEM  CONFIG: withdrawMax:    %d\n", shareConfig->withdrawMax);
    printf("MEM  CONFIG: bidMax:         %d\n", shareConfig->bidMax);
    printf("MEM  CONFIG: offerMax:       %d\n", shareConfig->offerMax);
    printf("MEM  CONFIG: industryMax:    %d\n", shareConfig->industryMax);
    printf("MEM  CONFIG: stockIndexMax:  %d\n", shareConfig->stockIndexMax);
    #ifdef USE_PBU
    printf("MEM  CONFIG: pbuNumberMax:   %d\n", shareConfig->pbuNumberMax);
    #endif

    printf("\n");
}


/*****************************************************************
** ������:      PrintMemoryAllocationStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ������Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryAllocationStats() {
    MEM_STATS               *stats              = GetMemStats();
    MEM_ALLOCATION_SHARE    *shareAllocation   = &stats->shareAllocation;

    printf("MEM ALLOC: totalSize:        %ld\n", stats->totalSize);

    printf("MEM  ALLOC: categorySize:    %ld\n", shareAllocation->categorySize);
    printf("MEM  ALLOC: stockSize:       %ld\n", shareAllocation->stockSize);
    printf("MEM  ALLOC: orderSize:       %ld\n", shareAllocation->orderSize);
    printf("MEM  ALLOC: tradeSize:       %ld\n", shareAllocation->tradeSize);
    printf("MEM  ALLOC: withdrawSize:    %ld\n", shareAllocation->withdrawSize);
    printf("MEM  ALLOC: bidSize:         %ld\n", shareAllocation->bidSize);
    printf("MEM  ALLOC: offerSize:       %ld\n", shareAllocation->offerSize);
    printf("MEM  ALLOC: AindustrySize:   %ld\n", shareAllocation->aIndustrySize);
    printf("MEM  ALLOC: BindustrySize:   %ld\n", shareAllocation->bIndustrySize);
    printf("MEM  ALLOC: stockIndexSize:  %ld\n", shareAllocation->stockIndexSize);
    #ifdef USE_PBU                       
	printf("MEM  ALLOC: pbuFirmSize:     %ld \n", shareAllocation->pbuFirmSize);
	printf("MEM  ALLOC: pbuTableSize:    %ld \n", shareAllocation->pbuTableSize);
	printf("MEM  ALLOC: pbuBuySortSize:  %ld \n", shareAllocation->pbuBuySortSize);
	printf("MEM  ALLOC: pbuSellSortSize: %ld \n", shareAllocation->pbuSellSortSize);
	printf("MEM  ALLOC: pbuDataSize:     %ld \n", shareAllocation->pbuDataSize);
    #endif

    printf("\n");
}


/*****************************************************************
** ������:      PrintMemoryCurrentStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ浱ǰ״̬
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryCurrentStats() {
    MEM_STATS           *stats			= GetMemStats();
    MEM_CURRENT_SHARE   *shareCurrent	= &stats->shareCurrent;

	printf("MEM  CURRENT: gwServerIp:     %s\n", shareCurrent->gwServerIp);
    printf("MEM  CURRENT: gwServerPort:   %d\n", shareCurrent->gwServerPort);
	printf("MEM  CURRENT: categoryNum:    %d\n", shareCurrent->nextCategory);
    printf("MEM  CURRENT: stockNum:       %d\n", shareCurrent->nextStock);
    printf("MEM  CURRENT: stockIndexNum:  %d\n", shareCurrent->nextStockIndex);
    printf("MEM  CURRENT: orderNum:       %d\n", shareCurrent->nextOrder);
    printf("MEM  CURRENT: tradeNum:       %d\n", shareCurrent->nextTrade);
    printf("MEM  CURRENT: withdrawNum:    %d\n", shareCurrent->nextWithdraw);
    printf("MEM  CURRENT: bidNum:         %d\n", shareCurrent->nextBid);
    printf("MEM  CURRENT: offerNum:       %d\n", shareCurrent->nextOffer);
    printf("MEM  CURRENT: AindustryNum:   %d\n", shareCurrent->nextAIndustry);
	printf("MEM  CURRENT: BindustryNum:   %d\n", shareCurrent->nextBIndustry);
	#ifdef USE_PBU
	printf("MEM  CURRENT: PBUFirmNum:     %d\n", shareCurrent->nextPBUFirm);
	#endif
    
    printf("\n");
}


/*****************************************************************
** ������:      PrintMemoryTimeStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ��е�CSʱ����TEʱ���¼
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryTimeStats() {
    MEM_STATS       *stats          = GetMemStats();
    MEM_TIME        *shareTime     = &stats->shareTime;

    printf("MEM A TIME: teTime:   %d\n", shareTime->teTime);
    printf("MEM A TIME: csTime:   %d\n", shareTime->csTime);
    printf("MEM A TIME: windage:  %d\n", shareTime->windage);


    printf("\n");
}


/*****************************************************************
** ������:      PrintMemoryTradeStats
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡ�����ڴ浱ǰ�������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.13
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintMemoryTradeStats() {
  //  MEM_STATS       *stats          = GetMemStats();
   // MEM_TRADE_STATS *trade         = &stats->shareTradeStats;
	
	//printf("MEM  TRADE: receivedSetDataNumber:        %d\n", trade->csDataSequence.receivedSetDataNumber);
	//printf("MEM  TRADE: receivedCommonDataNumber:     %d\n", trade->csDataSequence.receivedCommonDataNumber);
    //printf("MEM  TRADE: workedOrderNumber:            %d\n", trade->workedOrderNumber);
    //printf("MEM  TRADE: workedTradeNumber:            %d\n", trade->workedTradeNumber);
   // printf("MEM  TRADE: workedWithdrawNumber:         %d\n", trade->workedWithdrawNumber);
   // printf("MEM  TRADE: sentTradeNumber:              %d\n", trade->sentTradeNumber);

    printf("\n");
}
