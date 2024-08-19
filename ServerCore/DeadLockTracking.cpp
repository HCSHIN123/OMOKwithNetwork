#include "pch.h"
#include "DeadLockTracking.h"

void DeadLockTracking::DFS(int id)
{
	if (m_Discovered[id] != -1)
		return;

	m_Discovered[id] = m_iDiscoveredCount++;

	//인접한 정접 순회
	std::map<int, std::set<int>>::iterator findIter = m_LockLog.find(id);
	if (findIter == m_LockLog.end())
	{
		m_Finished[id] = true;
		return;
	}

	std::set<int>& nextSet = findIter->second;
	for (int next : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (m_Discovered[next] == -1)
		{
			m_Parent[next] = id;
			DFS(next);
			continue;
		}

		// here가 there보다 먼저 발견된 상황 : 순방향 간선
		if (m_Discovered[id] < m_Discovered[next])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않은상황 : 역방향 간선
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
	//등록된적 없는 LockName일경우 ID를발급하고 있을경우 ID를 가져온다.
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

	//Lock이 잡혀있는 상태
	if (LockStack.empty() == false)
	{
		//LockStack 가장위에 있는ID를 가져와
		int prevLockID = LockStack.top();
		//현재 ID와 다를경우 데드락상태인지 확인한다.
		if (lockID != prevLockID)
		{
			//해당 PrevID의 Log에 연결되어있는 ID 중 현재 LockID가 없는지 확인
			std::set<int>& log = m_LockLog[prevLockID];
			if (log.find(lockID) == log.end())
			{
				log.insert(lockID);
				CycleCheck();
			}
		}
	}
	//현재 잡은 락을 Stack에 등록
	LockStack.push(lockID);
}

void DeadLockTracking::PopLock(const char* name)
{
	std::lock_guard<std::mutex> lockGuard(m_Lock);
	//Stack에 등록된 Lock이 없을경우
	if (LockStack.empty())
		CRASH

	//등록된 이름의 Lock이 없거나 해당 LockID가 현재Stack의 LockID가 다를경우
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
