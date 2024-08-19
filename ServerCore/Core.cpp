#include "pch.h"
#include "Core.h"

Core::Core()
{
	m_pThreadManager = new ThreadManager;
	m_pDeadLockTracking = new DeadLockTracking;
	m_pSendBufferManager = new SendBufferManager;
	SocketUtils::Init();//
}

Core::~Core()
{
	if (m_pThreadManager != nullptr)
		delete m_pThreadManager;
	if (m_pDeadLockTracking != nullptr)
		delete m_pDeadLockTracking;
	if(m_pSendBufferManager != nullptr)
		delete m_pSendBufferManager;
	SocketUtils::Clear();//
}
