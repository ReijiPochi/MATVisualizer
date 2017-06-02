#pragma once
#include <vector>

class ReleasableObject
{
public:
	ReleasableObject();
	static void ReleaseAll();
	bool isReleased = false;
	virtual void Release();
	static std::vector<ReleasableObject*> *objectsList;
};