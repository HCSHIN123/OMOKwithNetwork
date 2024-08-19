#pragma once


enum TEXTURE_TYPE
{
	BUTTON_PLAY,
	MAINSCENE,
	TITLE,
	CREATE_ROOM,
	ENTER_ROOM,
	GAMEBOARD,
	REFRESH,
	TILE,
	BLACK_STONE,
	WHITE_STONE,
	BUTTON_READY,
	BUTTON_OUT,
	BUTTON_REGAME,
};

class Texture;
class ResourceManager
{
	SINGLETON(ResourceManager)
private:
	std::map<std::string, Texture*> m_MapTexture;
	std::string GetTextureFileName(TEXTURE_TYPE _eTextureType);
	
	
	Texture* FindTexture(const std::string& _strKey);
public:
	Texture* LoadTexture(TEXTURE_TYPE _eTextureType);
};

