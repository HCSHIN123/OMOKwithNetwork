#include "pch.h"
#include "RecvBufferHelper.h"

RecvBufferHelper::RecvBufferHelper()
{
	m_pBuffer = nullptr;
	m_iCurIndex = 0;
	m_iSize = 0;
}

RecvBufferHelper::RecvBufferHelper(Byte* _buffer, unsigned int _size, unsigned int _index)
{
	m_pBuffer = _buffer;
	m_iCurIndex = _index;
	m_iSize = _size;
}

RecvBufferHelper::~RecvBufferHelper()
{
}

bool RecvBufferHelper::Peek(void* _destination, unsigned int _length)
{
	//_length ��ŭ ���� �� �ִ� �����Ͱ� �ִ��� Ȯ��
	if (GetUseableSize() < _length)
		return false;
	//�����͸� destination�� ����
	memcpy(_destination, &m_pBuffer[m_iCurIndex], _length);
	return true;
}

bool RecvBufferHelper::Read(void* _destination, unsigned int _length)
{
	if (Peek(_destination, _length) == false)
		return false;
	//�����͸� ���ó�� �Ͽ� index�� ������ŭ �ڷ� �̵�
	m_iCurIndex += _length;
	return true;
}
