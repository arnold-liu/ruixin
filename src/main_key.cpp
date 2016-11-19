
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


#include "gbl_define.h"

#include "main_key.h"





MainKey* MainKey::getInstance()
{
	static MainKey mk;
	return &mk;
}

MainKey::~MainKey()
{
	if(-1!=m_fd)
	{
		close(m_fd);
	}
}

void MainKey::setCurWin(HWND win)
{
	m_win=win;
}



MainKey::MainKey()
{
	m_win=0;

	m_fd=open("/dev/buttons", O_RDONLY);
	if(-1==m_fd)
	{
		JPRINTF(__FILE__, __LINE__, "can not open buttons %m\n");
		return ;
	}

	pthread_create(&m_pid, NULL, proc, this);
}


void MainKey::doproc()
{
#define KEY_CNT 5
	char last_button[KEY_CNT];

	memset(last_button, '0', KEY_CNT);
	
	while(1)
	{
		char button[KEY_CNT];
		int ret=read(m_fd, button, sizeof(button));
		if(ret<=0)
		{
			usleep(500);
			continue;
		}

//printf("%d [%02x] [%02x] [%02x] [%02x] [%02x]\n", ret, button[0], button[1], button[2], button[3], button[4]);

		for(int fori=0; fori<ret; fori++)
		{
			if(button[fori]!=last_button[fori])
			{
				if(m_win!=0)
				{
					PostMessage(m_win, MSG_USER_KEY, fori, button[fori]-'0');
				}
				last_button[fori]=button[fori];
			}
		}

		
	}
}


void* MainKey::proc(void* arg)
{
	MainKey* mk=(MainKey*)arg;
	
	mk->doproc();

	return NULL;
}

