#pragma once

#include<vector>
#include<list>
#include<stack>
#include<queue>
#include<map>
#include<unordered_map>
#include<set>
#include<iostream>
#include<functional>
#include<mutex>
#include<atomic>
#include<memory>
#include<assert.h>
#include<string>
//Core
#include "Core.h"
#include "CoreTLS.h"
#include "CoreMecro.h"

//Thread
#include "ThreadManager.h"
#include "Lock.h"
#include "DeadLockTracking.h"

//Network
//Windows.h 와 함께 Include 시 충돌이 일어날 수 있으므로 주의
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include"SocketUtils.h"
#include"IOCPCore.h"
#include"IOCPEvent.h"
#include"NetworkAddress.h"
#include"Listener.h"
#include"Session.h"
#include"PacketSession.h"
#include"Service.h" 
#include"ServerService.h"
#include"ClientService.h"