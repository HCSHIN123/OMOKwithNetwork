#pragma once
class RecvBufferHelper
{
private:
	//데이터 버퍼
	Byte* m_pBuffer;
	//총 크기
	unsigned int m_iSize;
	//현재 커서 위치
	unsigned int m_iCurIndex;
public:
	RecvBufferHelper();
	RecvBufferHelper(Byte* _buffer,unsigned int _size,unsigned int _index = 0);
	~RecvBufferHelper();

	Byte* GetBuffer() { return m_pBuffer; }
	unsigned int GetSize() { return m_iSize; }
	unsigned int GetCurIndex() { return m_iCurIndex; }
	unsigned int GetUseableSize() { return m_iSize - m_iCurIndex; }

	bool Peek(void* _destination,unsigned int _length);
	template<typename type>
	bool Peek(type* _destination) { return Peek(_destination, sizeof(type)); }

	bool Read(void* _destination,unsigned int _length);
	template<typename type>
	bool Read(type* _destination) { return Read(_destination, sizeof(type)); }

	template<typename type>
	RecvBufferHelper& operator>> (type& _destination)
	{
		_destination = *reinterpret_cast<type*>(&m_pBuffer[m_iCurIndex]);
		m_iCurIndex += sizeof(type);
		return *this;
	}
};

