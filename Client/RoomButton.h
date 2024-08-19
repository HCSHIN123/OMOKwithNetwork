#pragma once
#include "Button.h"
class RoomButton : public Button
{
private:
	std::function<void(int)> m_EnterCheckFunc;
	std::string m_strMessage;
	Room m_CurrentRoom;
public:
	RoomButton();
	virtual void Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type, std::function<void(int)> _CallBackFunction, Room _roomInfo);
	void SetRoomInfo(Room _room) { m_CurrentRoom = _room; }
	void Update() override;
	void Render(HDC _memDC) override;
};

