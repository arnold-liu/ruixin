
#include <stdio.h>
#include "main_db.h"
#include "gbl_define.h"


#define DB_NAME "main.db"



MainDB* MainDB::getInstance()
{
	static MainDB mdb;
	return &mdb;
}

MainDB::~MainDB()
{
}

MainDB::MainDB()
{
	if(SQLITE_OK!=sqlite3_open(DB_NAME, &m_sqlite))
	{
		JPRINTF(__FILE__, __LINE__, "can not open sqlite db\n");
	}
}

sqlite3* MainDB::getdb()
{
	return m_sqlite;
}







