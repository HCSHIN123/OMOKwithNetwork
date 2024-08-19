#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int _bufferSize)
{
	m_iBufferSize = _bufferSize;
	m_iCapacity = _bufferSize * BUFFER_COUNT;
	m_Buffer.resize(m_iCapacity);
	m_iReadIndex = 0;
	m_iWriteIndex = 0;
}

RecvBuffer::~RecvBuffer()
{
}
//WriteIndex와 ReadIndex를 0으로 초기화
void RecvBuffer::Clean()
{
	int DataSize = GetDataSize();
	if (DataSize == 0)//WriteIndex와 ReadIndex가 같다면 전송받은 버퍼의 모든 메모리 사용완료
	{
		m_iReadIndex = 0;
		m_iWriteIndex = 0;
	}
	else//전송받은 버퍼의 메모리중 일부가 남음
	{
		if (GetFreeSize() < m_iBufferSize)//남은 메모리가 최대 메모리보다 적게 남았을 경우
		{
			//읽지 못한 남은 메모리만큼 앞으로 이동시킨 뒤
			memcpy(&m_Buffer[0], &m_Buffer[m_iReadIndex], DataSize);
			//Index 초기화
			m_iReadIndex = 0;
			m_iWriteIndex = DataSize;
		}
	}
}

bool RecvBuffer::Read(int Bytes)
{
	//읽은 메모리량이 남은 메모리량 보다 많을경우 비정상적인 상황
	if (Bytes > GetDataSize())
		return false;
	//읽은메모리량만큼 Index 증가 
	m_iReadIndex += Bytes;

	return true;
}

bool RecvBuffer::Write(int Bytes)
{
	//저장 메모리량이 최대 저장가는 메모리량보다 클경우 비정상적인 상황
	if (Bytes > GetFreeSize())
		return false;
	//저장한 메모리량만큼 Index 증가
	m_iWriteIndex += Bytes;

	return true;
}
