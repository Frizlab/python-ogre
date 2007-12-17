void OverhangTerrain_vp_linear_triplanar_cg(
    float4 position : POSITION,
    float delta     : BLENDWEIGHT,
    float3 normal	: NORMAL,

    out float4 oPosition : POSITION,
    out float2 oUv1		 : TEXCOORD0,
    out float2 oUv2		 : TEXCOORD1,
    out float4 colour    : COLOR,
    out float fog		 : TEXCOORD3,
    out float3 oNormal	 : TEXCOORD2,

    uniform float4x4 worldViewProj,
    uniform float4 lightPos,
    uniform float4 ambient,
    uniform float4 lightDiffuseColour,
    uniform float4x4 world,
    uniform float morphFactor
    )
{
    float3 protoUv = mul(world, position);
    // Main texture coords
    oUv1 =  (protoUv / 1500).xz;
    // Detail texture coords
    oUv2 =  protoUv / 62.5;
    // Apply morph
    position.y = position.y + (delta.x * morphFactor);
    // world / view / projection
    oPosition = mul(worldViewProj, position);
    // Full bright (no lighting)
    //colour = float4(0.5,0.5,0.5,1);
    // Fog
    // f = end - camz / end - start
    // when start / end has been set, fog value is distance
    fog = oPosition.z / 1500;
    oNormal = normal;

    float3 lightDir = 	normalize(
                lightPos.xyz -  (position.xyz * lightPos.w));

	//colour = ambient + saturate(dot(lightDir, normal)) * lightDiffuseColour;
    colour = float4(0.0, 0.0, 0.0, 1.0);
}