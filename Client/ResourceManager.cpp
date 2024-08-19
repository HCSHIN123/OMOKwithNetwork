#include "pch.h"
#include "ResourceManager.h"
#include "PathManager.h"
#include "Texture.h"

ResourceManager::ResourceManager()
{

}
ResourceManager::~ResourceManager()
{
	for (auto iter = m_MapTexture.begin(); iter != m_MapTexture.end(); iter++)
		delete iter->second;
}

std::string ResourceManager::GetTextureFileName(TEXTURE_TYPE _eTextureType)
{
	switch (_eTextureType)
	{
		case TEXTURE_TYPE::BUTTON_PLAY:									return "OmokPlayButton.bmp";
		case TEXTURE_TYPE::MAINSCENE:									return "OmokMainScene.bmp";
		case TEXTURE_TYPE::TITLE:									return "OmokTitle.bmp";
		case TEXTURE_TYPE::CREATE_ROOM:									return "CreateRoom.bmp";
		case TEXTURE_TYPE::ENTER_ROOM:									return "EnterRoom.bmp";
		case TEXTURE_TYPE::GAMEBOARD:									return "tile_1.bmp";
		case TEXTURE_TYPE::REFRESH:									return "Refresh_Texture.bmp";
		case TEXTURE_TYPE::TILE:									return "MoveableTileMask.bmp";
		case TEXTURE_TYPE::BLACK_STONE:									return "baduk_b.bmp";
		case TEXTURE_TYPE::WHITE_STONE:									return "baduk_w.bmp";
		case TEXTURE_TYPE::BUTTON_READY:									return "ReadyBtn.bmp";
		case TEXTURE_TYPE::BUTTON_OUT:									return "OutBtn.bmp";
		case TEXTURE_TYPE::BUTTON_REGAME:									return "RegameButton.bmp";
	
	default: return "";
	}
}



Texture* ResourceManager::LoadTexture(TEXTURE_TYPE _eTextureType)
{
	std::string strFileName = GetTextureFileName(_eTextureType);
	assert(strFileName.length() != 0);

	std::string strKey = strFileName.substr(0, strFileName.length() - 4);

	Texture* pTexture = FindTexture(strKey);
	if (pTexture == nullptr)
	{
		pTexture = new Texture;
		std::string strPath = PathManager::GetInstance()->GetContentpath();
		strPath += strTexturePath + strFileName;
		pTexture->Load(strPath);
		pTexture->SetKey(strKey);
		pTexture->SetRelativePath(strFileName);
		m_MapTexture.insert(std::make_pair(strKey, pTexture));
	}
	return pTexture;
}

Texture* ResourceManager::FindTexture(const std::string& _strKey)
{
	std::map<std::string, Texture*>::iterator iter = m_MapTexture.find(_strKey);
	if (iter == m_MapTexture.end())
		return nullptr;
	else
		return iter->second;
}
