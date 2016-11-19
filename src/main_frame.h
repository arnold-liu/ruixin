#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <minigui/common.h>


int quitThisDialog(HWND hDlg, const int retVal);
bool TestWindowCreate(HWND hOwner);



class MainFrame
{
private:
	static unsigned int m_w;
	static unsigned int m_h;

public:
	MainFrame();
	virtual ~MainFrame();	

	int create(unsigned int w, unsigned int h);


public:
	static unsigned int getWidth();
	static unsigned int getHeight();
	

};


















#endif //MAIN_FRAME_H
