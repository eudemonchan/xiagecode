class SR_Buf
{
private:
	char *bufData_;             //bufָ��
	unsigned int bufLength_;    //buf����

public:
	UINT UserDefineData;
	SR_Buf(void)
	{
		UserDefineData = 0;
		bufLength_ = 0;
		bufData_ = NULL;
	}

	//�������캯��
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
		//�ͷ�buf
		if ( bufData_ != NULL )
		{
			delete []bufData_;
		}
	}

	//��ȡbuf
	char* GetBuffer()
	{
		return bufData_;
	}

	//�����ڴ�
	BOOL AllocBuffer( unsigned int length )
	{
		//����֮ǰ���ͷžɵ�
		ReleaseBuffer();
		bufData_ = new char[length];
		if ( bufData_ == NULL )
		{
			return FALSE;
		}
		else
		{
			memset( bufData_, 0, length );  //���ݳ�ʼ��Ϊ0
			bufLength_ = length;            //���泤�� 
			return TRUE;
		}
	}

	//��ȡbuf�Ĵ�С
	unsigned int GetBufSize()
	{
		return bufLength_;
	}

	//��ȡANSI�ַ����ĳ���
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

	//��ȡUnicode�ַ����ĳ���
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

	//��ȡbuf�ĵ�ַ����Ҫ������ָ��ʱʹ��
	char** GetBufferAddress()
	{
		return &bufData_;
	}

	//�ֶ��ͷ�Buffer
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