#include "DBUtil.h"

static DBUtil* g_DBUtil =  nullptr;

//创建数据库
void DBUtil::initDB(const char *db)
{
    //打开一个数据库，如果该数据库不存在，则创建一个数据库文件
    int result = sqlite3_open(db, &m_pDB);
    if( result != SQLITE_OK )
    {
		log( "打开数据库失败，错误码:%d ，错误原因:%s\n" , result, sqlite3_errmsg(m_pDB));
	}
}

//tableIsExist的回调函数
int isExisted( void * para, int n_column, char ** column_value, char ** column_name )
{
    bool *isExisted_=(bool*)para;
    *isExisted_=(**column_value)!='0';
    return 0;
}

//判断表格是否存在
bool DBUtil::tableIsExist(std::string name)
{
    if (m_pDB!=NULL)
    {
        //判断表是否存在
        bool tableIsExisted = false;
        std::string sqlstr  = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
        int result = sqlite3_exec(m_pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &m_errMsg);
        return tableIsExisted;
    }
    return false;
}



//在数据库中判断名为name的表示否存在，如果不存在则创建这张表
//@示例语句string sqls = "create table user(id integer,username text,password text)";
void DBUtil::createTable(std::string sql, std::string name)
{
    if (!tableIsExist(name))
    {
        //创建表，设置ID为主键，且自动增加
        int result = sqlite3_exec(m_pDB,sql.c_str(),NULL,NULL,&m_errMsg);
        if( result != SQLITE_OK )
            log( "创建表失败，错误码:%d ，错误原因:%s\n" , result, m_errMsg );
    }
    
}


//删除表格
//@示例语句sqlstr="drop table name";
void DBUtil::deleteTable(std::string sql, std::string name )
{
    if (tableIsExist(name))
    {
        int result = sqlite3_exec(m_pDB,sql.c_str(),NULL,NULL,&m_errMsg);
        if( result != SQLITE_OK )
            log( "创建表失败，错误码:%d ，错误原因:%s\n" , result, m_errMsg );
    }
}


//插入数据
//@示例语句sqlstr=" insert into MyTable_1( name ) values ( '擎天柱' ) ";
void DBUtil::insertData(std::string sql){
    int result = sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, m_errMsg);
}


//删除数据
//@示例语句sqlstr="delete from MyTable_1 where ID = 2";
void DBUtil::deleteData(std::string sql)
{
    int result=sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, m_errMsg );
}


//修改数据
void DBUtil::updateData(std::string sql)
{
    int result = sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "插入记录失败，错误码:%d ，错误原因:%s\n" , result, m_errMsg );
}


//getDataCount的回调函数
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name )
{
    int *count = (int*)para;
    *count = n_column;
    return 0;
}
//获取记录的条数
//@示例语句string sqlsssss = "select count(*) from user";
//@示例语句  取得表格字段的语句string sqlsssss = "select * from user";
int DBUtil::getDataCount(std::string sql)
{
    int count = 0;
    sqlite3_exec(m_pDB, sql.c_str(), loadRecordCount, &count, &m_errMsg);
    return count;
}

int loadRecord(void* para, int n_column, char ** column_value, char ** column_name )
{
	CCLOG("n_column:%d",n_column);
	
	TableBase* pData = (TableBase*)para;
	pData->toObject(n_column, column_value, column_name);

	return 0;
}

void DBUtil::getDataInfo(std::string sql, TableBase* pData)
{
	setdbDelegate(pData);
	sqlite3_exec(m_pDB, sql.c_str(), loadRecord, pData, &m_errMsg);
}

//关闭数据库
void DBUtil::closeDB()
{
    sqlite3_close(m_pDB);
}

DBUtil* DBUtil::getInstance()
{
	if (g_DBUtil == NULL){
		g_DBUtil = new DBUtil();
	}
	return g_DBUtil;
}

void DBUtil::release()
{
	if (g_DBUtil)
		delete g_DBUtil;
	g_DBUtil = nullptr;
}