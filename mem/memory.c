/******************************************************************
** 文件名:      memory.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.4
** 描 述:       共享内存分配与管理函数
** 修订记录：
** 修订日期     修订人      修复的bug或者修订原因
**-----------------------------------------------------------------
   2006.8.31    Li Bo       在共享内存中增加stockIndex部分
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
 * 常量定义
 */


/*
 * 模块名称
 */
static const char   _moduleName[] = "memory";


/*
 * 属性
 */
static void     *_shareMemoryAddress    = NULL;                             /* 记录共享内存的开始地址 */


/*
 * 内部函数声明
 */
static int      BuildMemKey();
static void*    BuildShareMemory(const MEM_CONFIG_ALL*);
static void     InitMemoryData(void*, MEM_STATS*);


/*****************************************************************
** 函数名:      InitShareMemory
** 输 入:
** 输 出:       TRUE:   成功;
**              FALSE:  失败
** 功能描述:    初始化系统共享内存
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
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
          ERROR(_moduleName, "获得共享内存Id失败! error: %s", strerror(errno));
            return 0;
        }

        if ((_shareMemoryAddress = shmat(memId, NULL, 0)) == SHM_FAILED) {
          ERROR(_moduleName, "连接共享内存失败! error: %s", strerror(errno));
            return 0;
        }
    }
 //   printf("ordermax=%lld\n", GetShareMemConfigStats()->orderMax);
    if (memConfig){
    	//初始化SET_ID_MAP和SET_STATS    
        //InitSetMap();
	    //初始化开市集合竞价标记
          GetMemTradeStats()->ocallFlag = -1;
	    //初始化pbu内存区
	    #ifdef USE_PBU
	    	InitPBU();
	    #endif

    }
    PrintEvaluateMemAllocationStats(memConfig) ;
    system("ipcs -m");
    return 1;
}


/*****************************************************************
** 函数名:      ReleaseShareMemory
** 输 入:
** 输 出:       无
** 功能描述:    释放系统共享内存
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      EvaluateMemAllocationStats
** 输 入:       MEM_CONFIG_SHARE
** 输 出:       MEM_ALLOCATION_SHARE
** 功能描述:    评估共享内存的使用情况
** 全局变量:    无
** 调用模块:    略
** 作 者:       Li Bo
** 日 期:       2006.9.1
** 修 改:		在共享内存中增加stockIndex部分
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_ALLOCATION_ALL*
EvaluateMemAllocationStats(const MEM_CONFIG_ALL *memConfig) {
    static MEM_ALLOCATION_ALL   allocation;
    long                        size = 0;

   const MEM_CONFIG_SHARE      *shareConfig = &memConfig->shareConfig;

    MEM_ALLOCATION_SHARE        *shareAllocation = &allocation.shareAllocation;

    bzero(&allocation, sizeof(MEM_ALLOCATION_ALL));

    /*
     * 计算各Model的申请空间大小
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
     * 计算各Model的偏移位置与总申请空间大小
     */

    /*
     * 共用单元
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
     * 设置总使用空间
     */
  //  allocation.totalSize = size;这里申请的共享内存是13216642158B
  allocation.totalSize = size ;

    return &allocation;
}


/*****************************************************************
** 函数名:      IsShareMemoryReady
** 输 入:
** 输 出:       TRUE:   成功;
**              FALSE:  失败
** 功能描述:    返回系统共享内存是否已准备就绪
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.14
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
IsShareMemoryReady() {
    return _shareMemoryAddress == SHM_FAILED ? 0 : 1;
}


/*****************************************************************
** 函数名:      GetShareMemory
** 输 入:
** 输 出:
** 功能描述:    返回共享内存的数据段开始地址
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void*
GetShareMemory() {
    return _shareMemoryAddress;
}

/*****************************************************************
** 函数名:      GetMemStats
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_STATS 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_STATS*
GetMemStats() {
    return (MEM_STATS*) GetShareMemory();
}


/*****************************************************************
** 函数名:      GetShareMemConfigStats
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_CONFIG_SHARE 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_CONFIG_SHARE*
GetShareMemConfigStats() {
        return &GetMemStats()->shareConfig;
}


/*****************************************************************
** 函数名:      GetShareMemAllocationStats
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_ALLOCATION_SHARE 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_ALLOCATION_SHARE*
GetShareMemAllocationStats() {
    return &GetMemStats()->shareAllocation;
}


/*****************************************************************
** 函数名:      GetShareMemCurrentStats
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_CURRENT_SHARE 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_CURRENT_SHARE*
GetShareMemCurrentStats() {
    return &GetMemStats()->shareCurrent;
}



/*****************************************************************
** 函数名:      GetMemTime
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_TIME 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_TIME*
GetMemTime() {
   
    return &GetMemStats()->shareTime;
}


/*****************************************************************
** 函数名:      GetMemTradeStats
** 输 入:
** 输 出:
** 功能描述:    返回 MEM_TRADE_STATS 结构
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.9
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
MEM_TRADE_STATS*
GetMemTradeStats() {
    return &GetMemStats()->shareTradeStats;
}


/*****************************************************************
** 函数名:      GetCsSequence
** 输 入:       无
** 输 出:       CS_DATA_SEQUENCE
** 功能描述:    返回CS端已接收的数据数量
** 全局变量:    无
** 调用模块:
** 作 者:       Li Bo
** 日 期:       2006.9.16
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
/*CS_DATA_SEQUENCE*
GetCsSequence() {
    return &GetMemTradeStats()->csDataSequence;
}*/

/*****************************************************************
** 函数名:      UpdateMemTime
** 输 入:       TE主机时间
** 输 出:       无
** 功能描述:    更新 MEM_TIME
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.17
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
UpdateMemTime(int teTime) {
  //  MEM_TIME    *memTime = GetMemTime();

   // memTime->teTime = teTime;
   // memTime->csTime = GetIntTime();
   // memTime->windage = DiffSecondInt(memTime->teTime, memTime->csTime);
}


/*****************************************************************
** 函数名:      GetTeTime
** 输 入:       无
** 输 出:       TE主机时间
** 功能描述:    返回TE主机时间
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.25
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetTeTime() {
    ///return GetIntTimeFromSeconds(
         //   GetSecondsFromInt(GetIntTime()) - GetMemTime()->windage);
         return 0;
}


/*****************************************************************
** 函数名:      BuildMemKey
** 输 入:
** 输 出:
** 功能描述:    返回共享内存的MemKey
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static int
BuildMemKey() {
    return ftok(".", CS_SHARE_MEMORY_ID);
 return 0;
}


/*****************************************************************
** 函数名:      BuildShareMemory
** 输 入:       MEM_ALLOCATION_ALL
** 输 出:       n           : 成功
**              SHM_FAILED  : 失败
** 功能描述:    构造系统共享内存
** 全局变量:    无
** 调用模块:    略
** 作 者:       Song Fang
** 日 期:       2005.9.7
** 修 改:
** 日 期:
** 版本:        0.1
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

   TRACE(_moduleName, "开始构造共享内存...");
   TRACE(_moduleName, "申请总内存大小: %ld", stats.totalSize);

   TRACE(_moduleName, "共用单元内存申请信息 -- statsSize: %ld",
           shareAllocation->statsSize);
   TRACE(_moduleName, "内存申请信息 -- stockSize: %ld; orderSize: %ld; tradeSize: %ld; withdrawSize: %ld; bidSize: %ld; offerSize: %ld; aIndustrySize: %ld; bIndustrySize: %ld",
            shareAllocation->stockSize,
            shareAllocation->orderSize,
            shareAllocation->tradeSize,
            shareAllocation->withdrawSize,
            shareAllocation->bidSize,
            shareAllocation->offerSize,
            shareAllocation->aIndustrySize,
            shareAllocation->aIndustrySize);
    #ifdef USE_PBU
    	TRACE(_moduleName, "PBU内存申请信息 -- pbuFirmSize: %ld; pbuTableSize: %ld; pbuBuySortSize: %ld; pbuSellSortSize: %ld; pbuDataSize: %ld;",
    		shareAllocation->pbuTableSize,
    		shareAllocation->pbuTableSize,
    		shareAllocation->pbuBuySortSize,
    		shareAllocation->pbuSellSortSize,
    		shareAllocation->pbuDataSize);
    #endif
    TRACE(_moduleName, "共用单元内存偏移量信息 -- statsOffset: %ld",
          shareAllocation->statsOffset);
    TRACE(_moduleName, "内存偏移量信息 -- stockOffset: %ld; orderOffset: %ld; tradeOffset: %ld; withdrawOffset: %ld; bidOffset: %ld; offerOffset: %ld; aIndustryOffset: %ld; aIndustryOffset: %ld",
            shareAllocation->stockOffset,
            shareAllocation->orderOffset,
            shareAllocation->tradeOffset,
            shareAllocation->withdrawOffset,
            shareAllocation->bidOffset,
            shareAllocation->offerOffset,
            shareAllocation->aIndustryOffset,
            shareAllocation->bIndustryOffset);
	#ifdef USE_PBU
		TRACE(_moduleName, "PBU内存偏移信息 -- pbuFirmOffset: %ld; pbuTableOffset: %ld; pbuBuySortOffset: %ld; pbuSellSortOffset: %ld; pbuDataOffset: %ld;",
			shareAllocation->pbuFirmOffset,
			shareAllocation->pbuTableOffset,
			shareAllocation->pbuBuySortOffset,
			shareAllocation->pbuSellSortOffset,
			shareAllocation->pbuDataOffset);
	#endif
    /*
     * 申请共享内存
     */

    if ((memId = shmget(BuildMemKey(), stats.totalSize, 0777 | IPC_CREAT)) < 0) {
      WARN(_moduleName, "申请共享内存失败! -- size: %ld; error: %s",
             stats.totalSize, strerror(errno));
        return SHM_FAILED;
    }

    if ((memAddr = shmat(memId, NULL, 0)) == SHM_FAILED) {
      ERROR(_moduleName, "连接共享内存失败! error: %s", strerror(errno));
        return SHM_FAILED;
    }

printf("ket_t %d size %d flag %d memid %d  total size [%ld]\n",BuildMemKey() ,stats.totalSize , SHM_MODE_RW | IPC_CREAT , memId ,  stats.totalSize );
 /*
     * 初始化内存数据
     */
    InitMemoryData(memAddr, &stats);
    INFO(_moduleName, "构造共享内存成功! 共享内存总大小为[%ld]字节", stats.totalSize);
    return memAddr;
}


/*****************************************************************
** 函数名:      InitMemoryData
** 输 入:       无
** 输 出:       无
** 功能描述:    初始化排名列表
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.19
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
static void
InitMemoryData(void *memAddr, MEM_STATS *stats) {
  int             i                   = 0;
   MEM_TRADE_STATS *shareTradeStats   = NULL;

   shareTradeStats = &stats->shareTradeStats;

    /*
     * 初始化排名列表
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
** 函数名:      PrintEvaluateMemAllocationStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存使用情况评估
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.11.14
** 修 改:
** 日 期:
** 版本:        0.1
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

    
    printf("INFO: 共享内存使用情况评估 ...\n");
    printf("INFO: 申请总内存大小 total memory size : %ld\n", stats.totalSize);

    printf("INFO: 共用单元内存申请信息 \n");
    printf("      --  statsSize: %ld \n", shareAllocation->statsSize);
    
    printf("INFO: 内存申请信息 \n");
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
** 函数名:      PrintMemoryStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintMemoryConfigStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存参数配置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintMemoryAllocationStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存分配信息
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintMemoryCurrentStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存当前状态
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintMemoryTimeStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存中的CS时间与TE时间记录
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintMemoryTradeStats
** 输 入:       无
** 输 出:       无
** 功能描述:    打印共享内存当前交易情况
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.13
** 修 改:
** 日 期:
** 版本:        0.1
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
