





#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbl_define.h"
#include "main_machine.h"

#include "main_db.h"


MachineManager::MachineManager()
{
}

MachineManager::~MachineManager()
{
}

MachineManager* MachineManager::getInstance()
{
	static MachineManager rm;
	return &rm;
}


#define MACHINE_TABLE "machine"
#define FIELD_NAME "name"
#define FIELD_ID "id"



#define LASTMAC_TABLE "lastmac"
#define LFD_ID "id"


bool MachineManager::checkexist(const char* name)
{
	if(NULL==name)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select count(*) from "MACHINE_TABLE" where "FIELD_NAME"='%s';";
	int len=strlen(sql_fmt)+strlen(name)+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, name);
	
	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			//const char* column_name = sqlite3_column_name(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					if(sqlite3_column_int(stmt, i)>0)
					{
						bret=true;
						break;
					}
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	free(sql);
	
	return bret ;	
}


bool MachineManager::checkexist(int mid)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select count(*) from "MACHINE_TABLE" where "FIELD_ID"=%d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, mid);
	
	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			//const char* column_name = sqlite3_column_name(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					if(sqlite3_column_int(stmt, i)>0)
					{
						bret=true;
						break;
					}
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	free(sql);
	
	return bret ;	
}



bool MachineManager::add(const char* name)
{
	if(NULL==name)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="insert into "MACHINE_TABLE"("FIELD_NAME") values ('%s');";
	int len=strlen(sql_fmt)+strlen(name)+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, name);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}

bool MachineManager::del(const char* name)
{
	if(NULL==name)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="delete from "MACHINE_TABLE" where "FIELD_NAME" = '%s';";
	int len=strlen(sql_fmt)+strlen(name)+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, name);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}

bool MachineManager::del(int mid)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="delete from "MACHINE_TABLE" where "FIELD_ID" = %d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, mid);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}

bool MachineManager::update(int mid, char* name)
{
	if(NULL==name)
	{
		return false;
	}

	if(strlen(name)<=0)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="update "MACHINE_TABLE" set "FIELD_NAME"= '%s' where "FIELD_ID" = %d;";
	int len=strlen(sql_fmt)+16+strlen(name)+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, name, mid);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}


bool MachineManager::getMachineName(int machine_id, string& mname)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "FIELD_NAME" from "MACHINE_TABLE" where "FIELD_ID" = %d ;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id);

	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{
			int vtype = sqlite3_column_type(stmt , i);  
			switch( vtype )  
			{
				case SQLITE_TEXT:
					do
					{
						const char* ptr=(const char*)sqlite3_column_text(stmt, i);
						mname=(const char*)ptr;
						bret=true;

						//showString(ptr);
						
					}while(0);
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:	
	free(sql);	
	return bret ;
}


bool MachineManager::getPrevMachineId(int id_cur, int* id_ret)
{
	if(NULL==id_ret)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "FIELD_ID" from "MACHINE_TABLE" where "FIELD_ID" < %d order by "FIELD_ID" desc limit 1;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, id_cur);

	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			//const char* column_name = sqlite3_column_name(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					*id_ret=sqlite3_column_int(stmt, i);					
					bret=true;					
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:	
	free(sql);	
	return bret ;
}

bool MachineManager::getNextMachineId(int id_cur, int* id_ret)
{
	if(NULL==id_ret)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "FIELD_ID" from "MACHINE_TABLE" where "FIELD_ID" > %d order by "FIELD_ID" asc limit 1;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, id_cur);

	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					*id_ret=sqlite3_column_int(stmt, i);					
					bret=true;					
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:	
	free(sql);	
	return bret ;
}








bool MachineManager::checkLastExist()
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql="select count(*) from "LASTMAC_TABLE" ;";
	
	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					if(sqlite3_column_int(stmt, i)>0)
					{
						bret=true;
						break;
					}
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	
	return bret ;	
}

bool MachineManager::getLast(int& mid)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql="select "LFD_ID" from "LASTMAC_TABLE" ;";
	
	bool bret=false;
	int ret;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{
		goto out2;
	}
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{     
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					mid=sqlite3_column_int(stmt, i);
					bret=true;
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	
	return bret ;	
}


bool MachineManager::addLast(int mid)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="insert into "LASTMAC_TABLE"("LFD_ID") values (%d);";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, mid);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}


bool MachineManager::updateLast(int mid)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="update "LASTMAC_TABLE" set "LFD_ID" = %d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, mid);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}

