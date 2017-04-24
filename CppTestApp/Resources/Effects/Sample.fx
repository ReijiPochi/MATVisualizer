//----------------------------------------------------------------------
// File : Sample.fx
// Desc : Sample Shader
// Copyright(c) Project Asura. All right reserved.
//----------------------------------------------------------------------


cbuffer globalCBuffer : register(b0)
{
	matrix camera;
};

struct VSInput
{
	float3 Position : SV_Position;
	uint dataIndex : GC_DataIndex;
};

struct GSPSInput
{
	float4 Position : SV_Position;
	float dataIndex : GC_DataIndex;
};

GSPSInput VSFunc( VSInput input )
{
	GSPSInput output = (GSPSInput)0;

	output.Position = mul(camera, float4(input.Position, 1.0f));
	output.dataIndex = input.dataIndex;

	return output;
}


[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[ 3 ], inout TriangleStream<GSPSInput> stream )
{
	stream.Append(input[0]);
	stream.Append(input[1]);
	stream.Append(input[2]);

	stream.RestartStrip();
}


float4 PSFunc( GSPSInput input ) : SV_Target0
{
	float g = (float)input.dataIndex;
	return float4( 0.5f, g * 0.2f, 0.0f, 1.0f );
}