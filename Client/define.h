#pragma once

#define SINGLETON(Type) public:\
										static Type* GetInstance()\
										{\
											static Type instance;\
											return &instance;\
										}\
									private:\
										Type();\
										~Type();


enum class OBJECT_GROUP
{
	MANAGER,
	BOARD,
	BLOCK,
	WALL_BLOCK,
	BUTTON,
	END,

};