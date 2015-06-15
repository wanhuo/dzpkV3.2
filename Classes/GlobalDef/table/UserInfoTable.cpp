#include "UserInfoTable.h"


UserInfoTable::UserInfoTable()
{
	user_id = "";
	account = "";
	password = "";
	sessionid = "";
}

int UserInfoTable::toObject(int n_column, char ** column_value, char ** column_name)
{
	UserInfoTable* table = new UserInfoTable();

	for (int i=0; i<n_column; i++)
	{
		if (strcmp(column_name[i], "user_id") == 0) {
			table->setUserId(column_value[i]);
		} else if (strcmp(column_name[i], "account") == 0) {
			table->setAccount(column_value[i]);
		} else if (strcmp(column_name[i], "password") == 0) {
			table->setPassword(column_value[i]);
		}else if (strcmp(column_name[i], "sessionid") == 0) {
			table->setSessionId(column_value[i]);
		} else {
			continue;
		}
	}

	m_datas.pushBack(table);

	return 0;
}