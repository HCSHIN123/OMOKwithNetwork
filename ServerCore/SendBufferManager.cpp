#include "pch.h"
#include "SendBufferManager.h"

std::shared_ptr<SendBuffer> SendBufferManager::GetSendBuffer(unsigned int _size)
{
	//사용중인 SendBuff가 없을경우 Pool에서 새로 받아오기
	if (SendBuffPool == nullptr)
	{
		SendBuffPool = Pop();
		SendBuffPool->Init();
	}

	assert(SendBuffPool->Useable());

	//만일 전달받은 Buff의 남은 사용공간이 현재 사용할 공간보다 작을경우 버리고 새 Buff를 전달받음
	while (SendBuffPool->GetFreeSize() < _size)
	{
		SendBuffPool = Pop();
		SendBuffPool->Init();
	}
	std::cout << "Useable SendBuffPool Memory : " << SendBuffPool->GetFreeSize() << std::endl;
	//분배할 Buff의 사용상태 갱신
	return SendBuffPool->GetBuffer(_size);
}

std::shared_ptr<SendBufferPool> SendBufferManager::Pop()
{
	{
		LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
		//Pool에 남은 Buff가 없을경우 새로 만들어 전달
		if (m_SendBufferPools.empty() == false)
		{
			//Pool에 여유 Buff가 있을경우 받아서 전달
			std::shared_ptr<SendBufferPool> sendBufferPool = m_SendBufferPools.back();
			m_SendBufferPools.pop_back();
			return sendBufferPool;
		}
	}
	//shared_ptr 생성 시 두번째 인자에 함수를 넣을경우 
	//해당 shared_ptr의 referece counting이 0됐을때 메모리 해제하지 않고 전달한 함수를 호출하게 한다.
	return std::make_shared<SendBufferPool>();
}

void SendBufferManager::Push(std::shared_ptr<SendBufferPool> _buff)
{
	//Pool에 여유 Buff 추가
	LockGuardWrite writeLockGuard(m_Lock, typeid(this).name());
	m_SendBufferPools.push_back(_buff);
}

void SendBufferManager::PushGlobal(SendBufferPool* _buff)
{
	//각 Thread 별로 별도의 SendBuff를 관리하는 Manager를 thread_local로 만들어 저장
	Core::GetInstance().GetSendBufferManager()->Push(std::shared_ptr<SendBufferPool>(_buff, PushGlobal));
}
