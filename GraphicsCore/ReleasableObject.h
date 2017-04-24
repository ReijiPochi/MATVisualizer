#pragma once
#include <vector>

class ReleasableObject
{
public:
	ReleasableObject();
	static void ReleaseAll();
	virtual void Release();
	static std::vector<ReleasableObject*> *objectsList;
};