
void receiverVP(
	float4 position		: POSITION,
	float2 UV		    : TEXCOORD0,
	float3 normal		: NORMAL,

	out float4 outPos			: POSITION,
	out float2 outUV		    : TEXCOORD0,
	out float4 outShadowUV		: TEXCOORD1,

	uniform float4x4 world,
	uniform float4x4 worldViewProj,
	uniform float4x4 texViewProj
#if LINEAR_RANGE
    , uniform float4 shadowDepthRange
#endif
	)
{
	float4 worldPos = mul(world, position);
	outPos = mul(worldViewProj, position);

    outUV = UV;
	// calculate shadow map coords
	outShadowUV = mul(texViewProj, worldPos);
#if LINEAR_RANGE
	// adjust by fixed depth bias, rescale into range
	outShadowUV.z = (outShadowUV.z - shadowDepthRange.x) * shadowDepthRange.w;
#endif
}

