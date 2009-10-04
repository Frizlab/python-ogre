// Expand a range-compressed vector
float3 expand(float3 v)
{
	return (v - 0.5) * 2;
}

void receiverFPBasic(
	float4 position			: POSITION,
	float2  UV			    : TEXCOORD0,
	float4 shadowUV			: TEXCOORD1,

	uniform sampler2D normalMap : register(s0),
	uniform sampler2D shadowMap : register(s1),
    
    uniform float4 lightAmbient,
    
    uniform float4 lightPosition, // object space
    uniform float4 lightDiffuse,
    uniform float4 lightSpecular,
    
	out float4 result		: COLOR)
{

    // Angle between normal and light vector
    //float l_CosNL = saturate(dot(l_Normal, l_LightVector));
    float DirectLightIntensity = saturate(dot(expand(tex2D(normalMap, UV).xzy), lightPosition.xyz));
            
    if (DirectLightIntensity == 0)
    {
        // no light hitting this surface.
        // No light can reach back surfaces...
        result = lightAmbient;
    }
    else
   {
    	// Pre-raise the specular exponent to the eight power
    	// Note we have no 'pow' function in basic fragment programs, if we were willing to accept compatibility
    	// with ps_2_0 / arbfp1 and above, we could have a variable shininess parameter
    	// This is equivalent to 
    	float specFactor = DirectLightIntensity;
    	for (int i = 0; i < 3; ++i)
    		specFactor *= DirectLightIntensity;
    	

    	// Calculate dot product for diffuse
    	float3 lightVal = (lightDiffuse * saturate(DirectLightIntensity)) + 
    			(lightSpecular * specFactor);
            
            
            
        // point on shadowmap
        shadowUV = shadowUV / shadowUV.w;
        float4 depthShadow = tex2D(shadowMap, shadowUV.xy);
        float centerdepth = depthShadow.x;
        
        if (depthShadow.z > 0)
        {
           //result = (centerdepth > shadowUV.z) ? lightAmbient +  float4(lightVal*centerdepth, 1) : float4(0,0,0,1);
           result = lightAmbient +  float4(lightVal * centerdepth, 1);
        }
        else
        {
            result = lightAmbient;
        }
    }
}


void receiverFP(
	float4 position			: POSITION,
	float2  UV			    : TEXCOORD0,
	float4 shadowUV			: TEXCOORD1,

	uniform sampler2D normalMap : register(s0),
	uniform sampler2D shadowMap : register(s1),
    
    uniform float4 lightAmbient,
    
    uniform float4 lightPosition, // object space
    uniform float4 lightDiffuse,
    uniform float4 lightSpecular,
    
	uniform float inverseShadowmapSize,
	uniform float fixedDepthBias,
	uniform float gradientClamp,
	uniform float gradientScaleBias,
#if FUZZY_TEST
    uniform float shadowFuzzyWidth,
#endif
	out float4 result		: COLOR)
{

    float3 DirectLightIntensity = dot(expand(tex2D(normalMap, UV).xzy), lightPosition.xyz);
    
	// Pre-raise the specular exponent to the eight power
	// Note we have no 'pow' function in basic fragment programs, if we were willing to accept compatibility
	// with ps_2_0 / arbfp1 and above, we could have a variable shininess parameter
	// This is equivalent to 
	float specFactor = DirectLightIntensity;
	for (int i = 0; i < 3; ++i)
		specFactor *= DirectLightIntensity;
	

	// Calculate dot product for diffuse
	float3 lightVal = (lightDiffuse * saturate(DirectLightIntensity)) + 
			(lightSpecular * specFactor);
            
    // no light hitting this surface.
   
    if (dot(lightVal, float3(1, 1, 1)) == 0)
    {
       result = lightAmbient;
    }
   else
   {   
        // point on shadowmap
#if LINEAR_RANGE
        shadowUV.xy = shadowUV.xy / shadowUV.w;
#else
        shadowUV = shadowUV / shadowUV.w;
#endif
        float4 depthShadow = tex2D(shadowMap, shadowUV.xy);
        float centerdepth = depthShadow.x;
        
        if (depthShadow.z > 0)
        {
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
            
#if FUZZY_TEST
        	// fuzzy test - introduces some ghosting in result and doesn't appear to be needed?
        	//float visibility = saturate(1 + delta_z / (gradient * shadowFuzzyWidth));
        	float visibility = saturate(1 + (finalCenterDepth - shadowUV.z) * shadowFuzzyWidth * shadowUV.w);

        	result = lightAmbient + float4(lightVal.xyz,1) * visibility;
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

        	result = lightAmbient +  float4(lightVal * final, 1);
        	
#else
        	result = lightAmbient + (finalCenterDepth > shadowUV.z) ? float4(lightVal.xyz,1) : float4(0,0,0,1);
#endif

#endif
        }
        else
        {
            result = lightAmbient;
        }
    }
   
}
