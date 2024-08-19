#pragma once
class DeadLockTracking
{
private:
	//unordered_map : �������� �ʴ� map+
	std::unordered_map<const char*, int> m_LockNameMap;
	std::unordered_map<int, const char*> m_LockIDMap;
	//set : Key�� �����ϴ� map
	std::map<int, std::set<int>>	m_LockLog;
	std::mutex m_Lock;
	std::vector<int> m_Discovered;
	int m_iDiscoveredCount;
	std::vector<bool> m_Finished;
	std::vector<int> m_Parent;

	void DFS(int id);

public:
	DeadLockTracking();
	~DeadLockTracking();
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CycleCheck();
};

