#pragma once

//����� Thread�� ü�������� �����ϱ����� ���� class
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

