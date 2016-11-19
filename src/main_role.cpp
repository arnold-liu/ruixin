#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gbl_define.h"
#include "main_role.h"

#include "main_db.h"

#if 0

RoleManager::RoleManager()
{
}

RoleManager::~RoleManager()
{
}

RoleManager* RoleManager::getInstance()
{
	static RoleManager rm;
	return &rm;
}

bool RoleManager::init()
{
	return readin();
}

bool RoleManager::check(const char* name, const char* pwd)
{
	if(m_rols.find(name)==m_rols.end())
	{
		return false;
	}
	
	return (m_rols[name].compare(pwd));
}

bool RoleManager::add_nosave(const char* name, const char* pwd)
{
	if(m_rols.find(name)!=m_rols.end())
	{
		return false;
	}

	m_rols[name]=pwd;

	return true;
}

bool RoleManager::add(const char* name, const char* pwd)
{
	if(!add_nosave(name, pwd))
	{
		return false;
	}
	
	return save();
}

bool RoleManager::del(const char* name, const char* pwd)
{
	if(m_rols.find(name)==m_rols.end())
	{
		return false;
	}

	m_rols.erase(name);
	
	return save();
}


enum role_type_e
{
	role_e_name=0xea,
	role_e_pwd=0xac,
};

struct role_t
{
	int _r0;
	int _r1:8;
	int type:8;
	int _r2:8;
	int len:8;
	int _r3;
};

bool RoleManager::getRoles(const char* buf, int len)
{
	char sel=0;
	struct role_t* prt=(struct role_t*)buf;

	string name;
	string pwd;
	while(len>=(int)(sizeof(role_t)))
	{		
		if(prt->type==role_e_name)
		{
			string tmp(buf+sizeof(role_t), prt->len);
			name=tmp;
		}
		else if(prt->type==role_e_pwd)
		{
			string tmp(buf+sizeof(role_t), prt->len);
			pwd=tmp;
		}

		sel++;

		if(sel==2)
		{
			add_nosave(name.c_str(), pwd.c_str());
			sel=0;
		}

		int step_len=sizeof(role_t)+prt->len;
		len-=step_len;
		buf+=step_len;
	}

	return true;
}

bool RoleManager::toRoles(char*& buf, int& len)
{
	int slen=0;
	for(mss_t::iterator it=m_rols.begin(); it!=m_rols.end(); it++)
	{
		slen+=sizeof(role_t)*2;
		slen+=it->first.length();
		slen+=it->second.length();
	}
	
	len=slen;

	if(len<=0)
	{
		return true;
	}
	
	buf=(char*)malloc(len);

	struct role_t* prt=(struct role_t*)buf;

	for(mss_t::iterator it=m_rols.begin(); it!=m_rols.end(); it++)
	{
		prt->len=it->first.length();
		prt->type=role_e_name;
		memcpy(prt+1, it->first.c_str(), prt->len);
		buf+=sizeof(role_t)+prt->len;
		prt=(struct role_t*)buf;
		
		prt->len=it->second.length();
		prt->type=role_e_pwd;
		memcpy(prt+1, it->second.c_str(), prt->len);
		buf+=sizeof(role_t)+prt->len;
		prt=(struct role_t*)buf;
	}

	return true;
}

#define roles_filename "/root/roles.dat"

bool RoleManager::readin()
{
	int fd=open(roles_filename, O_RDONLY);
	if(-1==fd)
	{
		return false;
	}

	int len=lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	char* buf=(char*)malloc(len);
	int ret=read(fd, buf, len);
	if(ret!=len)
	{
		free(buf);
		close(fd);
		return false;
	}

	if(!getRoles(buf, len))
	{
		JPRINTF(__FILE__, __LINE__, "can not get roles\n");
	}

	free(buf);
	close(fd);
	
	return true;
}

bool RoleManager::save()
{
	int fd=open(roles_filename, O_RDWR);
	if(-1==fd)
	{
		return false;
	}

	char* buf=NULL;
	int len=0;
	
	if(!toRoles(buf, len))
	{
		JPRINTF(__FILE__, __LINE__, "can not save roles\n");
	}
	
	ftruncate(fd, 0);

	if(len>0)
	{
		int ret=write(fd, buf, len);
		if(ret!=len)
		{
			free(buf);
			close(fd);
			return false;
		}
	}

	free(buf);
	close(fd);
	return true;
}


#else



#define ROLE_TABLE "role"
#define RFD_ID "id"
#define RFD_NAME "name"
#define RFD_PWD "pwd"
#define RFD_TYPE "type"



RoleManager::RoleManager()
{
}

RoleManager::~RoleManager()
{
}

RoleManager* RoleManager::getInstance()
{
	static RoleManager rm;
	return &rm;
}


bool RoleManager::check(const char* name, const char* pwd)
{
	if(NULL==name || NULL==pwd)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "RFD_PWD" from "ROLE_TABLE" where "RFD_NAME" = '%s';";
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
				case SQLITE_TEXT:   
					if(strcmp(pwd, (char*)sqlite3_column_text(stmt, i))==0)
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

bool RoleManager::checkexist(const char* name)
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
	
	const char* sql_fmt="select count(*) from "ROLE_TABLE" where "RFD_NAME"='%s';";
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

bool RoleManager::checkexist(int uid)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select count(*) from "ROLE_TABLE" where "RFD_ID"=%d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, uid);
	
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



bool RoleManager::add(const char* name, const char* pwd, int type)
{
	if(NULL==name || NULL==pwd)
	{
		return false;
	}
	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="insert into "ROLE_TABLE"("RFD_NAME","RFD_PWD","RFD_TYPE") values ('%s','%s',%d);";
	int len=strlen(sql_fmt)+strlen(name)+strlen(pwd)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, name, pwd, type);



	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);
	return bret ;
}

bool RoleManager::del(const char* name)
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
	
	const char* sql_fmt="delete from "ROLE_TABLE" where "RFD_NAME" = '%s';";
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

bool RoleManager::del(int rid)
{
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="delete from "ROLE_TABLE" where "RFD_ID" = %d;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, rid);

	bool bret=false;
	int ret=sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	free(sql);	
	return bret ;
}


bool RoleManager::update(int mid, char* name, char* pwd, int type)
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

	bool need_commer=false;
	string sql_str="update "ROLE_TABLE" set ";
	if(name && strlen(name))
	{	
		sql_str+=" "RFD_NAME" = '";
		sql_str+=name;
		sql_str+="' ";
		need_commer=true;
	}

	if(pwd && strlen(pwd))
	{
		if(need_commer)
		{
			sql_str+=",";
		}
		else
		{
			need_commer=true;
		}
		sql_str+=RFD_PWD" = '";
		sql_str+=pwd;
		sql_str+="' ";
	}

	if(type>=0)
	{
		char str_type[16];
		snprintf(str_type, sizeof(str_type), "%d", type);
		if(need_commer)
		{
			sql_str+=",";
		}
		sql_str+=RFD_TYPE" = ";
		sql_str+=str_type;
	}

	char str_id[16];
	snprintf(str_id, sizeof(str_id), "%d", mid);
	sql_str+=" where "RFD_ID" = ";
	sql_str+=str_id;
	sql_str+=" ;";
	

	bool bret=false;
	int ret=sqlite3_exec(db, sql_str.c_str(), NULL, NULL, NULL);
	if(ret==SQLITE_OK)
	{
		bret=true;
	}

	return bret ;
}


bool RoleManager::getUserName(int uid, string& mname)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "RFD_NAME" from "ROLE_TABLE" where "RFD_ID" = %d ;";
	int len=strlen(sql_fmt)+16+1;
	char* sql=(char*)malloc(len);
	snprintf(sql, len, sql_fmt, uid);

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


bool RoleManager::getPrevUserId(int id_cur, int* id_ret)
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
	
	const char* sql_fmt="select "RFD_ID" from "ROLE_TABLE" where "RFD_ID" < %d order by "RFD_ID" desc limit 1;";
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

bool RoleManager::getNextUserId(int id_cur, int* id_ret)
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
	
	const char* sql_fmt="select "RFD_ID" from "ROLE_TABLE" where "RFD_ID" > %d order by "RFD_ID" asc limit 1;";
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



bool RoleManager::getRoleType(const char* name, unsigned int& type)
{	
	sqlite3* db=MainDB::getInstance()->getdb();
	if(NULL==db)
	{
		return false;
	}
	
	const char* sql_fmt="select "RFD_TYPE" from "ROLE_TABLE" where "RFD_NAME" = '%s';";
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
			switch( vtype )  
			{  
				case SQLITE_INTEGER:   
					type=sqlite3_column_int(stmt, i);					
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




#endif
