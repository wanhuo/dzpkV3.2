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
            ��װ sqlite3����
************************************************************/

class DBUtil {    
public:
	CC_SYNTHESIZE(DBDelegate*, pDelegate, dbDelegate);

    //  �����ݿ�,dbΪ���ݿ������
    void initDB(const char *db);
    
    //�����жϱ���Ƿ����
    // name:��ʾ����
    bool tableIsExist(std::string name);
    
    
    //��������һ������Ϊname�ı�񣬴���ʱ����ƥ��ʱ���иñ�Ĵ�����������򲻴���
    //������
    void createTable(std::string sql, std::string name);
    
    //����ɾ��һ�ű���Ϊname�ı��ɾ��ʱ����ƥ���Ƿ��иñ�Ĵ��������������ִ��ɾ������
    //ɾ������
    void deleteTable(std::string sql, std::string name);
    
    //��������в�������
    void insertData(std::string sql);
    
    //���������ɾ������
    void deleteData(std::string sql);
    
    //����������޸�����
    void updateData(std::string sql);
    
    //��ȡ��¼������
    int getDataCount(std::string sql);
    
    //��ȡ��¼����Ϣ
    void getDataInfo(std::string sql, TableBase* pData);
	
    
    //�رմ򿪵����ݿ�
    void closeDB();

	// ��������
	static DBUtil* getInstance();

	static void release();
private:
	sqlite3 *m_pDB; // ���ݿ�ָ��
	char* m_errMsg; // ȫ�ִ�����Ϣ
};

#endif