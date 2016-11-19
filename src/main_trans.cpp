#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#include <cstdlib>  
#include <cerrno>  
#include <cassert>  
#include <climits>

#include "main_trans.h"




  
IconvPair::IconvPair(const char* toCode, const char* fromCode)
    : mToCode(toCode)  
    , mFromCode(fromCode)  
    , mIconv(reinterpret_cast<iconv_t>(-1))  
    , mBuffer(NULL)  
    , mBufferLen(0)  
    , mContentLen(0)  
{
	if(NULL==toCode || NULL==fromCode)
	{
		return;
	}
	
	mBufferLen = INIT_BUFFER;  
	mBuffer = reinterpret_cast<char*>(malloc(mBufferLen));  
	if (NULL == mBuffer)  
	{  
		return;
	}  

	mIconv = iconv_open(toCode, fromCode);  
	if (reinterpret_cast<iconv_t>(-1) == mIconv)  
	{
	printf("--------[%s:%d]  %m\n", __FILE__, __LINE__);

		return ;
	}  
}  
  
IconvPair::~IconvPair()  
{  
	if (reinterpret_cast<iconv_t>(-1) != mIconv)  
	{  
		iconv_close(mIconv);  
	}  

	if(mBuffer)
	{
		free(mBuffer);
	}
}  


void showString(const char* inbuf)
{
	const char* buf=inbuf;
	printf("\n%p  start\n", inbuf);
	
	while(*buf)
	{
		printf("%02x ", *buf);
		buf++;
	}

	printf("\n%p  end\n", inbuf);
}

size_t IconvPair::convert(const char *inBuffer, size_t inBytesLeft)  
{
printf("--------[%s:%d] \n", __FILE__, __LINE__);

	if(reinterpret_cast<iconv_t>(-1) == mIconv)
	{
		return -1;
	}
printf("--------[%s:%d] \n", __FILE__, __LINE__);


	char* tmpInBuf=(char*)malloc(inBytesLeft);
	if(NULL==tmpInBuf)
	{
		return -1;
	}
	char * freeBuf=tmpInBuf;
	memcpy(tmpInBuf, inBuffer, inBytesLeft);
  
	iconv(mIconv, NULL, NULL, NULL, NULL);

showString(inBuffer);

	char *outBuffer = mBuffer;  
	size_t outBytesLeft = mBufferLen;  
	size_t ret = iconv(mIconv, &tmpInBuf, &inBytesLeft, &outBuffer, &outBytesLeft);

printf("--------%d  %d \n", ret, outBytesLeft);
showString(outBuffer);

	while ((UINT_MAX == ret) && (0 == outBytesLeft)/*(E2BIG == errno)*/)  
	{  
		size_t oldBufferLen = mBufferLen;  
		if(!incBuffer())
		{
			free(freeBuf);
			return -1;
		}

		outBuffer = mBuffer + oldBufferLen;  
		outBytesLeft = mBufferLen - oldBufferLen;  
		ret = iconv(mIconv, &tmpInBuf, &inBytesLeft, &outBuffer, &outBytesLeft);  
	}  

	mContentLen = mBufferLen - outBytesLeft;
	if (0 == outBytesLeft)  
	{  
		if(!incBuffer())
		{
			free(freeBuf);
			return -1;
		}
	}  
	mBuffer[mContentLen] = 0;

	free(freeBuf);
	return ret;  
}  
  
bool IconvPair::incBuffer()  
{  
	mBufferLen *= 2;  
	mBuffer = reinterpret_cast<char*>(realloc(mBuffer, mBufferLen));  
	if (NULL == mBuffer)  
	{  
		return false;
	}

	return true;
} 












UTF8_to_GBK* UTF8_to_GBK::getInstance()
{
	static UTF8_to_GBK utg;
	return &utg;
}

UTF8_to_GBK::~UTF8_to_GBK()
{
}

#if 0
bool UTF8_to_GBK::trans(string& in, string& out)
{
	if(m_cd==(iconv_t) -1)
	{
		printf("iconv not open\n");
		return false;
	}

	size_t ret=utf8ToGbk(in.c_str(), in.size(), out.c_str(), out.size())
	if(-1==ret)
	{
		return false;
	}

	return true;
}
#endif

UTF8_to_GBK::UTF8_to_GBK():IconvPair("GBK", "UTF-8")
{
#if 0
	m_cd=iconv_open("GBK", "UTF-8");
	if(m_cd==(iconv_t) -1)
	{
		printf("can not open iconv %m\n");
	}
#endif	
}

