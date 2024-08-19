#include "pch.h"
#include "RecvBufferHelper.h"

RecvBufferHelper::RecvBufferHelper()
{
	m_pBuffer = nullptr;
	m_iCurIndex = 0;
	m_iSize = 0;
}

RecvBufferHelper::RecvBufferHelper(Byte* _buffer, unsigned int _size, unsigned int _index)
{
	m_pBuffer = _buffer;
	m_iCurIndex = _index;
	m_iSize = _size;
}

RecvBufferHelper::~RecvBufferHelper()
{
}

bool RecvBufferHelper::Peek(void* _destination, unsigned int _length)
{
	//_length 만큼 읽을 수 있는 데이터가 있는지 확인
	if (GetUseableSize() < _length)
		return false;
	//데이터를 destination에 복사
	memcpy(_destination, &m_pBuffer[m_iCurIndex], _length);
	return true;
}

bool RecvBufferHelper::Read(void* _destination, unsigned int _length)
{
	if (Peek(_destination, _length) == false)
		return false;
	//데이터를 사용처리 하여 index를 읽은만큼 뒤로 이동
	m_iCurIndex += _length;
	return true;
}
