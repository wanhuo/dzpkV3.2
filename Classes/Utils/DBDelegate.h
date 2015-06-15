#ifndef _DB_DELEGATE_
#define _DB_DELEGATE_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class DBDelegate {
public:
	virtual int toObject(int n_column, char ** column_value, char ** column_name) { return 0; };
};

#endif