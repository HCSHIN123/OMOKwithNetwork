#pragma once
class SendBufferHelper
{
private:
	//데이터 버퍼
	Byte* m_pBuffer;
	//총 크기
	unsigned int m_iSize;
	//현재 커서 위치
	unsigned int m_iCurIndex;
public:
	SendBufferHelper();
	SendBufferHelper(Byte* _buffer, unsigned int _size, unsigned int _index = 0);
	~SendBufferHelper();

	Byte* GetBuffer() { return m_pBuffer; }
	unsigned int GetSize() { return m_iSize; }
	unsigned int GetCurIndex() { return m_iCurIndex; }
	unsigned int GetUseableSize() { return m_iSize - m_iCurIndex; }

	bool Write(void* _source, unsigned int _length);
	template<typename type>
	bool Write(type* _source) { return Write(_source, sizeof(type)); }

	template<typename type>
	SendBufferHelper& operator<< (const type& _source)
	{
		*reinterpret_cast<type*>(&m_pBuffer[m_iCurIndex]) = _source;
		m_iCurIndex += sizeof(type);

		return *this;
	}
	template<typename type>
	SendBufferHelper& operator<< (const type&& _source)
	{
		*reinterpret_cast<type*>(&m_pBuffer[m_iCurIndex]) = std::move(_source);
		m_iCurIndex += sizeof(type);

		return *this;
	}

	template<typename type>
	type* Reserve()
	{
		if (GetUseableSize() < sizeof(type))
			return nullptr;

		type* ret = reinterpret_cast<type*>(&m_pBuffer[m_iCurIndex]);
		m_iCurIndex += sizeof(type);
		return ret;
	}
};

