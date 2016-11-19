#ifndef MAIN_RECORD_H
#define MAIN_RECORD_H


#include <string>

using std::string;



class record_t
{
public:
	const char* machine;
	double val;
	string time;

	record_t()
	{
		machine=NULL;
		val=0.0;
	}
};



class MainRecord
{
public:
	static MainRecord* getInstance();
	virtual ~MainRecord();

	int getMachineRecord(int machine_id, record_t* rec, int cnt);
	bool addNewRecord(int machine_id, double val);

	int saveToFile(int fd, int mid, string& stime, string& dtime);
private:
	MainRecord();

	unsigned int getRecordCount(int machine_id);
	bool clearRecord(int machine_id, int cnt);

	int saveMachineRecordFile(int fd, int machine_id, string& stime, string& dtime);

};















#endif
