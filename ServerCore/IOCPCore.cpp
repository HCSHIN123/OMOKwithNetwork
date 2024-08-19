#include "pch.h"
#include "IOCPCore.h"


IOCPCore::IOCPCore()
{
	//CompletionPort ����
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
	assert(m_hIOCP != INVALID_HANDLE_VALUE);
}

IOCPCore::~IOCPCore()
{
	//CompletionPort ����
	CloseHandle(m_hIOCP);
}

bool IOCPCore::Register(std::shared_ptr<IOCPObject> _iocpObject)
{
	//CompletionPort�� Ư�� IOCPObject�� �񵿱�� �۾��Ϸ� ���θ� �˸� ��� ����Ѵ�.
	SOCKET socket;
	return CreateIoCompletionPort(_iocpObject->GetHandle(), m_hIOCP, 0, 0);
}

bool IOCPCore::Dispatch(unsigned int _TimeOut)
{
	DWORD Bytes = 0;
	ULONG_PTR Key;
	IOCPEvent* iocpEvent = nullptr;
	//����س��Ҵ� ��ȣ����� ����� �ش� ��ȣ�� IOCPObject�� IOCPEvent���� �� ó��
	bool Result = GetQueuedCompletionStatus(m_hIOCP, &Bytes, &Key, reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), _TimeOut);
	
	if (Result == true)
	{
		//IOCPEvent�� �����ص״� IOCPObject ����
		std::shared_ptr<IOCPObject> iocpObject = iocpEvent->GetOwner();
		//�۾��� ��ȣ�� ���޵Ǹ� �ش� IOCPObject �۾� ����
		iocpObject->Dispatch(iocpEvent, Bytes);
	}
	else
	{
		int ErrorCode = WSAGetLastError();
		if (ErrorCode == WAIT_TIMEOUT)
			return false;
		else
		{
			//IOCPEvent�� �����ص״� IOCPObject ����
			std::shared_ptr<IOCPObject> iocpObject = iocpEvent->GetOwner();
			iocpObject->Dispatch(iocpEvent, Bytes);
		}
	}

	return true;
}
