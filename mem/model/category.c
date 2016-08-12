/******************************************************************
** 文件名:      category.c
** 版 本:       0.1
** 创建人:      Song Fang
** 日 期:       2005.9.11
** 描 述:       Category信息定义
** 修订记录：	
** 修订日期     修订人      修复的bug或者修订原因
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
** 函数名:      GetCategoryList
** 输 入:       无
** 输 出:       CATEGORY_CS[]
** 功能描述:    返回Category数组的起始位置
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
CATEGORY_CS*
GetCategoryList() {
    return (CATEGORY_CS*) GetShareMemory() + GetShareMemAllocationStats()->categoryOffset;
}


/*****************************************************************
** 函数名:      GetCategory
** 输 入:       无
** 输 出:       CATEGORY_CS*
** 功能描述:    返回指定位置的CATEGORY_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
CATEGORY_CS*
GetCategory(int index) {
    if (index < 0 || index >= GetShareMemConfigStats()->categoryMax) {
        return NULL;
    }
    return GetCategoryList() + index;
}


/*****************************************************************
** 函数名:      GetCategoryById
** 输 入:       品种代码
** 输 出:       CATEGORY_CS*
** 功能描述:    返回品种代码对应的CATEGORY_CS
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.10.10
** 修 改:		Li Bo
** 日 期:		2008.8.21 修改为根据id获取品种(id跟位置无关)
** 版本:        0.1
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
** 函数名:      GetCategoryNumber
** 输 入:       无
** 输 出:       Category总数
** 功能描述:    返回Category数量
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
int
GetCategoryNumber() {
    return GetShareMemCurrentStats()->nextCategory;
}


/*****************************************************************
** 函数名:      AddCategory
** 输 入:       无
** 输 出:       新增的Category的位置
** 功能描述:    向Category数组中增加Category条目
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:		
** 日 期:		
** 版本:        0.1
****************************************************************/
int
AddCategory(CATEGORY_CS *category) {
    CATEGORY_CS *target = NULL;
    int         index   = GetShareMemCurrentStats()->nextCategory;
    
    /*
     * 检查内存是否越界
     */
    if (index >= GetShareMemConfigStats()->categoryMax) {
    	FATAL("Category.AddCategory()", "券种数量超出预定义的最大券种数量！ Current Number: %d" 
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
** 函数名:      GetStockFromSubType
** 输 入:       产品子类型
** 输 出:       产品类型
** 功能描述:    根据产品子类型获得产品类型
** 全局变量:    无
** 调用模块:    无
** 作 者:       Li Bo
** 日 期:       2006.9.1
** 修 改:       
** 日 期:       
** 版本:        0.1
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
** 函数名:      AddCategoryStock
** 输 入:       无
** 输 出:       Category的位置
** 功能描述:    向指定券种中添加所属证券信息
** 全局变量:    无
** 调用模块:
** 作 者:       Song Fang
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
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
** 函数名:      PrintCategoryInfoToFile
** 输 入:       fp 文件描述符
** 输 出:       无
** 功能描述:    打印券种信息到指定的文件中
** 全局变量:    无
** 调用模块:
** 作 者:       weihy
** 日 期:       2005.9.10
** 修 改:
** 日 期:
** 版本:        0.1
****************************************************************/
void
PrintCategoryInfoToFile(int fp) {
    CATEGORY_CS    *category    = NULL;
    char            buf[1024];
    int             number      = 0;
    int             i           = 0;

    number = GetCategoryNumber();

    sprintf(buf, "#****券种信息****\n#券种总数: %d\n\n", number);
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
