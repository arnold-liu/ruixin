#ifndef MAIN_TRANS_H
#define MAIN_TRANS_H

#include <iconv.h>
#include <string>

using std::string;


void showString(const char* inbuf);


class IconvPair
{
	enum   
	{  
		INIT_BUFFER = 4096  
	};  
  
public:  
	IconvPair(const char* toCode, const char* fromCode);
	virtual ~IconvPair();  
  
private:  
	IconvPair(const IconvPair&);  
	IconvPair& operator=(const IconvPair&);  
  
public:
	friend bool operator<(const IconvPair &lhs, const IconvPair &rhs)  
	{  
		if (lhs.mToCode < rhs.mToCode)  
		{  
			return true;  
		}  
		else if (lhs.mToCode > rhs.mToCode)  
		{  
			return false;  
		}  

		return lhs.mFromCode <=rhs.mFromCode ? true : false;  
	}  

	const char* buffer() const {return mBuffer;}  
	size_t bufferLen() const {return mBufferLen;}  
	size_t contentLen() const {return mContentLen;}  

	size_t convert(const char *inBuffer, size_t inBytesLeft);
  
private:  
	bool incBuffer();
  
private:
	string mToCode;  
	string mFromCode; 
	iconv_t mIconv;  
	char *mBuffer;  
	size_t mBufferLen;  
	size_t mContentLen; 
};







class UTF8_to_GBK:public IconvPair
{
public:
	static UTF8_to_GBK* getInstance();
	virtual ~UTF8_to_GBK();


private:
	UTF8_to_GBK();
};












#endif
