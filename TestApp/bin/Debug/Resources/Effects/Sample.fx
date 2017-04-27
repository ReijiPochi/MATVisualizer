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
};

GSPSInput VSFunc(VSInput input)
{
	GSPSInput output = (GSPSInput)0;

	output.Position = mul(camera, float4(input.Position, 1.0f));
	output.data = float4(dataBuffer.Load(input.dataIndex), 1.0f);

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
	return input.data;
}