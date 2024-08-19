#include "pch.h"
#include "Lock.h"
#include<Windows.h>

Lock::Lock()
{
	m_Lock.store(EMPTY_MASK);
	m_iWriteCount = 0;
}

Lock::~Lock()
{
}

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PushLock(name);
#endif
	const int CurThreadID = (m_Lock.load() & WRITE_MASK) >> 16;

	//동일한 Thread가 재차  Lock을 걸경우 LockCheck없이 바로 진행함
	if (CurThreadID == l_ThreadID)
	{
		++m_iWriteCount;
		return;
	}
	//ThreadID를 Write에 저장하며  Read는 0으로 초기화
	const int Flag = (l_ThreadID << 16) & WRITE_MASK;
	const int OldTick = GetTickCount64();
	while (true)
	{
		for (int SpintCount = 0; SpintCount < MAX_SPIN_COUNT; ++SpintCount)
		{
			//현재 Write Read모두 소유권이 없을시에 자신의 ThreadID 를 Write로 등록
			int Check = EMPTY_MASK;
			if (m_Lock.compare_exchange_strong(Check, Flag) == true)
			{
				++m_iWriteCount;
				return;
			}
		}
		//일정시간 이상  Lock 실패시 문제가 있으므로 강제 Crash발생
		if (GetTickCount64() - OldTick >= TIMEOUT_TICK)
			CRASH
		//잠시 cpu주도권을 포기함
		std::this_thread::yield();
	}
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PushLock(name);
#endif
	//Write 중인 ThreadID를 가져옴
	const int CurThreadID = (m_Lock.load() & WRITE_MASK) >> 16;
	//자신의 ThreadID와 동일할경우 Read는 동시에 진행이 불가능하다는 전제하에 진행하므로
	//다른 Thread가 접근이 불가능 하기 때문에 무조건적인 성공으로 보고 Read를 1올려줌
	if (CurThreadID == l_ThreadID)
	{
		m_Lock.fetch_add(1);
		return;
	}

	const int OldTick = GetTickCount64();
	while (true)
	{
		for (int SpintCount = 0; SpintCount < MAX_SPIN_COUNT; ++SpintCount)
		{
			//Check변수에 Write Bit를 모두 0으로 변환하여 현재 Write중인 Thread가 없는지 확인하여
			//없을경우 Read를 단순하게 1을 증가시켜준다.
			int Check = m_Lock.load() & READ_MASK;
			if (m_Lock.compare_exchange_strong(Check, Check + 1))
				return;
		}
		if (GetTickCount64() - OldTick >= TIMEOUT_TICK)
			CRASH
		std::this_thread::yield();
	}
}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PopLock(name);
#endif
	//현재 Read중인 Thread가 있을경우 Write의 UnLock이 불가능하므로 Crash를 발생시킨다.
	if ((m_Lock.load() & READ_MASK) != 0)
		CRASH
		--m_iWriteCount;
	//Unlock 후 현재 Write중인 Thread가 없을경우
	//Lock을 0으로 초기화시킨다.
	if (m_iWriteCount == 0)
		m_Lock.store(EMPTY_MASK);
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PopLock(name);
#endif
	//Read가 0인상태에서 UnLock을 하는것은
	//Read중인 Thread가 없는데 UnLock하는것이므로 Crash를 발생시킨다.
	if((m_Lock.fetch_sub(1) & READ_MASK) == 0)
		CRASH
}
