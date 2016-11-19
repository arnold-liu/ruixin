#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>

#include "main_adc.h"
#include "gbl_define.h"




MainAdc* MainAdc::getInstance()
{
	static MainAdc ma;
	return &ma;
}

MainAdc::~MainAdc()
{
	if(m_fd>0)
	{
		close(m_fd);
		m_fd=-1;
	}
}

bool MainAdc::getAdVal(int& val)
{
	bool bret=false;
	char buffer[30];
	int len = read(m_fd, buffer, sizeof(buffer) -1);
	if (len > 0) 
	{
		buffer[len] = '\0';
		int value = -1;
		if(1==sscanf(buffer, "%d", &value))
		{
			val=value;
			bret=true;
		}
	}

	return bret;
}

MainAdc::MainAdc()
{
	m_fd = open("/dev/adc", O_RDONLY);
	if (m_fd < 0) {
		JPRINTF(__FILE__, __LINE__, "open ADC device err\n");
	}
}