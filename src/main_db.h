#ifndef MAIN_DB_H
#define MAIN_DB_H

#include <sqlite3.h>


class MainDB
{
private:
	sqlite3* m_sqlite;
	
public:
	static MainDB* getInstance();
	virtual ~MainDB();

	sqlite3* getdb();
private:
	MainDB();
};














#endif
