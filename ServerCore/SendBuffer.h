#pragma once
typedef unsigned char Byte;
class SendBufferPool;
class SendBuffer : std::enable_shared_from_this<SendBuffer>
{
private:
	std::shared_ptr<SendBufferPool> m_Owner;
	Byte* m_Buffer;
	unsigned int m_iAllocSize;
	unsigned int m_iUsedSize;
public:
	SendBuffer(std::shared_ptr<SendBufferPool> _owner,Byte* _buff, int _bufferSize);
	~SendBuffer();

	Byte* GetBuff() { return m_Buffer; }
	int GetSize() { return m_iUsedSize; }

	void UseBuff(unsigned int _size);

};

