#include "pch.h"
#include "Button.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "InputManager.h"
#include "GameCore.h"



Button::Button()
{
	m_pButtonTexture = nullptr;
	m_CallBackFunction = nullptr;
	SetActivate(false);
}

Button::~Button()
{
}

void Button::Update()
{
	if (is_Activated() == false)
		return;

	if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::UP)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(GameCore::GetInstance()->GethWnd(), &ptMouse);
		if (m_CallBackFunction != nullptr && PtInRect(&m_rect, ptMouse) == true)
		{
			m_CallBackFunction();
		}
	}
}

void Button::Render(HDC _memDC)
{
	if (is_Activated() == false)
		return;
	TransparentBlt(_memDC, m_rect.left, m_rect.top, m_pButtonTexture->GetWidth(), m_pButtonTexture->GetHeight(), m_pButtonTexture->GetDC(),
			0, 0, m_pButtonTexture->GetWidth(), m_pButtonTexture->GetHeight(), RGB(255, 0, 255));
}

void Button::Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type)
{
	m_pButtonTexture = ResourceManager::GetInstance()->LoadTexture(_eTexture_Type);
	SetRect(_vec2Position);
}

void Button::Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type, std::function<void()> _CallBackFunction, std::string _name)
{
	
	Init(_vec2Position, _eTexture_Type);
	SetName(_name);
	m_CallBackFunction = _CallBackFunction;
}

void Button::SetRect(Vector2 _vec2Position)
{
	Object::SetPosition(_vec2Position);
	m_rect.left = _vec2Position.x;
	m_rect.top = _vec2Position.y;
	m_rect.right = _vec2Position.x + m_pButtonTexture->GetWidth();
	m_rect.bottom = _vec2Position.y + m_pButtonTexture->GetHeight();
}
