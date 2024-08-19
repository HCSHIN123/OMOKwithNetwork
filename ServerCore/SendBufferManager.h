#pragma once
#include "Lock.h"
#include "SendBufferPool.h"
class SendBufferManager
{
private:
	Lock m_Lock;
	std::vector<std::shared_ptr<SendBufferPool>> m_SendBufferPools;
public:
	std::shared_ptr<SendBuffer> GetSendBuffer(unsigned int _size);
	std::shared_ptr<SendBufferPool> Pop();
	void Push(std::shared_ptr<SendBufferPool> _buff);
	static void PushGlobal(SendBufferPool* _buff);
};

