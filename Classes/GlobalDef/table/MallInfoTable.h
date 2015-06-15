#ifndef _MALL_INFO_TABLE_
#define _MALL_INFO_TABLE_

#include "cocos2d.h"
#include <iostream>
#include "../../Utils/TableBase.h"
#include "../GlobalDef.h"
#include "../../GlobalDef/JsonWritter.h"

USING_NS_CC;
using namespace std;

class MallInfoTable : public TableBase {
public:
	
	CC_SYNTHESIZE(string, mall_path, mallPath);
	CC_SYNTHESIZE(string, path_md5, pathMD5);
	CC_SYNTHESIZE(string, local_path, localPath);

	virtual int toObject(int n_column, char ** column_value, char ** column_name);

	MallInfoTable(); 
	~MallInfoTable() {};
};

#endif