#pragma once

//TLS : Thread Local Storage
extern thread_local int l_ThreadID;
extern thread_local std::stack<int> LockStack;
extern thread_local std::shared_ptr<SendBufferPool> SendBuffPool;

