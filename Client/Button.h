#pragma once
#include"Object.h"

class Texture;
class Button :public Object
{
protected:
	
	std::string m_strName;
	std::function<void()> m_CallBackFunction;
	Texture* m_pButtonTexture;
	//Texture* m_pButtonTextureX;
	RECT	m_rect;

	
public:
	Button();
	~Button();
	// Object을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(HDC _memDC) override;
	virtual void Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type) override;
	virtual void Init(Vector2 _vec2Position, TEXTURE_TYPE _eTexture_Type, std::function<void()> _CallBackFunction, std::string _name);
	void SetRect(Vector2 _vec2Position);
	
	void SetName(std::string _name) { m_strName = _name; }
};

