#include "pch.h"
#include "PathManager.h"

PathManager::PathManager() : m_szContentsPath{}
{

}
PathManager::~PathManager()
{

}
void PathManager::Init()
{
	GetCurrentDirectory(sizeof(m_szContentsPath), m_szContentsPath);
	int length = strlen(m_szContentsPath);
	int i = length - 1;
	while (m_szContentsPath[i] != '\\') { --i; }
	m_szContentsPath[i] = NULL;
	strcat_s(m_szContentsPath, strResourcePath.c_str());
}
