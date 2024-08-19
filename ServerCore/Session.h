#pragma once
#include<memory>
#include"RecvBuffer.h"
#include"SendBuffer.h"

class Session : public IOCPObject
{
	friend class Listener;
	friend class IOCPCore;
	friend class Service;

	enum { BUFFER_SIZE = 0x10000,};
private:
	Lock m_Lock;
	//weak_ptr : shared_ptr 에 대한 비소유 참조
	std::weak_ptr<Service> m_Service;
	SOCKET m_Socket;
	NetworkAddress m_NetworkAddress;
	std::atomic<bool> m_Connected;

	ConnectEvent m_ConnectEvent;
	RecvEvent m_RecvEvent;
	//지역변수 대신 맴버변수로 잡고 재활용
	SendEvent m_SendEvent;
	DisconnectEvent m_DisconnectEvent;

	RecvBuffer m_RecvBuffer;
	//SendBuffer용 Queue
	std::queue<std::shared_ptr<SendBuffer>> m_SendBufferQueue;
	std::atomic<bool> m_bSendRegistered;

	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();
	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int _bytes);
	void ProcessSend(int _bytes);
	void HandleError(int _errorCode);

protected:
	virtual void OnConnected() {}
	virtual int OnRecv(Byte* _buffer, int _length) { return _length; }
	virtual void OnSend(int _length) {}
	virtual void OnDisconnected() {}
public:

	Session();
	virtual ~Session();

	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IOCPEvent* _iocpEvent, int Bytes = 0) override;

	void SetNetworkAddress(NetworkAddress _networkAddress) { m_NetworkAddress = _networkAddress; }
	NetworkAddress GetNetworkAddress() { return m_NetworkAddress; }
	SOCKET GetSocket() { return m_Socket; }
	bool IsConnected() { return m_Connected; }
	std::shared_ptr<Session> GetSession() { return static_pointer_cast<Session>(shared_from_this()); }

	void Disconnect(const WCHAR* message);
	//weak_ptr.lock : 비참조 ptr에서 새로 참조하는 shared_ptr을 생성한다.
	std::shared_ptr<Service> GetService() { return m_Service.lock(); }
	void SetService(std::shared_ptr<Service> _service) { m_Service = _service; }

	void Send(std::shared_ptr<SendBuffer> _buff);
	bool Connect();
};

