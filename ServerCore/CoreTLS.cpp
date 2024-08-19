#include "pch.h"
#include "CoreTLS.h"

thread_local int l_ThreadID = 0;
thread_local std::stack<int> LockStack;
thread_local std::shared_ptr<SendBufferPool> SendBuffPool;