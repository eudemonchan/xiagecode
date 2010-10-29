class SR_Buf
{
private:
	char *bufData_;             //buf指针
	unsigned int bufLength_;    //buf长度

public:
	UINT UserDefineData;
	SR_Buf(void)
	{
		UserDefineData = 0;
		bufLength_ = 0;
		bufData_ = NULL;
	}

	//拷贝构造函数
	SR_Buf(const SR_Buf& srBuf)
	{
		UserDefineData = srBuf.UserDefineData;
		bufLength_ = srBuf.bufLength_;
		if ( bufLength_ != 0 )
		{
			bufData_ = new char[bufLength_];
			memcpy( bufData_, srBuf.bufData_, bufLength_ );
		}
		else
		{
			bufData_ = NULL;
		}
	}

	SR_Buf & operator = (const SR_Buf &srBuf)
	{
		UserDefineData = srBuf.UserDefineData;
		bufLength_ = srBuf.bufLength_;
		char *pTemp = NULL;
		if ( bufLength_ != 0 )
		{
			pTemp = new char[bufLength_];
			memcpy( pTemp, srBuf.bufData_, bufLength_ );
			if ( bufData_ != NULL )
			{
				delete []bufData_;
			}
			bufData_ = pTemp;
		}
		else
		{
			if ( bufData_ != NULL )
			{
				delete []bufData_;
			}
			bufData_ = NULL;
		}
		return *this;
	}

	~SR_Buf(void)
	{
		//释放buf
		if ( bufData_ != NULL )
		{
			delete []bufData_;
		}
	}

	//获取buf
	char* GetBuffer()
	{
		return bufData_;
	}

	//分配内存
	BOOL AllocBuffer( unsigned int length )
	{
		//分配之前先释放旧的
		ReleaseBuffer();
		bufData_ = new char[length];
		if ( bufData_ == NULL )
		{
			return FALSE;
		}
		else
		{
			memset( bufData_, 0, length );  //数据初始化为0
			bufLength_ = length;            //保存长度 
			return TRUE;
		}
	}

	//获取buf的大小
	unsigned int GetBufSize()
	{
		return bufLength_;
	}

	//获取ANSI字符串的长度
	int GetMutiCharLength()
	{
		int length = strlen( bufData_ );
		if ( length < 0 || (unsigned int)length > bufLength_ )
		{
			return -1;
		}
		else
		{
			return length;
		}
	}

	//获取Unicode字符串的长度
	int GetWideCharLength()
	{
		int length = wcslen( (wchar_t*)bufData_ );
		if ( length < 0 || (unsigned int)length > bufLength_ )
		{
			return -1;
		}
		else
		{
			return length;
		}
	}

	//获取buf的地址，需要传二级指针时使用
	char** GetBufferAddress()
	{
		return &bufData_;
	}

	//手动释放Buffer
	void ReleaseBuffer()
	{
		if ( bufData_ != NULL )
		{
			delete []bufData_;
			bufData_ = NULL;
			bufLength_ = 0;
		}
	}
};