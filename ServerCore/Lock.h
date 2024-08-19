#pragma once
class Lock
{
	enum
	{
		TIMEOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_MASK = 0xFFFF0000,
		READ_MASK = 0x0000FFFF,
		EMPTY_MASK = 0,
	};
private:
	//[00000000] [00000000]
	//    Write        Read
	std::atomic<int> m_Lock;
	int m_iWriteCount;
public:
	Lock();
	~Lock();
	void WriteLock(const char* name);
	void ReadLock(const char* name);
	void WriteUnLock(const char* name);
	void ReadUnLock(const char* name);
};


class LockGuardRead
{
public:
	LockGuardRead(Lock& _Lock,const char* _name) : m_Lock(_Lock),m_name(_name)
	{
		m_Lock.ReadLock(m_name);
	}
	~LockGuardRead()
	{
		m_Lock.ReadUnLock(m_name);
	}
private:
	Lock& m_Lock;
	const char* m_name;
};


class LockGuardWrite
{
public:
	LockGuardWrite(Lock& _Lock, const char* _name) : m_Lock(_Lock), m_name(_name)
	{
		m_Lock.WriteLock(m_name);
	}
	~LockGuardWrite()
	{
		m_Lock.WriteUnLock(m_name);
	}
private:
	Lock& m_Lock;
	const char* m_name;
};