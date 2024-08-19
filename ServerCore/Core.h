#pragma once
#include "ThreadManager.h"
#include "DeadLockTracking.h"
#include "SendBufferManager.h"

//�⺻���� Server Library�� ���������� ����ϴ� Manager�� ����
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

