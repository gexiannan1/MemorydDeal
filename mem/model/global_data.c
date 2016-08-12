/******************************************************************
** �ļ���:      global_data.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.18
** �� ��:       ϵͳȫ�����ݶ���
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/global_data.c,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#include    <string.h>
#include    <stdio.h>
#include    "global_data.h"
#include    "model.h"
#include    "..//memory.h"
#include    "../util/symbol.h"
#include    "../util/string/common_str.h"


/*
 * ��������
 */
#define	DEFAULT_MULTICAST_DELAY_TIME	10000		/* Ĭ�ϵ��鲥��Ƭ�ӳ�ʱ�䣨΢�룩 */


/*****************************************************************
** ������:      GetGlobalData
** �� ��:
** �� ��:
** ��������:    ���� GLOBAL_DATA �ṹ
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
GLOBAL_DATA*
GetGlobalData() {
    return &GetMemStats()->globalData;
}


/*****************************************************************
** ������:      GetGlobalMulticastDelayTime
** �� ��:
** �� ��:
** ��������:    �����鲥��Ƭ�ӳ�ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetGlobalMulticastDelayTime() {
	int multicastDelayTime = GetGlobalData()->multicastDelayTime;
	
	if (multicastDelayTime <= 0) {
		return DEFAULT_MULTICAST_DELAY_TIME;
	}
    return multicastDelayTime;
}


/*****************************************************************
** ������:      SetGlobalMulticastDelayTime
** �� ��:
** �� ��:
** ��������:    �����鲥��Ƭ�ӳ�ʱ��
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Song Fang
** �� ��:       2005.10.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
SetGlobalMulticastDelayTime(int delayTime) {
    GetGlobalData()->multicastDelayTime = delayTime;
}


/*****************************************************************
** ������:      PrintGlobalData
** �� ��:       ��
** �� ��:       ��
** ��������:    ��ӡϵͳȫ������
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.18
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintGlobalData() {
	GLOBAL_DATA *globalData = GetGlobalData();
	
	printf("GLOBAL DATA: multicastDelayTime:    %d\n", 
			globalData->multicastDelayTime);
	
	printf("\n");
}
