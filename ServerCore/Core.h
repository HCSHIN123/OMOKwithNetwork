#pragma once
#include "ThreadManager.h"
#include "DeadLockTracking.h"
#include "SendBufferManager.h"

//기본적인 Server Library에 공통적으로 사용하는 Manager들 관리
class Core
{
private:
	ThreadManager* m_pThreadManager;
	DeadLockTracking* m_pDeadLockTracking;
	SendBufferManager* m_pSendBufferManager;
	Core();
public:
	ThreadManager* GetThreadManager()
	{
		return m_pThreadManager;
	}
	DeadLockTracking* GetDeadLockTracking()
	{
		return m_pDeadLockTracking;
	}
	SendBufferManager* GetSendBufferManager()
	{
		return m_pSendBufferManager;
	}
	static Core& GetInstance()
	{
		static Core core;
		return core;
	}
	~Core();
};

