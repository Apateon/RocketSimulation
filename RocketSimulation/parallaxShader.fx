cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 CameraPosition;
}

cbuffer ConstantBufferUniform : register (b1)
{
	float4 LightPosition[5];
	float4 LightColour[5];
	uint4 NumberOfLights;
}

Texture2D txDiffuse : register(t0);
Texture2D txBump : register(t1);
Texture2D txHeight : register(t2);

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
	float4 PosWorld : TEXCOORD0;
	float2 TexCoord : TEXCOORD1;
	float3x3 TBN : TEXCOORD2;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(float4(input.Pos, 1.0f), World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.PosWorld = mul(float4(input.Pos, 1.0f), World);
	output.TexCoord = input.TexCoord;

	float3 norm = normalize(float4(input.Normal, 1.0f)).xyz;
	float3 tan = normalize(mul(float4(input.Tangent, 1.0f),World)).xyz;
	float3 binorm = normalize(mul(float4(input.Binormal, 1.0f), World)).xyz;

	output.TBN = float3x3(
		tan,
		binorm,
		norm
		);

	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float4 matDiffuse = float4(0.8, 0.8, 0.8, 1.0);
	float4 matSpec = float4(1.0, 1.0, 1.0, 1.0);
	float4 ambient = float4(0.1, 0.2, 0.2, 1.0);

	float mappingScale = 0.075f;
	float bias0 = 0.02f;

	float4 texColour = txDiffuse.Sample(txSampler, input.TexCoord);
	float height = txHeight.Sample(txSampler, input.TexCoord).r;
	height = mappingScale * height - bias0;

	float4 light = float4(0, 0, 0, 1);
	float3 viewDir = normalize(CameraPosition.xyz - input.PosWorld.xyz);
	float3 viewDirTangentSpace = normalize(mul(input.TBN, viewDir));
	float2 texCorrected = input.TexCoord + (height * viewDirTangentSpace.xy);
	float3 n = normalize(2 * txBump.Sample(txSampler, texCorrected) - 1.0);

	for (uint i = 0; i < NumberOfLights.x; ++i)
	{
		float3 lightDir = normalize(LightPosition[i].xyz - input.PosWorld.xyz);
		float3 lightDirTangentSpace = normalize(mul(input.TBN, lightDir));

		float diffuse = max(0.0, dot(lightDirTangentSpace, n));

		float3 R = normalize(reflect(-lightDirTangentSpace, n));

		float spec = pow(max(0.0, dot(viewDirTangentSpace, R)), 50);

		light += saturate((ambient + (matDiffuse*diffuse) + (matSpec*spec)) * LightColour[i]);
	}

	return texColour * light;
}