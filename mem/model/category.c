/******************************************************************
** �ļ���:      category.c
** �� ��:       0.1
** ������:      Song Fang
** �� ��:       2005.9.11
** �� ��:       Category��Ϣ����
** �޶���¼��	
** �޶�����     �޶���      �޸���bug�����޶�ԭ��
**-----------------------------------------------------------------
******************************************************************/

/*
 * $Header: /RepLevel2/v3/src/qe/src/public/model/category.c,v 1.1 2012/03/29 01:19:51 tc\boli Exp $
 * $Revision: 1.1 $
 * $Date: 2012/03/29 01:19:51 $
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<unistd.h>

#include    "category.h"
#include    "model.h"
#include    "model_mon.h"
#include    "..//memory.h"
#include    "..//util/symbol.h"
#include    "../util/string/common_str.h"
#include    "../util/log/log.h"


/*****************************************************************
** ������:      GetCategoryList
** �� ��:       ��
** �� ��:       CATEGORY_CS[]
** ��������:    ����Category�������ʼλ��
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
CATEGORY_CS*
GetCategoryList() {
    return (CATEGORY_CS*) GetShareMemory() + GetShareMemAllocationStats()->categoryOffset;
}


/*****************************************************************
** ������:      GetCategory
** �� ��:       ��
** �� ��:       CATEGORY_CS*
** ��������:    ����ָ��λ�õ�CATEGORY_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
CATEGORY_CS*
GetCategory(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->categoryMax) {
        return NULL;
    }
    return GetCategoryList() + index;
}


/*****************************************************************
** ������:      GetCategoryById
** �� ��:       Ʒ�ִ���
** �� ��:       CATEGORY_CS*
** ��������:    ����Ʒ�ִ����Ӧ��CATEGORY_CS
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.10.10
** �� ��:		Li Bo
** �� ��:		2008.8.21 �޸�Ϊ����id��ȡƷ��(id��λ���޹�)
** �汾:        0.1
****************************************************************/
CATEGORY_CS*
GetCategoryById(int id) {
    int				i = 0;
    CATEGORY_CS*	category = NULL;
    
    for(i = 0; i < GetCategoryNumber(); i++){
    	category = GetCategory(i);
    	if(id == category->id){
    		return category;
    	}
    }
    return NULL;
}


/*****************************************************************
** ������:      GetCategoryNumber
** �� ��:       ��
** �� ��:       Category����
** ��������:    ����Category����
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
GetCategoryNumber() {
    return GetShareMemCurrentStats()->nextCategory;
}


/*****************************************************************
** ������:      AddCategory
** �� ��:       ��
** �� ��:       ������Category��λ��
** ��������:    ��Category����������Category��Ŀ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:		
** �� ��:		
** �汾:        0.1
****************************************************************/
int
AddCategory(CATEGORY_CS *category) {
    CATEGORY_CS *target = NULL;
    int         index   = GetShareMemCurrentStats()->nextCategory;
    
    /*
     * ����ڴ��Ƿ�Խ��
     */
    if (index >= GetShareMemConfigStats()->categoryMax) {
    	FATAL("Category.AddCategory()", "ȯ����������Ԥ��������ȯ�������� Current Number: %d" 
    			, index);
    	return -1;
    }

    target = GetCategory(index);
    memcpy((char*) target, (char*) category, sizeof(CATEGORY_CS));
    target->firstStock = target->lastStock = NULL_INDEX;

	GetShareMemCurrentStats()->nextCategory++;
    return index;
}


/*****************************************************************
** ������:      GetStockFromSubType
** �� ��:       ��Ʒ������
** �� ��:       ��Ʒ����
** ��������:    ���ݲ�Ʒ�����ͻ�ò�Ʒ����
** ȫ�ֱ���:    ��
** ����ģ��:    ��
** �� ��:       Li Bo
** �� ��:       2006.9.1
** �� ��:       
** �� ��:       
** �汾:        0.1
****************************************************************/
int	
GetStockFromSubType(char* pSubType){
	CATEGORY_CS*	category = NULL;
	int				i		 = 0;
	int				j		 = 0;
	
	if(pSubType == NULL){
		return -1;
	}
	
	for(i = 0; i < GetCategoryNumber(); i++){
		category = GetCategory(i);
		for(j = 0; j < CATEGORY_SUB_TYPE_NUMBER; j++){
			if(strcmp(category->subType[j], "") == 0){
				break;
			}
			if(strcmp(pSubType, category->subType[j]) == 0){
				return category->id;
			}
		}
	}
	
	return -1;
}


/*****************************************************************
** ������:      AddCategoryStock
** �� ��:       ��
** �� ��:       Category��λ��
** ��������:    ��ָ��ȯ�����������֤ȯ��Ϣ
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       Song Fang
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
int
AddCategoryStock(int categoryId, STOCK_CS *stock) {
    CATEGORY_CS *category   = NULL;
    int         lastStock   = -1;

    category = GetCategoryById(categoryId);
    if (!category) {
        return -1;
    }

    if (stock->categoryNo != category->id) {
        stock->categoryNo = category->id;
        category->securitiesNumber ++;

        if (category->firstStock < 0) {
            category->firstStock = category->lastStock = stock->stockNo;
        } else {
            lastStock = category->lastStock;
            category->lastStock = stock->stockNo;
            GetStock(lastStock)->nextCategory = stock->stockNo;
        }
    }

    return category->id;
}


/*****************************************************************
** ������:      PrintCategoryInfoToFile
** �� ��:       fp �ļ�������
** �� ��:       ��
** ��������:    ��ӡȯ����Ϣ��ָ�����ļ���
** ȫ�ֱ���:    ��
** ����ģ��:
** �� ��:       weihy
** �� ��:       2005.9.10
** �� ��:
** �� ��:
** �汾:        0.1
****************************************************************/
void
PrintCategoryInfoToFile(int fp) {
    CATEGORY_CS    *category    = NULL;
    char            buf[1024];
    int             number      = 0;
    int             i           = 0;

    number = GetCategoryNumber();

    sprintf(buf, "#****ȯ����Ϣ****\n#ȯ������: %d\n\n", number);
    write(fp, buf, strlen(buf));

    for (i = 0; i < number; i++) {
        category = GetCategory(i);

        sprintf(buf, "index: %-4d\t" \
                "id: %-4d\t" \
                "name: %-12s\t" \
                "securitiesNumber: %-4d\t" \
                "firstStock: %-4d\t" \
                "lastStock: %-4d\n"
                , i
                , category->id
                , category->name
                , category->securitiesNumber
                , category->firstStock
                , category->lastStock);

        write(fp, buf, strlen(buf));
    }
}
