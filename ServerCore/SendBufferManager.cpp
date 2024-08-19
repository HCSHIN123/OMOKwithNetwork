#include "pch.h"
#include "SendBufferManager.h"

std::shared_ptr<SendBuffer> SendBufferManager::GetSendBuffer(unsigned int _size)
{
	//������� SendBuff�� ������� Pool���� ���� �޾ƿ���
	if (SendBuffPool == nullptr)
	{
		SendBuffPool = Pop();
		SendBuffPool->Init();
	}

	assert(SendBuffPool->Useable());

	//���� ���޹��� Buff�� ���� �������� ���� ����� �������� ������� ������ �� Buff�� ���޹���
	while (SendBuffPool->GetFreeSize() < _size)
	{
		SendBuffPool = Pop();
		SendBuffPool->Init();
	}
	std::cout << "Useable SendBuffPool Memory : " << SendBuffPool->GetFreeSize() << std::endl;
	//�й��� Buff�� ������ ����
	return SendBuffPool->GetBuffer(_size);
}

std::shared_ptr<SendBufferPool> SendBufferManager::Pop()
{
	{
		LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
		//Pool�� ���� Buff�� ������� ���� ����� ����
		if (m_SendBufferPools.empty() == false)
		{
			//Pool�� ���� Buff�� ������� �޾Ƽ� ����
			std::shared_ptr<SendBufferPool> sendBufferPool = m_SendBufferPools.back();
			m_SendBufferPools.pop_back();
			return sendBufferPool;
		}
	}
	//shared_ptr ���� �� �ι�° ���ڿ� �Լ��� ������� 
	//�ش� shared_ptr�� referece counting�� 0������ �޸� �������� �ʰ� ������ �Լ��� ȣ���ϰ� �Ѵ�.
	return std::make_shared<SendBufferPool>();
}

void SendBufferManager::Push(std::shared_ptr<SendBufferPool> _buff)
{
	//Pool�� ���� Buff �߰�
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	m_SendBufferPools.push_back(_buff);
}

void SendBufferManager::PushGlobal(SendBufferPool* _buff)
{
	//�� Thread ���� ������ SendBuff�� �����ϴ� Manager�� thread_local�� ����� ����
	Core::GetInstance().GetSendBufferManager()->Push(std::shared_ptr<SendBufferPool>(_buff, PushGlobal));
}
