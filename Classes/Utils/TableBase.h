#ifndef _TABLE_BASE_
#define _TABLE_BASE_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DBDelegate.h"
#include "external/sqlite3/include/sqlite3.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TableBase : public Ref 
				, public DBDelegate {
public:
	Vector<TableBase*> m_datas;

public:
	TableBase() {};
	~TableBase() {
        m_datas.clear();
    };

	virtual int toObject(int n_column, char ** column_value, char ** column_name) { return 0; };
};

#endif