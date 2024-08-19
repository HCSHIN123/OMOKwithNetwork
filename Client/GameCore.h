#pragma once
#include"pch.h"
#include "Texture.h"
class GameCore
{
	SINGLETON(GameCore)
private:
	HWND					m_hWnd;
	HDC						m_hDC;
	HBITMAP				m_hBackBuffer;
	HDC						m_hBackDC;
	Vector2					m_WindowSize;
	Vector2					m_WindowStartPosition;

	int							m_iID;
	
public:
	void Init(HWND _hWnd, Vector2 _size);
	void GameLoop();
	HWND GethWnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	void CreateBackDC();
	Vector2 GetWindowSize() { return m_WindowSize; }
	void DisConnectPlayer(int _iID);
	int GetID() { return m_iID; }
	void RegistID(int _iID) { if (m_iID == -1) m_iID = _iID; }
	
private:
	void Update();
	void Render();
};

