#pragma once

#include "DLL.h"

#define GRAPHICS_OBJECT_MAX_COUNT (128)

struct GraphicsObjectDescription
{
	int dummy;
};

class GraphicsObject
{
public:
	static int Create(GraphicsObjectDescription desc);
	int objectID;

private:
	static int GetNewObjectID();
};

extern "C"
{
	static GraphicsObject* objects[GRAPHICS_OBJECT_MAX_COUNT];
	DLL_API int GraphicsObject_Create(GraphicsObjectDescription desc);
}