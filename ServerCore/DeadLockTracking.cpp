#include "pch.h"
#include "DeadLockTracking.h"

void DeadLockTracking::DFS(int id)
{
	if (m_Discovered[id] != -1)
		return;

	m_Discovered[id] = m_iDiscoveredCount++;

	//������ ���� ��ȸ
	std::map<int, std::set<int>>::iterator findIter = m_LockLog.find(id);
	if (findIter == m_LockLog.end())
	{
		m_Finished[id] = true;
		return;
	}

	std::set<int>& nextSet = findIter->second;
	for (int next : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
		if (m_Discovered[next] == -1)
		{
			m_Parent[next] = id;
			DFS(next);
			continue;
		}

		// here�� there���� ���� �߰ߵ� ��Ȳ : ������ ����
		if (m_Discovered[id] < m_Discovered[next])
			continue;

		// �������� �ƴϰ�, Dfs(there)�� ���� �������� ������Ȳ : ������ ����
		if (m_Finished[next] == false)
		{
			printf("%s -> %s\n", m_LockIDMap[id], m_LockIDMap[next]);

			int curID = id;
			while (true)
			{
				printf("%s -> %s\n", m_LockIDMap[m_Parent[curID]], m_LockIDMap[curID]);
				curID = m_Parent[curID];
				if (curID == next)
					break;
			}

			CRASH
		}
	}
	m_Finished[id] = true;
}

DeadLockTracking::DeadLockTracking()
{
	m_iDiscoveredCount = 0;
}

DeadLockTracking::~DeadLockTracking()
{
}

void DeadLockTracking::PushLock(const char* name)
{
	std::lock_guard<std::mutex> lockGuard(m_Lock);
	//��ϵ��� ���� LockName�ϰ�� ID���߱��ϰ� ������� ID�� �����´�.
	std::unordered_map< const char*, int>::iterator  findIterID = m_LockNameMap.find(name);
	int lockID = 0;
	if (findIterID == m_LockNameMap.end())
	{
		lockID = m_LockNameMap.size();
		m_LockNameMap.insert(std::make_pair(name, lockID));
		m_LockIDMap.insert(std::make_pair(lockID, name));
	}
	else
		lockID = findIterID->second;

	//Lock�� �����ִ� ����
	if (LockStack.empty() == false)
	{
		//LockStack �������� �ִ�ID�� ������
		int prevLockID = LockStack.top();
		//���� ID�� �ٸ���� ������������� Ȯ���Ѵ�.
		if (lockID != prevLockID)
		{
			//�ش� PrevID�� Log�� ����Ǿ��ִ� ID �� ���� LockID�� ������ Ȯ��
			std::set<int>& log = m_LockLog[prevLockID];
			if (log.find(lockID) == log.end())
			{
				log.insert(lockID);
				CycleCheck();
			}
		}
	}
	//���� ���� ���� Stack�� ���
	LockStack.push(lockID);
}

void DeadLockTracking::PopLock(const char* name)
{
	std::lock_guard<std::mutex> lockGuard(m_Lock);
	//Stack�� ��ϵ� Lock�� �������
	if (LockStack.empty())
		CRASH

	//��ϵ� �̸��� Lock�� ���ų� �ش� LockID�� ����Stack�� LockID�� �ٸ����
	int lockID = m_LockNameMap[name];
	if(LockStack.top() != lockID)
		CRASH


	LockStack.pop();
}

void DeadLockTracking::CycleCheck()
{
	int iLockCount = m_LockNameMap.size();
	m_Discovered = std::vector<int>(iLockCount, -1);
	m_iDiscoveredCount = 0;
	m_Finished = std::vector<bool>(iLockCount, false);
	m_Parent = std::vector<int>(iLockCount, -1);

	for (int i = 0; i < iLockCount; ++i)
		DFS(i);
	m_Discovered.clear();
	m_Finished.clear();
	m_Parent.clear();
}
