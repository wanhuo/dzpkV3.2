#include "DBUtil.h"

static DBUtil* g_DBUtil =  nullptr;

//�������ݿ�
void DBUtil::initDB(const char *db)
{
    //��һ�����ݿ⣬��������ݿⲻ���ڣ��򴴽�һ�����ݿ��ļ�
    int result = sqlite3_open(db, &m_pDB);
    if( result != SQLITE_OK )
    {
		log( "�����ݿ�ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, sqlite3_errmsg(m_pDB));
	}
}

//tableIsExist�Ļص�����
int isExisted( void * para, int n_column, char ** column_value, char ** column_name )
{
    bool *isExisted_=(bool*)para;
    *isExisted_=(**column_value)!='0';
    return 0;
}

//�жϱ���Ƿ����
bool DBUtil::tableIsExist(std::string name)
{
    if (m_pDB!=NULL)
    {
        //�жϱ��Ƿ����
        bool tableIsExisted = false;
        std::string sqlstr  = "select count(type) from sqlite_master where type='table' and name ='"+name+"'";
        int result = sqlite3_exec(m_pDB, sqlstr.c_str(), isExisted, &tableIsExisted, &m_errMsg);
        return tableIsExisted;
    }
    return false;
}



//�����ݿ����ж���Ϊname�ı�ʾ����ڣ�����������򴴽����ű�
//@ʾ�����string sqls = "create table user(id integer,username text,password text)";
void DBUtil::createTable(std::string sql, std::string name)
{
    if (!tableIsExist(name))
    {
        //����������IDΪ���������Զ�����
        int result = sqlite3_exec(m_pDB,sql.c_str(),NULL,NULL,&m_errMsg);
        if( result != SQLITE_OK )
            log( "������ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, m_errMsg );
    }
    
}


//ɾ�����
//@ʾ�����sqlstr="drop table name";
void DBUtil::deleteTable(std::string sql, std::string name )
{
    if (tableIsExist(name))
    {
        int result = sqlite3_exec(m_pDB,sql.c_str(),NULL,NULL,&m_errMsg);
        if( result != SQLITE_OK )
            log( "������ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, m_errMsg );
    }
}


//��������
//@ʾ�����sqlstr=" insert into MyTable_1( name ) values ( '������' ) ";
void DBUtil::insertData(std::string sql){
    int result = sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, m_errMsg);
}


//ɾ������
//@ʾ�����sqlstr="delete from MyTable_1 where ID = 2";
void DBUtil::deleteData(std::string sql)
{
    int result=sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, m_errMsg );
}


//�޸�����
void DBUtil::updateData(std::string sql)
{
    int result = sqlite3_exec(m_pDB, sql.c_str() , NULL, NULL, &m_errMsg);
    if(result != SQLITE_OK )
        log( "�����¼ʧ�ܣ�������:%d ������ԭ��:%s\n" , result, m_errMsg );
}


//getDataCount�Ļص�����
int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name )
{
    int *count = (int*)para;
    *count = n_column;
    return 0;
}
//��ȡ��¼������
//@ʾ�����string sqlsssss = "select count(*) from user";
//@ʾ�����  ȡ�ñ���ֶε����string sqlsssss = "select * from user";
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

//�ر����ݿ�
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