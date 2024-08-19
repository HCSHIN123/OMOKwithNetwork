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

	//������ Thread�� ����  Lock�� �ɰ�� LockCheck���� �ٷ� ������
	if (CurThreadID == l_ThreadID)
	{
		++m_iWriteCount;
		return;
	}
	//ThreadID�� Write�� �����ϸ�  Read�� 0���� �ʱ�ȭ
	const int Flag = (l_ThreadID << 16) & WRITE_MASK;
	const int OldTick = GetTickCount64();
	while (true)
	{
		for (int SpintCount = 0; SpintCount < MAX_SPIN_COUNT; ++SpintCount)
		{
			//���� Write Read��� �������� �����ÿ� �ڽ��� ThreadID �� Write�� ���
			int Check = EMPTY_MASK;
			if (m_Lock.compare_exchange_strong(Check, Flag) == true)
			{
				++m_iWriteCount;
				return;
			}
		}
		//�����ð� �̻�  Lock ���н� ������ �����Ƿ� ���� Crash�߻�
		if (GetTickCount64() - OldTick >= TIMEOUT_TICK)
			CRASH
		//��� cpu�ֵ����� ������
		std::this_thread::yield();
	}
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PushLock(name);
#endif
	//Write ���� ThreadID�� ������
	const int CurThreadID = (m_Lock.load() & WRITE_MASK) >> 16;
	//�ڽ��� ThreadID�� �����Ұ�� Read�� ���ÿ� ������ �Ұ����ϴٴ� �����Ͽ� �����ϹǷ�
	//�ٸ� Thread�� ������ �Ұ��� �ϱ� ������ ���������� �������� ���� Read�� 1�÷���
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
			//Check������ Write Bit�� ��� 0���� ��ȯ�Ͽ� ���� Write���� Thread�� ������ Ȯ���Ͽ�
			//������� Read�� �ܼ��ϰ� 1�� ���������ش�.
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
	//���� Read���� Thread�� ������� Write�� UnLock�� �Ұ����ϹǷ� Crash�� �߻���Ų��.
	if ((m_Lock.load() & READ_MASK) != 0)
		CRASH
		--m_iWriteCount;
	//Unlock �� ���� Write���� Thread�� �������
	//Lock�� 0���� �ʱ�ȭ��Ų��.
	if (m_iWriteCount == 0)
		m_Lock.store(EMPTY_MASK);
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	Core::GetInstance().GetDeadLockTracking()->PopLock(name);
#endif
	//Read�� 0�λ��¿��� UnLock�� �ϴ°���
	//Read���� Thread�� ���µ� UnLock�ϴ°��̹Ƿ� Crash�� �߻���Ų��.
	if((m_Lock.fetch_sub(1) & READ_MASK) == 0)
		CRASH
}
