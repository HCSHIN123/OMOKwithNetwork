#include "pch.h"
#include "SendBufferHelper.h"

SendBufferHelper::SendBufferHelper()
{
	m_pBuffer = nullptr;
	m_iCurIndex = 0;
	m_iSize = 0;
}

SendBufferHelper::SendBufferHelper(Byte* _buffer, unsigned int _size, unsigned int _index)
{
	m_pBuffer = _buffer;
	m_iCurIndex = _index;
	m_iSize = _size;
}

SendBufferHelper::~SendBufferHelper()
{
}

bool SendBufferHelper::Write(void* _source, unsigned int _length)
{
	if(GetUseableSize() < _length)
		return false;
	memcpy(&m_pBuffer[m_iCurIndex], _source, _length);
	m_iCurIndex += _length;
	return true;
}