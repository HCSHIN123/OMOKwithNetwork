#pragma once

//강제로 Crash를 내고싶을때 사용하는 Mecro
#define CRASH									\
{													\
	int* ptr = nullptr;							\
	*ptr = 10;									\
} 


//특정 조건에 강제로 Crash를 내고싶을때 사용하는 Mecro
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