#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbl_define.h"
#include "main_db.h"
#include "main_cali.h"


#define CALI_TABLE "cali"
#define CFD_ID "id"
#define CFD_AD "ad"
#define CFD_VAL "val"
#define CFD_MM "mm"
#define CFD_MID "machine_id"




MainCali* MainCali::getInstance()
{
	static MainCali mc;
	return &mc;
}

MainCali::~MainCali()
{
}

bool MainCali::checkExist(int machine_id, int id)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select count(*) from "CALI_TABLE" where "CFD_MID"=%d and "CFD_ID"=%d;";
	int len=strlen(sql_fmt)+16*2+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, id);
	
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
	free(sql);
	
	return bret ;	

}

bool MainCali::add(int machine_id, int ad, double val, double mm, int id)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="insert into "CALI_TABLE"("CFD_ID","CFD_AD","CFD_VAL","CFD_MID","CFD_MM") values (%d,%d,%e,%d,%e);";
	int len=strlen(sql_fmt)+16*3+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, id, ad, val, machine_id, mm);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}

bool MainCali::del(int machine_id, int id)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="delete from "CALI_TABLE" where "CFD_ID" = %d and "CFD_MID" = %d;";
	int len=strlen(sql_fmt)+16*2+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, id, machine_id);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;

}

bool MainCali::update(int machine_id, int id, double val, double mm)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="update "CALI_TABLE" set "CFD_VAL"=%e, "CFD_MM"=%e where "CFD_ID" = %d and "CFD_MID" = %d;";
	int len=strlen(sql_fmt)+16*3+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, val, mm, id, machine_id);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}


bool MainCali::update(int machine_id, int id, double val, double mm, int ad)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="update "CALI_TABLE" set "CFD_VAL"=%e, "CFD_MM"=%e, "CFD_AD"=%d where "CFD_ID" = %d and "CFD_MID" = %d;";
	int len=strlen(sql_fmt)+16*3+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, val, mm, ad, id, machine_id);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}



bool MainCali::getCali(int machine_id, int id, double& val, double& mm)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "CFD_VAL","CFD_MM" from "CALI_TABLE" where "CFD_MID"=%d and "CFD_ID"=%d;";
	int len=strlen(sql_fmt)+16*3+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, id);
	
	bool bret_val=false, bret_mm=false;
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
				case SQLITE_FLOAT:
					if(0==i)
					{
						val=sqlite3_column_double(stmt, i);					
						bret_val=true;
					}
					else if(1==i)
					{
						mm=sqlite3_column_double(stmt, i);					
						bret_mm=true;
					}
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	free(sql);
	
	return (bret_val && bret_mm);
}



bool MainCali::getCaliAll(int machine_id, int id, double& val, double& mm, int& ad)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "CFD_VAL","CFD_MM","CFD_AD" from "CALI_TABLE" where "CFD_MID"=%d and "CFD_ID"=%d;";
	int len=strlen(sql_fmt)+16*3+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, id);
	
	bool bret_val=false, bret_mm=false, bret_ad=false;
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
				case SQLITE_FLOAT:
					if(0==i)
					{
						val=sqlite3_column_double(stmt, i);					
						bret_val=true;
					}
					else if(1==i)
					{
						mm=sqlite3_column_double(stmt, i);					
						bret_mm=true;
					}
				break;
				case SQLITE_INTEGER:
					ad=sqlite3_column_int(stmt, i);
					bret_ad=true;
				break;
			}  
		}  
	}  

out2:
	sqlite3_finalize(stmt);
out1:
	free(sql);
	
	return (bret_val && bret_mm && bret_ad);
}




MainCali::MainCali()
{
}



