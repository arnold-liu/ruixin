#ifndef HS_UDEV_H
#define HS_UDEV_H



#include <sys/types.h>
#include <pthread.h>
#include <map>
#include <list>
#include <deque>

#include <string>



using namespace std;



enum part_type_e
{	
	PART_TYPE_UNKNOW,
	PART_TYPE_USB,
	
	PART_TYPE_MAX
};


struct udevice
{
	string dev_name;
	int type;
	unsigned int major;
	unsigned int minor;
};



typedef list<udevice*> PART_LIST;



class Hsudev
{
private:
	PART_LIST m_parts;
	pthread_mutex_t m_part_lock;

private:
	Hsudev();

	static void* procDevice(void* arg);

	static int init_hotplug_sock();	
	
	void part_lockon();
	void part_lockoff();
	
	void checkDiskUevent(const string& uevent, const list<dev_t>& procPts);
	void searchStaticDisk(const string& path, const list<dev_t>& procPts);

	
	void tryprocpt(list<dev_t>& procPts);

	bool addPartition(udevice* part);
	void delPartition(const string name);	

	
public:
	pthread_t m_pid;


public:
	static Hsudev* getInstance();
	static void destroy();
	~Hsudev();

	bool startFilter();

	bool checkUsb();
	//int export_db();

	int getUsbExportFd();


};







bool sig_unblock();
bool sig_notify_thread(pthread_t& pid);
bool sig_block();













#endif
