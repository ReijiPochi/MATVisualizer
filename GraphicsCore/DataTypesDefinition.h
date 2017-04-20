#pragma once

#include <windows.h>

struct Vector3
{
	Vector3(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	float X;
	float Y;
	float Z;
};

struct VertexData_Shape
{
	Vector3 SV_Position;
};

struct VertexData_ShapeAndValue
{
	Vector3 SV_Position;
	UINT GC_DataIndex1;
};