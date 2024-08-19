#pragma once
class RecvBuffer
{
	enum {BUFFER_COUNT = 10}; //기존 사용가능 버퍼 메모리의 10배 : 최대 사용가능 버퍼 메모리
private:
	int m_iBufferSize; //사용가능한 버퍼 메모리 크기
	int m_iReadIndex;//버퍼에 저장된 데이터를 사용하기 위한 Index
	int m_iWriteIndex;// 버퍼에 저장하기 위한 Index;
	std::vector<Byte> m_Buffer;
	int m_iCapacity; //실제 사용가능한 최대 버퍼 메모리 크기
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

