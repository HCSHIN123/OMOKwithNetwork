#pragma once
#pragma comment(lib, "msimg32.lib")
#ifdef _DEBUG
#pragma comment(lib,"ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib,"Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib,"ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib,"Protobuf\\Release\\libprotobuf.lib")
#endif

#include"CorePCH.h"
#include"struct.h"
#include"const.h"
#include"define.h"
#include<Windows.h>