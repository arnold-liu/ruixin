

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>  
#include <sys/un.h>  
#include <sys/ioctl.h>  
#include <sys/socket.h>  
#include <linux/types.h>  
#include <linux/netlink.h>  
#include <errno.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <fcntl.h>
#include <signal.h>
#include <sys/mount.h>

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <fstream>


#include <dirent.h>
#include <sys/vfs.h>

#include <limits.h>


#include "gbl_define.h"
#include "hs_udev.h"
#include "main_record.h"



#define UEVENT_BUFFER_SIZE 2048  

#define NOTIFY_SIG SIGUSR1


bool sig_notify_thread(pthread_t& pid)
{
	int ret=pthread_kill(pid, NOTIFY_SIG);
	if(ret)
	{
		DPRINTF("sigmask error:[%d] %m\n", ret);
		return false;
	}

	return true;
}

bool sig_block()
{
	sigset_t m_sigset;
	sigemptyset(&m_sigset);	
	sigaddset(&m_sigset, NOTIFY_SIG);

	int ret=pthread_sigmask(SIG_BLOCK, &m_sigset, NULL);
	if(ret)
	{
		DPRINTF("sigmask error:[%d] %m\n", ret);
		return false;
	}

	return true;
}

bool sig_unblock()
{
	sigset_t m_sigset;
	sigemptyset(&m_sigset);	
	sigaddset(&m_sigset, NOTIFY_SIG);

	int ret=pthread_sigmask(SIG_UNBLOCK, &m_sigset, NULL);
	if(ret)
	{
		DPRINTF("sigmask error:[%d] %m\n", ret);
		return false;
	}

	return true;
}




int Hsudev::init_hotplug_sock()  
{  
	const int buffersize = 1024;  
	int ret;  

	struct sockaddr_nl snl;  
	bzero(&snl, sizeof(struct sockaddr_nl));  
	snl.nl_family = AF_NETLINK;  
	snl.nl_pid = getpid();  
	snl.nl_groups = 1;  

	int s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);  
	if (s == -1)   
	{  
		perror("socket");  
		return -1;  
	}  
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));  

	ret = bind(s, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));  
	if (ret < 0)   
	{  
		perror("bind");  
		close(s);  
		return -1;  
	}  

	return s;  
}  


Hsudev::Hsudev()
{
	pthread_mutex_init(&m_part_lock, NULL);
}


bool Hsudev::addPartition(udevice* part)
{	
	string mount_src("/dev/");
	string mount_dst("/mnt/");

	const char* support_fs[]=
	{
		"ext4",
		"vfat",
		"ntfs",
		//"xfs",
		//"reiserfs",
	};
	
	mount_src.append(part->dev_name);
	mount_dst.append(part->dev_name);

sleep(1);

	while(access(mount_src.c_str(), F_OK))
	{	
		if(-1==mknod(mount_src.c_str(), 0660|S_IFBLK, makedev(part->major, part->minor)))
		{
			if(EEXIST!=errno)
			{
				DPRINTF("can not create [%s] dev node:%m\n", mount_src.c_str());
			}
			else
			{
				DPRINTF("dev node [%s] exist\n", mount_src.c_str());
			}
		}
		else
		{
			DPRINTF("create dev node [%s] major:%d minor:%d \n", mount_src.c_str(), part->major, part->minor);
		}
	}	

	if(access(mount_dst.c_str(), F_OK))
	{
		mkdir(mount_dst.c_str(), 0555);
	}	

	bool mount_ok=false;
	
	part_lockon();

	unsigned long mount_flag=0;

	for(int fori=0; fori<(int)ARRAY_SIZE(support_fs); fori++)
	{		
		bool bMount=false;

		if(NULL==support_fs[fori])
		{
			continue;
		}
		
		do
		{
			if(-1==mount(mount_src.c_str(), mount_dst.c_str(), support_fs[fori], mount_flag, NULL))
			{
			}
			else
			{
				bMount=true;
			}
			
		}while(0);
		
		if(bMount)
		{
			m_parts.push_back(part);

			mount_ok=true;
			
			DPRINTF("insert part:%s %d\n", part->dev_name.c_str(), part->type);
			
			break;
		}		
	}
	
	part_lockoff();

	if(!mount_ok)
	{
		rmdir(mount_dst.c_str());
	}

	return true;
}

void Hsudev::delPartition(const string name)
{
	DPRINTF("delete partion %s\n", name.c_str());
	
	string mount_dst("/mnt/");	

	bool bremove=false;
	
	part_lockon();
	for(PART_LIST::iterator it=m_parts.begin(); it!=m_parts.end(); it++)
	{		
		udevice* dev=*it;

		if(dev->dev_name==name)
		{			
			m_parts.erase(it);			
			mount_dst.append(dev->dev_name);
			bremove=true;
			
			delete dev;
			break;
		}		
	}
	part_lockoff();
	
	if(bremove)
	{
		umount(mount_dst.c_str());
		rmdir(mount_dst.c_str());
		
		DPRINTF("remove part:%s ok\n", mount_dst.c_str());
	}
}



void* Hsudev::procDevice(void* arg)
{	
	Hsudev* udev=(Hsudev*)arg;
	
	sig_unblock();

	int udsock = init_hotplug_sock();
	
	while(1)
	{	
		const char* action_add="add@";
		const char* action_del="remove@";
		const string parttype="DEVTYPE=partition";
		const string disktype="DEVTYPE=disk";
		const string uname="DEVNAME=";
		const string upath="DEVPATH=";
		const string umajor="MAJOR=";
		const string uminor="MINOR=";
			
		char buf[UEVENT_BUFFER_SIZE * 2];
		memset(buf, 0, sizeof(buf));

		int ret;
#if 0
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(udsock, &fds);

		struct timeval tv;
		tv.tv_sec=0;
		tv.tv_usec=udev->getDelayTime();
		
		ret=select(udsock+1, &fds, NULL, NULL, &tv);
		if(ret<=0)
		{
			continue;
		}
#endif

		ret=recv(udsock, &buf, sizeof(buf), 0);
		if(ret<=0)
		{
			if(errno==EINTR)
			{
				break;
			}

			JPRINTF(__FILE__, __LINE__, "recv error:%m\n");
			
			continue;
		}

		for(int i=0; i<ret; i++)
		{
			if(buf[i]==0)
			{
				buf[i]='\n';
			}
		}

		string wholeBuf(buf);
		//DPRINTF("%s\n", wholeBuf.c_str());
		
		stringstream ss(string((char*)buf, ret));

		string devname;
		unsigned int major=0,minor=0;
		if(memcmp(buf, action_add, strlen(action_add))==0)
		{	
			int devtype=0;
			bool is_partition=false;
			
			while(!ss.eof())
			{
				string aline;
				getline(ss, aline);

				if(aline==parttype)
				{					
					is_partition=true;
				}
				else if(aline.compare(0, uname.size(), uname)==0)
				{				
					devname.insert(0, aline, uname.size(), aline.size()-uname.size());
				}
				else if(aline.compare(0, umajor.size(), umajor)==0)
				{
					errno=0;
					char* endptr;
					int val=strtol(aline.c_str()+umajor.size(), &endptr, 10);
					if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
					{
						DPRINTF("find dev major error:%m\n");
					}
					else if (endptr == aline.c_str())
					{
						DPRINTF("No digits were found\n");
					}
					else
					{
						major=val;
					}

					DPRINTF("major:%d     line:%s\n", major, aline.c_str());
				}
				else if(aline.compare(0, uminor.size(), uminor)==0)
				{				
					errno=0;
					char* endptr;
					int val=strtol(aline.c_str()+uminor.size(), &endptr, 10);
					if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
					{
						DPRINTF("find dev minor error:%m\n");
					}
					else if (endptr == aline.c_str())
					{
						DPRINTF("No digits were found\n");
					}
					else
					{
						minor=val;
					}

					DPRINTF("minor:%d     line:%s\n", minor, aline.c_str());
				}
				else if(aline.compare(0, upath.size(), upath)==0)
				{
					if(aline.find("usb")!=string::npos)
					{						
						devtype=PART_TYPE_USB;						
					}
				}
			}

			if(is_partition && (devtype==PART_TYPE_USB))
			{
				udevice* mdev=new udevice;
				mdev->dev_name=devname;
				mdev->type=devtype;
				mdev->major=major;
				mdev->minor=minor;
				udev->addPartition(mdev);
			}			
			
		}
		else if(memcmp(buf, action_del, strlen(action_del))==0)
		{
			bool is_partition=false;			
			while(!ss.eof())
			{
				string aline;
				getline(ss, aline);
				if(aline==parttype)
				{					
					is_partition=true;
				}
				else if(aline.compare(0, uname.size(), uname)==0)
				{				
					devname.insert(0, aline, uname.size(), aline.size()-uname.size());
				}
			}
			
			if(is_partition)
			{
				udev->delPartition(devname);
			}			
		}


	}
	return NULL;
}


Hsudev* Hsudev::getInstance()
{
	static Hsudev* udev=NULL;
	if(NULL==udev)
	{
		udev=new Hsudev;
	}
	
	return udev;
}

void Hsudev::destroy()
{	
	Hsudev* udev=Hsudev::getInstance();
	delete udev;
}


Hsudev::~Hsudev()
{	
	DPRINTF("destruct \n");
	
	sig_notify_thread(m_pid);
	
	pthread_join(m_pid, NULL);

	part_lockon();
	for(PART_LIST::iterator it=m_parts.begin(); it!=m_parts.end(); it++)
	{		
		udevice* dev=*it;
		
		string mount_dst("/mnt/");
		
		mount_dst.append(dev->dev_name);
		
		errno=0;
		if(umount(mount_dst.c_str()))
		{
			DPRINTF("remove part:%s  err:%m\n", mount_dst.c_str());
#ifdef DEBUG			
			char cmd[256];
			sprintf(cmd, "ls -l /proc/%d/fd/", getpid());
			system(cmd);			
#endif			
		}
		else
		{
			DPRINTF("remove part:%s  ok\n", mount_dst.c_str());
		}			
		
		delete dev;		
				
	}
	m_parts.clear();
	
	part_lockoff();
	
	pthread_mutex_destroy(&m_part_lock);

	DPRINTF("destruct ok\n");
}


void Hsudev::part_lockon()
{
	pthread_mutex_lock(&m_part_lock);
}
void Hsudev::part_lockoff()
{
	pthread_mutex_unlock(&m_part_lock);
}



void Hsudev::checkDiskUevent(const string& fullname, const list<dev_t>& procPts)
{	
	const string parttype="DEVTYPE=partition";
	const string disktype="DEVTYPE=disk";
	const string uname="DEVNAME=";
	//const string upath="DEVPATH=";	
	const string umajor="MAJOR=";
	const string uminor="MINOR=";

	string uevent;
	fstream filestr(fullname.c_str(), fstream::in);
	
	string devname; 	
	int devtype=0;
	bool is_partition=false;
	unsigned int major=0,minor=0;
	
	if(filestr.is_open ( )==false)
	{
		return;
	}
	
	while(!filestr.eof())
	{
		string aline;
		filestr>>aline;

		if(aline==parttype)
		{					
			is_partition=true;
		}
		else if(aline.compare(0, umajor.size(), umajor)==0)
		{
			errno=0;
			char* endptr;
			int val=strtol(aline.c_str()+umajor.size(), &endptr, 10);
			if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
			{
				DPRINTF("find dev major error:%m\n");
			}
			else if (endptr == aline.c_str())
			{
				DPRINTF("No digits were found\n");
			}
			else
			{
				major=val;
			}
		}
		else if(aline.compare(0, uminor.size(), uminor)==0)
		{				
			errno=0;
			char* endptr;
			int val=strtol(aline.c_str()+uminor.size(), &endptr, 10);
			if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
			{
				DPRINTF("find dev minor error:%m\n");
			}
			else if (endptr == aline.c_str())
			{
				DPRINTF("No digits were found\n");
			}
			else
			{
				minor=val;
			}
		}
		else if(aline.compare(0, uname.size(), uname)==0)
		{				
			devname.insert(0, aline, uname.size(), aline.size()-uname.size());
		}
	}

	do
	{
		dev_t tmpDev=makedev(major, minor);
		bool bFind=false;

		for(list<dev_t>::const_iterator it=procPts.begin(); it!=procPts.end(); it++)
		{
			if(tmpDev==(*it))
			{
				bFind=true;
				break;
			}
		}

		if(false==bFind)
		{
			break;
		}
		
		if(fullname.find("usb")!=string::npos)
		{		
			devtype=PART_TYPE_USB;			
		}
		else
		{
			break;
		}		
			
		if(is_partition && (devtype=PART_TYPE_USB))
		{
			udevice* mdev=new udevice;
			mdev->dev_name=devname;
			mdev->type=devtype;
			mdev->major=major;
			mdev->minor=minor;
			addPartition(mdev);
		}
	}while(0);

	filestr.close();
	
}


void Hsudev::searchStaticDisk(const string& path, const list<dev_t>& procPts)
{
	struct dirent **namelist;
	int n;

	n = scandir(path.c_str(), &namelist, NULL, alphasort);
	if (n < 0)
	{
		DPRINTF("scan dir err %m\n");
	}
	else 
	{
		while (n--)
		{			
			const char* dname=namelist[n]->d_name;
			int dtype=namelist[n]->d_type;
			if(dname[0]!='.')
			{
				//printf("%s\n", namelist[n]->d_name);
				if(dtype==DT_UNKNOWN)
				{
					string tmppath=path;
					tmppath+="/";
					tmppath+=dname;
					struct stat st;
					int ret=stat(tmppath.c_str(), &st);
					if(ret<0)
					{
						if(S_ISREG(st.st_mode))
						{								
							dtype=DT_REG;
						}
						else if(S_ISDIR(st.st_mode))
						{
							dtype=DT_DIR;
						}
					}
				}
				
				if(dtype==DT_DIR)
				{
					string npath;
					npath=path+"/"+dname;
					searchStaticDisk(npath, procPts);
				}
				else if(dtype==DT_REG)
				{
					if(strcmp(dname, "uevent")==0)
					{						
						string fullname(path+"/"+dname);					

						checkDiskUevent(fullname, procPts);
					}
				}
				
			}
			free(namelist[n]);
		}
		
		free(namelist);
	}

}


void Hsudev::tryprocpt(list<dev_t>& procPts) 
{
#define _PATH_PROC_PARTITIONS	"/proc/partitions"
	FILE *procpt;
	char line[128], ptname[128];
	int ma, mi;
	unsigned long long sz;

	procpt = fopen(_PATH_PROC_PARTITIONS, "r");
	if (procpt == NULL) {
		DPRINTF("cannot open %s\n", _PATH_PROC_PARTITIONS);
		return;
	}

	while (fgets(line, sizeof(line), procpt))
	{
		if (sscanf (line, " %d %d %llu %128[^\n ]", &ma, &mi, &sz, ptname) != 4)
			continue;
		dev_t dvt=makedev(ma,mi);
		procPts.push_back(dvt);
	}
	fclose(procpt);
}



bool Hsudev::startFilter()
{	
	list<dev_t> procPts;

	tryprocpt(procPts);
	
	searchStaticDisk("/sys", procPts);

	pthread_create(&m_pid, NULL, procDevice, this);
	
	return true;
}

#if 0
bool Hsudev::checkUsb()
{
	part_lockon();
	if(m_parts.size())
	{
		part_lockoff();
		return true;
	}
	part_lockoff();
	return false;
}

int Hsudev::export_db()
{
	int ret=0;
	
	MainRecord* mrd=MainRecord::getInstance();
	
	errno=0;
	
	int fd=-1;
	string dstfile="/mnt/";
	part_lockon();
	if(m_parts.size())
	{
		udevice* udev=m_parts.front();
		dstfile+=udev->dev_name;
		dstfile+="/export.txt";
		fd=open(dstfile.c_str(), O_RDWR|O_CREAT, 0666);
		if(-1==fd)
		{
			ret=errno;
		}
	}
	else
	{
		ret=-1;
	}
	part_lockoff();

	if(ret)
	{
		return ret;
	}

	ret=mrd->saveToFile(fd);

	close(fd);

	return ret;
}

#else

bool find_mount_dst(char* buf, const char* mt)
{
	const char* pdst=NULL;
	while(buf && *buf)
	{
		if((*buf)==' ')
		{
			if(NULL==pdst)
			{
				pdst=buf+1;
			}
			else
			{
				(*buf)=0;
				if(strcmp(pdst, mt)==0)
				{
					return true;
				}
			}			
		}
		buf++;
	}

	return false;
}

#define DST_POINT "/udisk"

bool Hsudev::checkUsb()
{
#if 0
	int fd=open("/proc/mounts", O_RDONLY);
	if(-1==fd)
	{
		DPRINTF("can not open /proc/mounts %m\n");
		return false;
	}


	char buf[2*1024];

	int ret=read(fd, buf, sizeof(buf));
	if(ret<0)
	{
		DPRINTF("/proc/mounts read err %m\n");
		close(fd);
		return false;
	}
	else if(ret==0)
	{
		DPRINTF("/proc/mounts have no data\n");
		close(fd);
		return false;
	}

	buf[ret]=0;

	close(fd);


	stringstream ss(string((char*)buf, ret));	
	string devname;		
	while(!ss.eof())
	{
		string aline;
		getline(ss, aline);

		char str_line[aline.size()+1];
		strcpy(str_line, aline.c_str());

		if(find_mount_dst(str_line, "/udisk"))
		{
			return true;
		}
	}

	return false;
#else
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("/proc/mounts", "r");
	if (fp == NULL)
	{
		return false;
	}

	bool bret=false;
	while ((read = getline(&line, &len, fp)) != -1)
	{
		if(find_mount_dst(line, DST_POINT))
		{
			bret=true;
			break;
		}
	}
	
	free(line);
	fclose(fp);

	return bret;
#endif
}


int Hsudev::getUsbExportFd()
{
	int fd=-1;
	const char* dstfile=DST_POINT"/export.txt";

	fd=open(dstfile, O_RDWR|O_CREAT);

	return fd;
}

#endif



