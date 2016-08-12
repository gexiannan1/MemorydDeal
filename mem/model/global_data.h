/******************************************************************
** �ļ���:      global_data.h
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.10.18
** �� ��:       ϵͳȫ�����ݶ����ͷ�ļ�
** �޶���¼��
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/global_data.h,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#ifndef _MODEL_GLOBAL_DATA_H
#define _MODEL_GLOBAL_DATA_H


#include    "../util/symbol.h"


/*
 * ϵͳȫ������
 */
typedef struct {
    int     multicastDelayTime;                         /* �鲥��Ƭ�ӳ�ʱ�� */
} GLOBAL_DATA;
/* -------------------------           */


/*
 * ��������
 */
GLOBAL_DATA*	GetGlobalData();                        /* ����ϵͳȫ�����ݽṹ */

int				GetGlobalMulticastDelayTime();			/* �����鲥��Ƭ�ӳ�ʱ�� */
void			SetGlobalMulticastDelayTime(int);		/* �����鲥��Ƭ�ӳ�ʱ�� */

void     		PrintGlobalData();      				/* ��ӡϵͳȫ������ */


#endif  /* _MODEL_GLOBAL_DATA_H */
