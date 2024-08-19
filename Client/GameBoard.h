#pragma once
#include "Stone.h"
#include "Object.h"
class GameBoard : public Object
{
private:
	bool m_bMyTurn;
	bool m_bGameEnd;
	Stone m_Board[BOARD_SCALE::HEIGHT][BOARD_SCALE::WIDTH];
	Protocol::STONE_COLOR m_eMyColor;
	Protocol::STONE_COLOR m_eEnemyColor;
public:
	GameBoard();
	~GameBoard();
	void Init();
	void InitStone();
	void Reset();
	virtual void Update();
	void StoneCheck(POINT _pt, bool _isClick);
	void WinCheck(Vector2 _pos);
	int LineCheck(Vector2 _pos, Vector2 _dir);
	bool isMyturn() { return m_bMyTurn; }
	void StartGame(Protocol::STONE_COLOR _color);
	void EnemyInput(Vector2 _pos);
	void TurnBack(Vector2 _pos);
	virtual void Render(HDC _memDC);
	void DrawBoard(HDC _memDC);

	void End() { m_bGameEnd = true; }
	bool isEnd() { return m_bGameEnd; }
};
// ���ӸŴ��� ����, ���������� ��üũ�� �����°�üũ, �� ������Ʈ�� �߰� ���ϰ� ���� ������Ʈ�� ���� ���尡 ���� ������Ʈ�� ������
// ������ ������ ���� �������°� broadcast;, �θ� ����� �����ư�̶� �������ư ����߰� 
