
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
	//, uniform float4 shadowDepthRange
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


// Expand a range-compressed vector
float3 expand(float3 v)
{
	return (v - 0.5) * 2;
}

    
void receiverFP(
	float4 position			: POSITION,
	float2  UV			    : TEXCOORD0,
	float4 shadowUV			: TEXCOORD1,

	uniform sampler2D normalMap : register(s0),
	uniform sampler2D shadowMap : register(s1),
    
    uniform float4 lightPosition, // object space 
	uniform float inverseShadowmapSize,
	uniform float fixedDepthBias,
	uniform float gradientClamp,
	uniform float gradientScaleBias,
	uniform float shadowFuzzyWidth,
	
	out float4 result		: COLOR)
{
	// point on shadowmap
#if LINEAR_RANGE
	shadowUV.xy = shadowUV.xy / shadowUV.w;
#else
	shadowUV = shadowUV / shadowUV.w;
#endif
	float centerdepth = tex2D(shadowMap, shadowUV.xy).x;
    
    // gradient calculation
  	float pixeloffset = inverseShadowmapSize;
    float4 depths = float4(
    	tex2D(shadowMap, shadowUV.xy + float2(-pixeloffset, 0)).x,
    	tex2D(shadowMap, shadowUV.xy + float2(+pixeloffset, 0)).x,
    	tex2D(shadowMap, shadowUV.xy + float2(0, -pixeloffset)).x,
    	tex2D(shadowMap, shadowUV.xy + float2(0, +pixeloffset)).x);

	float2 differences = abs( depths.yw - depths.xz );
	float gradient = min(gradientClamp, max(differences.x, differences.y));
	float gradientFactor = gradient * gradientScaleBias;

	// visibility function
	float depthAdjust = gradientFactor + (fixedDepthBias * centerdepth);
	float finalCenterDepth = centerdepth + depthAdjust;

	// shadowUV.z contains lightspace position of current object
float3 normal = float3(1, 1, 1);
    //float3 normal = saturate(dot(expand(tex2D(normalMap, UV).xzy), lightPosition.xyz));
    
#if FUZZY_TEST
	// fuzzy test - introduces some ghosting in result and doesn't appear to be needed?
	//float visibility = saturate(1 + delta_z / (gradient * shadowFuzzyWidth));
	float visibility = saturate(1 + (finalCenterDepth - shadowUV.z) * shadowFuzzyWidth * shadowUV.w);

	result = vertexColour * visibility;
#else
	// hard test
#if PCF
	// use depths from prev, calculate diff
	depths += depthAdjust.xxxx;
	float final = (finalCenterDepth > shadowUV.z) ? 1.0f : 0.0f;
	final += (depths.x > shadowUV.z) ? 1.0f : 0.0f;
	final += (depths.y > shadowUV.z) ? 1.0f : 0.0f;
	final += (depths.z > shadowUV.z) ? 1.0f : 0.0f;
	final += (depths.w > shadowUV.z) ? 1.0f : 0.0f;
	
	final *= 0.2f;

	result = float4(normal * final, 1);
	
#else
	result = (finalCenterDepth > shadowUV.z) ? normal : float4(0,0,0,1);
#endif

#endif
   
}
