//----------------------------------------------------------------------
// File : Sample.fx
// Desc : Sample Shader
// Copyright(c) Project Asura. All right reserved.
//----------------------------------------------------------------------


cbuffer globalCBuffer : register(b0)
{
	matrix camera;
};

Buffer<float3> dataBuffer;

struct VSInput
{
	float3 Position : SV_Position;
	uint dataIndex : GC_DataIndex;
};

struct GSPSInput
{
	float4 Position : SV_Position;
	float4 data : GC_DataValue;
	float3 OriginalPosition : GC_Position;
};

GSPSInput VSFunc(VSInput input)
{
	GSPSInput output = (GSPSInput)0;

	output.Position = mul(camera, float4(input.Position, 1.0f));
	output.data = float4(dataBuffer.Load(input.dataIndex), 1.0f);
	output.OriginalPosition = input.Position;

	return output;
}

[maxvertexcount(3)]
void GSFunc(triangle GSPSInput input[ 3 ], inout TriangleStream<GSPSInput> stream)
{
	stream.Append(input[0]);
	stream.Append(input[1]);
	stream.Append(input[2]);

	stream.RestartStrip();
}

float4 PSFunc(GSPSInput input) : SV_Target0
{
	//float3 direction = float3(0.05, 0.05, 0.05) - input.OriginalPosition;

	//if (dot(direction, float3(0.5, 0.5, 0.5)) < 0)
	//{
	//	input.data = float4(0.0, 0.0, 0.0, 0.0);
	//}

	return input.data;
}