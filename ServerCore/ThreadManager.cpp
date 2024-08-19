#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"

ThreadManager::ThreadManager()
{
	ThreadManager::InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Add(std::function<void()> _callBack)
{
	std::lock_guard<std::mutex> guard(m_Lock);
	m_vecThreads.push_back(std::thread([](std::function<void()> _callBack)
	{
		ThreadManager::InitTLS();
		_callBack();
		ThreadManager::DestroyTLS();
	}, _callBack));
}

void ThreadManager::Join()
{
	for (std::thread& thread : m_vecThreads)
		if (thread.joinable())
			thread.join();
	m_vecThreads.clear();


}

void ThreadManager::InitTLS()
{
	static std::atomic<unsigned int> sThreadID = 0;
	l_ThreadID = ++sThreadID;
}

void ThreadManager::DestroyTLS()
{
}
