//----------------------------------------------------------------------
// File : Sample.fx
// Desc : Sample Shader
// Copyright(c) Project Asura. All right reserved.
//----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
// VSInput structure
////////////////////////////////////////////////////////////////////////
struct VSInput
{
	float4 Position : POSITION;
};

////////////////////////////////////////////////////////////////////////
// GSPSInput structure
////////////////////////////////////////////////////////////////////////
struct GSPSInput
{
	float4 Position : SV_POSITION;
};

//----------------------------------------------------------------------
// Name : VSFunc()
// Desc : Vertex Shader Entry Point
//----------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
	GSPSInput output = (GSPSInput)0;

	// そのまま送る
	output.Position = input.Position;

	return output;
}

//----------------------------------------------------------------------
// Name : GSFunc()
// Desc : Geometry Shader Entry Point
//----------------------------------------------------------------------
[maxvertexcount(3)]
void GSFunc( triangle GSPSInput input[ 3 ], inout TriangleStream<GSPSInput> stream )
{
	GSPSInput output;

	// 無加工
	for( int i=0; i<3; ++i )
	{
		output.Position = input[ i ].Position;
		stream.Append( output );
	}
	stream.RestartStrip();
}


//----------------------------------------------------------------------
// Name : PSFunc()
// Desc : Pixel Shader Entry Point
//----------------------------------------------------------------------
float4 PSFunc( GSPSInput output ) : SV_Target0
{
	// オレンジ色を出力
	return float4( 1.0f, 0.5f, 0.0f, 1.0f );
}