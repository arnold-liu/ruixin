#ifndef MAIN_MACHINE_H
#define MAIN_MACHINE_H

#include <string>

using std::string;


class MachineManager
{
public:	
	virtual ~MachineManager();
	static MachineManager* getInstance();

	bool add(const char* name);
	bool del(const char* name);	
	bool del(int mid);
	bool update(int mid, char* name);	
	bool checkexist(int mid);
	bool checkexist(const char* name);
	
	bool getMachineName(int machine_id, string& mname);
	bool getPrevMachineId(int id_cur, int* id_ret);
	bool getNextMachineId(int id_cur, int* id_ret);

	bool checkLastExist();
	bool getLast(int& mid);
	bool addLast(int mid);
	bool updateLast(int mid);


private:
	MachineManager();

	
	
};













#endif
