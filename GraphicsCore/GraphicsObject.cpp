#include "GraphicsObject.h"
#include <windows.h>


int GraphicsObject::Create(GraphicsObjectDescription desc)
{
	int id = GetNewObjectID();

	if (id < 0)
		return -1;

	GraphicsObject* go = new GraphicsObject();

	objects[id] = go;
}

// �g�p�ł���I�u�W�F�N�g�X���b�g���������܂��B
int GraphicsObject::GetNewObjectID()
{
	for (int index = 0; index < GRAPHICS_OBJECT_MAX_COUNT; index++)
	{
		if (objects[index] == NULL)
		{
			return index;
		}
	}

	return -1;
}

DLL_API int GraphicsObject_Create(GraphicsObjectDescription desc)
{
	return GraphicsObject::Create(desc);
}