#include "pch.h"
#include "GameCore.h"
#include"InputManager.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "PathManager.h"

GameCore::GameCore()
{
	m_hDC = nullptr;
	m_hWnd = nullptr;
	m_WindowStartPosition = {};
	m_WindowSize = {};
	m_iID = -1;
}
GameCore::~GameCore()
{
	ReleaseDC(m_hWnd, m_hDC);
}
void GameCore::Init(HWND _hWnd, Vector2 _size)
{
	
	PathManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	
	m_hWnd = _hWnd;
	m_WindowSize = _size;

	m_WindowStartPosition = Vector2{ (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_WindowSize.x / 2),(GetSystemMetrics(SM_CYSCREEN) / 2) - (m_WindowSize.y / 2) };
	SetWindowPos(m_hWnd, nullptr, m_WindowStartPosition.x, m_WindowStartPosition.y, m_WindowSize.x, m_WindowSize.y, SWP_SHOWWINDOW);
	m_hDC = GetDC(m_hWnd);
	m_hBackBuffer = CreateCompatibleBitmap(m_hDC, m_WindowSize.x, m_WindowSize.y);
	m_hBackDC = CreateCompatibleDC(m_hDC);
	HBITMAP hOldBitMap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBuffer);
	DeleteObject(hOldBitMap);

	InputManager::GetInstance()->Init();
	InputManager::GetInstance()->RegistKey(VK_LBUTTON);
	NetworkManager::GetInstance()->Init();
}

void GameCore::GameLoop()
{
	Update();
	Render();
	NetworkManager::GetInstance()->Update();
}

void GameCore::CreateBackDC()
{
	if (m_hBackBuffer != nullptr)
		DeleteObject(m_hBackBuffer);
	if (m_hBackDC != nullptr)
		DeleteDC(m_hBackDC);
	if (SceneManager::GetInstance()->GetCurScene() != nullptr)
	{
		Vector2 WindowSize = SceneManager::GetInstance()->GetCurScene()->GetWindowSize();
		m_hBackBuffer = CreateCompatibleBitmap(m_hDC, WindowSize.x, WindowSize.y);
		m_hBackDC = CreateCompatibleDC(m_hDC);
		HBITMAP hOldBitMap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBuffer);
		DeleteObject(hOldBitMap);
	}
}

void GameCore::DisConnectPlayer(int _iID)
{
	
}

void GameCore::Update()
{
	InputManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
}

void GameCore::Render()
{
	if (SceneManager::GetInstance()->GetCurScene() == nullptr)
		return;
	SceneManager::GetInstance()->Render(m_hBackDC);
	Vector2 WindowSize = SceneManager::GetInstance()->GetCurScene()->GetWindowSize();
	BitBlt(m_hDC, 0, 0, WindowSize.x, WindowSize.y, m_hBackDC, 0, 0, SRCCOPY);
}
