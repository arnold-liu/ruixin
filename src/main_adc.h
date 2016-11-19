#ifndef MAIN_ADC_H
#define MAIN_ADC_H


class MainAdc
{
public:
	static MainAdc* getInstance();
	virtual ~MainAdc();

	bool getAdVal(int& val);
	
private:
	MainAdc();

private:
	int m_fd;
};











#endif
