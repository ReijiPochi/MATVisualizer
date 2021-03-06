#include "ReleasableObject.h"

std::vector<ReleasableObject*> *ReleasableObject::objectsList = new std::vector<ReleasableObject*>;

ReleasableObject::ReleasableObject()
{
	objectsList->push_back(this);
}

void ReleasableObject::ReleaseAll()
{
	for (std::vector<ReleasableObject*>::iterator itr = objectsList->begin(); itr != objectsList->end(); ++itr)
	{
		if ((*itr) != nullptr)
		{
			if ((*itr)->isReleased)
				continue;

			(*itr)->Release();
			delete *itr;
		}
	}
}

void ReleasableObject::Release()
{
	isReleased = true;
}