#include "pch.h"
#include "RoomButton.h"
#include "NetworkManager.h"
#include "InputManager.h"
#include "GameCore.h"


RoomButton::RoomButton()
{
	m_CurrentRoom = Room(-1, -1);
}

void RoomButton::Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type, std::function<void(int)> _CallBackFunction, Room _roomInfo)
{
	Button::Init(_vec2Position, _eTexture_Type);
	m_EnterCheckFunc = _CallBackFunction;
	m_CurrentRoom = _roomInfo;
}


void RoomButton::Update()
{
	if (is_Activated() == false)
		return;

	if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::UP)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(GameCore::GetInstance()->GethWnd(), &ptMouse);
		if (m_EnterCheckFunc != nullptr && PtInRect(&m_rect, ptMouse) == true)
		{
			m_EnterCheckFunc(m_CurrentRoom.m_iRoomID);
		}
	}
}

void RoomButton::Render(HDC _memDC)
{
	if (!is_Activated())
		return;
	m_strMessage = "방번호 : " + std::to_string(m_CurrentRoom.m_iRoomID) + "             현재인원 : " + std::to_string(m_CurrentRoom.m_iCount) + "/2";
	Button::Render(_memDC);
	SetBkMode(_memDC, TRANSPARENT);
	TextOut(_memDC, m_rect.left + 10, m_rect.top + 25, m_strMessage.c_str(), m_strMessage.length());
}
