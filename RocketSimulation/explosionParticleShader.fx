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

Texture2D txDiffuse : register(t0);

SamplerState txSampler : register(s0);

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
	float2 TexCoord : TEXCOORD;
	float3 InstancePos : INSTANCEPOS;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal: NORMAL;
	float4 PosWorld : TEXCOORD0;
	float2 TexCoord : TEXCOORD1;
	float FadeRate : TEXCOORD2;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	matrix viewInv = transpose(View);
	float3 vx = viewInv[0].xyz;
	float3 vy = viewInv[1].xyz;
	output.Pos = float4((input.Pos.x * vx + input.Pos.y * vy), 1);

	output.Pos = mul(output.Pos, World);

	float lifecycle = 15;
	float freq = 1;
	float T = fmod(freq * Time.x + input.InstancePos.z/500, lifecycle);
	output.FadeRate = 1.0 - T / lifecycle;

	float height = 3;
	float spread = 0.3;
	output.Pos.xyz += float3(spread * cos(input.InstancePos.z * 300), height * sin(output.FadeRate), spread * sin(input.InstancePos.z * 300))*T;

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Normal = normalize(float4(input.Normal, 1.0f)).xyz;
	output.PosWorld = mul(output.Pos, World);
	output.TexCoord = input.TexCoord;
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float radius = 0.4;
	float range = radius * radius - dot(input.TexCoord - 0.5, input.TexCoord - 0.5);
	float shade = 2 / (1 + exp(12 * range));
	float4 texColour = txDiffuse.Sample(txSampler, input.TexCoord);
	texColour.a = saturate(texColour.r + texColour.g + texColour.b);
	return (texColour * (1 - shade) * input.FadeRate);
}