#pragma once

//사용할 Thread를 체계적으로 관리하기위해 만든 class
class ThreadManager
{
private:
	std::mutex m_Lock;
	std::vector<std::thread> m_vecThreads;
public:
	ThreadManager();
	~ThreadManager();
	void Add(std::function<void()> _callBack);
	void Join();

	static void InitTLS();
	static void DestroyTLS();
};

