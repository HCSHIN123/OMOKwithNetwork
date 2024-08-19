#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(std::shared_ptr<SendBufferPool> _owner, Byte* _buff, int _bufferSize)
	: m_iAllocSize(_bufferSize)
{
	m_Owner = _owner;
	m_Buffer = _buff;
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::UseBuff(unsigned int _size)
{
	assert(m_iAllocSize >= _size);
	m_iUsedSize = _size;
	m_Owner->Used(_size);
}
