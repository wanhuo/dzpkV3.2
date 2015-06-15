#ifndef _DB_UTIL_
#define _DB_UTIL_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "TableBase.h"
#include "DBDelegate.h"
#include "external/sqlite3/include/sqlite3.h"

USING_NS_CC;
USING_NS_CC_EXT;
/************************************************************
            封装 sqlite3操作
************************************************************/

class DBUtil {    
public:
	CC_SYNTHESIZE(DBDelegate*, pDelegate, dbDelegate);

    //  打开数据库,db为数据库的名字
    void initDB(const char *db);
    
    //用来判断表格是否存在
    // name:表示表名
    bool tableIsExist(std::string name);
    
    
    //用来创建一个表名为name的表格，创建时会先匹配时否有该表的存在如果存在则不创建
    //创建表
    void createTable(std::string sql, std::string name);
    
    //用来删除一张表名为name的表格，删除时会先匹配是否有该表的存在如果不存在则不执行删除操作
    //删除表名
    void deleteTable(std::string sql, std::string name);
    
    //用来向表中插入数据
    void insertData(std::string sql);
    
    //用来向表中删除数据
    void deleteData(std::string sql);
    
    //用来向表中修改数据
    void updateData(std::string sql);
    
    //获取记录的条数
    int getDataCount(std::string sql);
    
    //读取记录的信息
    void getDataInfo(std::string sql, TableBase* pData);
	
    
    //关闭打开的数据库
    void closeDB();

	// 单例对象
	static DBUtil* getInstance();

	static void release();
private:
	sqlite3 *m_pDB; // 数据库指针
	char* m_errMsg; // 全局错误信息
};

#endif