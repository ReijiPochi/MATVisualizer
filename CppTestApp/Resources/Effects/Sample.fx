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
	float3 Position : Position;
	uint dataIndex : GC_DataIndex;
};

struct GSPSInput
{
	float4 Position : SV_Position;
	// ‚È‚º‚© int,uint ‚¾‚Æ‚¤‚Ü‚­‚¢‚©‚È‚¢
	half dataIndex : GC_DataIndex;
};
//{ { 
//{M11:0.7983365 M12 : -0.03780428 M13 : -0.1081802 M14 : -0.1081693}
//{M21:0 M22 : 1.387214 M23 : -0.2377264 M24 : -0.2377026}
//{M31:-0.08946014 M32 : -0.337363 M33 : -0.9653928 M34 : -0.9652963}
//{M41:-0.06820931 M42 : -1.092937 M43 : 3.924931 M44 : 4.024538} }}

GSPSInput VSFunc( VSInput input )
{
	GSPSInput output = (GSPSInput)0;

	float4x4 cam = 
	{
		0.8f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.4f, 0.0f, 0.0f,
		0.0f, -0.3f, -1.0f, -1.0f,
		0.0f, -1.0f, 3.9f, 4.0f
	};
	float4 pos = float4(input.Position, 1.0f);

	output.Position = pos;
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