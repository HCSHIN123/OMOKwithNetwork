#pragma once
#include"SendBuffer.h"

enum class EVENT_TYPE
{
	CONNECT,
	DISCONNECT,
	ACCEPT,
	RECV,
	SEND,
};

class Session;
class IOCPEvent : public OVERLAPPED
{
protected:
	EVENT_TYPE m_eEventType;
	std::shared_ptr<IOCPObject> m_Owner;
public:
	IOCPEvent(EVENT_TYPE _eType);
	void Init();
	EVENT_TYPE GetEventType() { return m_eEventType; }
	std::shared_ptr<IOCPObject>& GetOwner() { return m_Owner; }
	void SetOwner(std::shared_ptr<IOCPObject> _owner) { m_Owner = _owner; }
};

class ConnectEvent : public IOCPEvent
{
public:
	ConnectEvent()
		: IOCPEvent(EVENT_TYPE::CONNECT) { }
};

class DisconnectEvent : public IOCPEvent
{
public:
	DisconnectEvent()
		: IOCPEvent(EVENT_TYPE::DISCONNECT) { }
};

class AcceptEvent : public IOCPEvent
{
private:
	std::shared_ptr<Session> m_Session;
public:
	AcceptEvent()
		: IOCPEvent(EVENT_TYPE::ACCEPT), m_Session(nullptr){ }
	void SetSession(std::shared_ptr<Session>& _session)
	{
		m_Session = _session;
	}
	std::shared_ptr<Session>& GetSession()
	{
		return m_Session;
	}
};

class RecvEvent : public IOCPEvent
{
private:
public:
	RecvEvent()
		: IOCPEvent(EVENT_TYPE::RECV) { }
};

class SendEvent : public IOCPEvent
{
private:
	std::vector<std::shared_ptr<SendBuffer>> m_Buff;
public:
	SendEvent()
		: IOCPEvent(EVENT_TYPE::SEND) { }
	void SetBuff(Byte* _buff, int Length)
	{
		m_Buff.resize(Length);
		//vector.data() : vector의 첫 주소 반환
		memcpy(m_Buff.data(), _buff, Length);
	}
	std::vector<std::shared_ptr<SendBuffer>>& GetBuff() { return m_Buff; }
};

