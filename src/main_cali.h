#ifndef MAIN_CALI_H
#define MAIN_CALI_H



class MainCali
{
public:
	static MainCali* getInstance();
	virtual ~MainCali();

	bool checkExist(int machine_id, int id);
	bool add(int machine_id, int ad, double val, double mm, int id);
	bool del(int machine_id, int id);
	bool update(int machine_id, int id, double val, double mm);	
	bool update(int machine_id, int id, double val, double mm, int ad);
	bool getCali(int machine_id, int id, double& val, double& mm);
	bool getCaliAll(int machine_id, int id, double& val, double& mm, int& ad);

private:
	MainCali();
};
















#endif
