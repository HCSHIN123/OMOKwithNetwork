#pragma once
#include"SendBuffer.h"
class SendBufferPool : public std::enable_shared_from_this<SendBufferPool>
{
	enum { SEND_BUFFER_SIZE = 0x1000, };
private:
	bool m_bUse;
	unsigned int m_iUsedSize;
	Byte m_Buffer[SEND_BUFFER_SIZE];
public:
	SendBufferPool();
	~SendBufferPool();
	void Init();
	std::shared_ptr<SendBuffer> GetBuffer(unsigned int _size);
	void Used(unsigned int _size);
	bool Useable() { return m_bUse == false; }
	unsigned int GetFreeSize() { return static_cast<unsigned int>(SEND_BUFFER_SIZE) - m_iUsedSize; }
};

