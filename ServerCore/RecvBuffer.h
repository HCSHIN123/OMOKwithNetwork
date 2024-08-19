#pragma once
class RecvBuffer
{
	enum {BUFFER_COUNT = 10}; //���� ��밡�� ���� �޸��� 10�� : �ִ� ��밡�� ���� �޸�
private:
	int m_iBufferSize; //��밡���� ���� �޸� ũ��
	int m_iReadIndex;//���ۿ� ����� �����͸� ����ϱ� ���� Index
	int m_iWriteIndex;// ���ۿ� �����ϱ� ���� Index;
	std::vector<Byte> m_Buffer;
	int m_iCapacity; //���� ��밡���� �ִ� ���� �޸� ũ��
public:
	RecvBuffer(int _bufferSize);
	~RecvBuffer();

	void Clean();
	bool Read(int Bytes);
	bool Write(int Bytes);
	Byte* GetReadAddress() { return &m_Buffer[m_iReadIndex]; }
	Byte* GetWriteAddress() { return &m_Buffer[m_iWriteIndex]; }
	int GetDataSize() { return m_iWriteIndex - m_iReadIndex; }
	int GetFreeSize() { return m_iCapacity - m_iWriteIndex; }
};

