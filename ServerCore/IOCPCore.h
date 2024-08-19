#pragma once

//enable_shared_from_this : �ڱ��ڽ��� Shared Point�� ��ȯ�ϱ����� �θ� Class ���
class IOCPObject : public std::enable_shared_from_this<IOCPObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IOCPEvent* _iocpEvent,int Bytes = 0) abstract;
};

class IOCPCore
{
private:
	HANDLE m_hIOCP;
public:
	IOCPCore();
	~IOCPCore();

	bool Register(std::shared_ptr<IOCPObject> _iocpObject);
	bool Dispatch(unsigned int _TimeOut = INFINITE);

	HANDLE GetHandle() { return m_hIOCP; }
};
