#include "MallInfoTable.h"


MallInfoTable::MallInfoTable()
{
	mall_path = "";
	path_md5 = "";
	local_path = "";
}

int MallInfoTable::toObject(int n_column, char ** column_value, char ** column_name)
{
	MallInfoTable* table = new MallInfoTable();

	for (int i=0; i<n_column; i++)
	{
		if (strcmp(column_name[i], "mall_path") == 0) {
			table->setmallPath(column_value[i]);
		} else if (strcmp(column_name[i], "path_md5") == 0) {
			table->setpathMD5(column_value[i]);
		} else if (strcmp(column_name[i], "local_path") == 0) {
			table->setlocalPath(column_value[i]);
		} else {
			continue;
		}
	}

	m_datas.pushBack(table);

	return 0;
}