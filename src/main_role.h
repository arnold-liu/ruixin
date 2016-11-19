#ifndef MAIN_ROLE_H
#define MAIN_ROLE_H


#include <string>

using std::string;

class RoleManager
{
public:	
	virtual ~RoleManager();
	static RoleManager* getInstance();

	bool check(const char* name, const char* pwd);
	bool add(const char* name, const char* pwd, int type);
	bool del(const char* name);	
	bool del(int rid);
	bool checkexist(const char* name);	
	bool checkexist(int uid);
	bool getRoleType(const char* name, unsigned int& type);
	
	bool update(int mid, char* name, char* pwd, int type);
	bool getUserName(int uid, string& mname);	
	bool getPrevUserId(int id_cur, int* id_ret);
	bool getNextUserId(int id_cur, int* id_ret);

private:
	RoleManager();

	
	
};







#endif 
