#ifndef MAIN_KEY_H
#define MAIN_KEY_H


#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <pthread.h>


#define MSG_USER_KEY (MSG_USER+88)



class MainKey
{
private:
	HWND m_win;
	int m_fd;
	pthread_t m_pid;
public:
	static MainKey* getInstance();
	virtual ~MainKey();

	void setCurWin(HWND win);

private:
	MainKey();
	static void* proc(void* arg);	
	void doproc();
};
















#endif
