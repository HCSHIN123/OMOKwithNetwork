#pragma once

//������ Crash�� ��������� ����ϴ� Mecro
#define CRASH									\
{													\
	int* ptr = nullptr;							\
	*ptr = 10;									\
} 


//Ư�� ���ǿ� ������ Crash�� ��������� ����ϴ� Mecro
#define CONDITION_CRASH(Condition)	\
{													\
	if (!(Condition))								\
	{												\
		CRASH									\
	}												\
}									


#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0)