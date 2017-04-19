
struct VSInput
{
	float4 Position : POSITION;
};

struct GSPSInput
{
	float4 Position : SV_POSITION;
};

GSPSInput VSFunc( VSInput input )
{
	GSPSInput output = (GSPSInput)0;

	output.Position = input.Position;

	return output;
}