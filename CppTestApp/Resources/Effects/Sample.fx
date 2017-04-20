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
	float4 Position : SV_Position;
	uint dataIndex : GC_DataIndex;
};

////////////////////////////////////////////////////////////////////////
// GSPSInput structure
////////////////////////////////////////////////////////////////////////
struct GSPSInput
{
	float4 Position : SV_Position;
};

//----------------------------------------------------------------------
// Name : VSFunc()
// Desc : Vertex Shader Entry Point
//----------------------------------------------------------------------
GSPSInput VSFunc( VSInput input )
{
	GSPSInput output = (GSPSInput)0;

	// ���̂܂ܑ���
	output.Position = input.Position;
	output.Position.x *= (1 - input.dataIndex * 0.1);

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

	// �����H
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
	// �I�����W�F���o��
	return float4( 1.0f, 0.5f, 0.0f, 1.0f );
}