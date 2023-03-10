cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 CameraPosition;
	float4 Time;
}

cbuffer ConstantBufferUniform : register (b1)
{
	float4 LightPosition[5];
	float4 LightColour[5];
	uint4 NumberOfLights;
}

TextureCube txDiffuse : register(t0);

SamplerState txSampler : register(s0);

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEXCOORD;
	float3 instancePos : INSTANCEPOS;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 TexCoord : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.Pos = float4(input.Pos, 1.0f) + CameraPosition;
	output.Pos.w = 1.0f;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection).xyww;
	output.TexCoord = input.Pos;

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	return txDiffuse.Sample(txSampler, input.TexCoord);
}