#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gbl_define.h"

#include "main_record.h"
#include "main_db.h"
#include "main_trans.h"

#include "main_machine.h"

#define MAX_RECORD_LIMIT 1000



#define RTABLE "record"
#define RFD_ID "id"
#define RFD_MID "machine_id"
#define RFD_VAL "val"
#define RFD_TIME "time"




MainRecord* MainRecord::getInstance()
{
	static MainRecord mr;
	return &mr;
}

MainRecord::~MainRecord()
{
	
}

bool getDateTime(string& dt)
{	
	struct tm tm;
	time_t tt=time(NULL);
	if(localtime_r(&tt, &tm))
	{
		char dtime[32];
		snprintf(dtime, sizeof(dtime), "%4d-%02d-%02d %02d:%02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		dt=dtime;
		return true;
	}
	
	return false;
}

int MainRecord::getMachineRecord(int machine_id, record_t* rec, int cnt)
{
	if(NULL==rec || cnt<=0)
	{
		return 0;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return -1;
	}
	
	const char* sql_fmt="select "RFD_VAL","RFD_TIME" from "RTABLE" where "RFD_MID" = %d order by "RFD_ID" desc limit %d;";
	int len=strlen(sql_fmt)+16*2+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, cnt);

	int ret;
	int row=0;
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
				case SQLITE_FLOAT:   
					rec[row].val=sqlite3_column_double(stmt, i);
				break;
				case SQLITE_TEXT:					
					rec[row].time=(const char*)sqlite3_column_text(stmt, i);
				break;
			}  
		}

		row++;
	}  

out2:
	sqlite3_finalize(stmt);
out1:	
	free(sql);	
	return row ;
}

bool MainRecord::addNewRecord(int machine_id, double val)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return -1;
	}

	string dt;

	getDateTime(dt);
	
	const char* sql_fmt="insert into "RTABLE"("RFD_MID","RFD_VAL","RFD_TIME") values (%d,%e,'%s');";
	int len=strlen(sql_fmt)+16*2+dt.size()+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, val, dt.c_str());

	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	
	free(sql);

	int cnt=getRecordCount(machine_id);
	if(cnt>=MAX_RECORD_LIMIT)
	{
		clearRecord(machine_id, cnt/2);
	}
	
	return (ret==SQLITE_OK) ;
}


MainRecord::MainRecord()
{
}


unsigned int MainRecord::getRecordCount(int machine_id)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return 0;
	}
	
	const char* sql_fmt="select count(*) from "RTABLE" where "RFD_MID" = %d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id);

	int row=0;
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
					row=sqlite3_column_int(stmt, i);									
				break;				
			}  
		}
	}
	
out2:
	sqlite3_finalize(stmt);
out1:	
	free(sql);
	
	return row ;
}



bool MainRecord::clearRecord(int machine_id, int cnt)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="delete from "RTABLE" where "RFD_ID" in (select "RFD_ID" from "RTABLE" where "RFD_MID" = %d order by "RFD_ID" asc limit 0,%d );";
	int len=strlen(sql_fmt)+16*2+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, machine_id, cnt);

	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	
	free(sql);	
	return (ret==SQLITE_OK) ;
}


int MainRecord::saveMachineRecordFile(int fd, int machine_id, string& stime, string& dtime)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return -1;
	}

	string sql_str="select "RFD_ID","RFD_VAL","RFD_TIME" from "RTABLE" where "RFD_MID" = ";
	
	char str_mac[16];
	snprintf(str_mac, sizeof(str_mac), "%d", machine_id);
	sql_str+=str_mac;

	if(stime.size())
	{
		sql_str+=" and "RFD_TIME" >= ' ";
		sql_str+=stime.c_str();
		sql_str+=" ' ";
	}

	if(dtime.size())
	{
		sql_str+=" and "RFD_TIME" <= ' ";
		sql_str+=dtime.c_str();
		sql_str+=" ' ";
	}

	
	sql_str+=" ;";

printf("----sql [%s]\n", sql_str.c_str());

	int ret=0;
	int ncolumn;
	sqlite3_stmt* stmt = NULL;
	ret=sqlite3_prepare_v2(db, sql_str.c_str(), -1, &stmt, NULL);
	if (ret != SQLITE_OK)
	{
		ret=-2;
		goto out1;
	}

	ncolumn = sqlite3_column_count(stmt);
	if(ncolumn<=0)
	{		
		goto out2;
	}	
	
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		string buffer;
		
		for(int i = 0 ; i < ncolumn ; i++ )
		{  
			int vtype = sqlite3_column_type(stmt , i);  
			switch( vtype )  
			{  
				case SQLITE_INTEGER:
					do
					{
						char str_int[16];
						snprintf(str_int, sizeof(str_int), "%d", sqlite3_column_int(stmt, i));
						buffer+="\t";
						buffer+="id: ";
						buffer+=str_int;
					}while(0);
				break;
				case SQLITE_FLOAT:
					do
					{
						char str_float[16];
						snprintf(str_float, sizeof(str_float), "%f", sqlite3_column_double(stmt, i));
						buffer+="\t";
						buffer+="val: ";
						buffer+=str_float;
					}while(0);
				break;
				case SQLITE_TEXT:
					buffer+="\t";
					buffer+="time: ";
					buffer+=(const char*)sqlite3_column_text(stmt, i);
					
				break;
			}  
		}

		buffer+="\r\n";
		
		errno=0;
		int wret=write(fd, buffer.c_str(), buffer.size());
		if(wret!=(int)buffer.size())
		{
			ret=errno;
			break;
		}
	}
	
out2:
	sqlite3_finalize(stmt);
out1:	
	
	return ret ;
}

int MainRecord::saveToFile(int fd, int mid, string& stime, string& dtime)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return -1;
	}

	MachineManager* mmgr=MachineManager::getInstance();
	
	int machine_id=0;
	
	while(1)
	{
		int tmp_mac;

		if(mid)
		{
			tmp_mac=mid;
		}
		else if(!mmgr->getNextMachineId(machine_id, &tmp_mac))
		{
			break;
		}
		

		if(tmp_mac!=machine_id)
		{
			machine_id=tmp_mac;
			string mname;
			if(!mmgr->getMachineName(machine_id, mname))
			{
				break;
			}

			mname+=":\r\n";
			errno=0;
			int wret=write(fd, mname.c_str(), mname.size());
			if(wret!=(int)mname.size())
			{
				DPRINTF("can not write. %m\n");
				return errno;
			}
			int ret=saveMachineRecordFile(fd, machine_id, stime, dtime);
			if(ret)
			{
				return ret;
			}
		}
		else
		{
			break;
		}
		
	}
	
	return 0;
}











