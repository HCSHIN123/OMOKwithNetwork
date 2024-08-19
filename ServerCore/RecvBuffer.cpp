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
//WriteIndex�� ReadIndex�� 0���� �ʱ�ȭ
void RecvBuffer::Clean()
{
	int DataSize = GetDataSize();
	if (DataSize == 0)//WriteIndex�� ReadIndex�� ���ٸ� ���۹��� ������ ��� �޸� ���Ϸ�
	{
		m_iReadIndex = 0;
		m_iWriteIndex = 0;
	}
	else//���۹��� ������ �޸��� �Ϻΰ� ����
	{
		if (GetFreeSize() < m_iBufferSize)//���� �޸𸮰� �ִ� �޸𸮺��� ���� ������ ���
		{
			//���� ���� ���� �޸𸮸�ŭ ������ �̵���Ų ��
			memcpy(&m_Buffer[0], &m_Buffer[m_iReadIndex], DataSize);
			//Index �ʱ�ȭ
			m_iReadIndex = 0;
			m_iWriteIndex = DataSize;
		}
	}
}

bool RecvBuffer::Read(int Bytes)
{
	//���� �޸𸮷��� ���� �޸𸮷� ���� ������� ���������� ��Ȳ
	if (Bytes > GetDataSize())
		return false;
	//�����޸𸮷���ŭ Index ���� 
	m_iReadIndex += Bytes;

	return true;
}

bool RecvBuffer::Write(int Bytes)
{
	//���� �޸𸮷��� �ִ� ���尡�� �޸𸮷����� Ŭ��� ���������� ��Ȳ
	if (Bytes > GetFreeSize())
		return false;
	//������ �޸𸮷���ŭ Index ����
	m_iWriteIndex += Bytes;

	return true;
}
