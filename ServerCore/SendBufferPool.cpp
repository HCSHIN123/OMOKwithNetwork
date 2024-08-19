#include "pch.h"
#include "SendBufferPool.h"

SendBufferPool::SendBufferPool()
	: m_Buffer{},m_bUse(false), m_iUsedSize(0)
{
}

SendBufferPool::~SendBufferPool()
{
}

void SendBufferPool::Init()
{
	//사용가능상태 전환
	m_bUse = false;
	m_iUsedSize = 0;
}

std::shared_ptr<SendBuffer> SendBufferPool::GetBuffer(unsigned int _size)
{
	assert(_size <= SEND_BUFFER_SIZE);
	assert(m_bUse == false);

	if (_size > GetFreeSize())
		return nullptr;
	m_bUse = true;
	return std::make_shared<SendBuffer>(shared_from_this(), &m_Buffer[m_iUsedSize], _size);
}

void SendBufferPool::Used(unsigned int _size)
{
	assert(m_bUse);
	m_bUse = false;
	m_iUsedSize += _size;
}
