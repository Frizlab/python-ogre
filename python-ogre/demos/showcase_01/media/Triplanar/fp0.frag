void OverhangTerrain_fp_triplanar_cg (
	float2 uv1 : TEXCOORD0,
	float2 uv2 : TEXCOORD1,
	float3 normal : TEXCOORD2,
	float fog : TEXCOORD3,

    // shadows
    //float4 tangent : TEXCOORD1,
    //float4 binormal : TEXCOORD2,
    //

	out float4 color : COLOR,

    uniform float4 lightPos,
    uniform float4 ambient,
    uniform float4 lightDiffuseColour,

	uniform sampler2D worldTex : register(s0),
	uniform sampler2D detailTex : register(s1),
	uniform sampler2D rock : register(s2)
    //uniform sampler2D normTex: register(s3)
	)
	{
		//taken from Cascades DX10 demo.
		float3 blendWeights = normalize(normal);
		blendWeights = abs(blendWeights);
		blendWeights *= float3(7,7,7);
		blendWeights = pow(blendWeights, float3(3,3,3));
		blendWeights = max(float3(0.0,0.0,0.0), blendWeights);
		//so they sum to 1.0:
		blendWeights /= dot(blendWeights, float3(1.0,1.0,1.0));

		float4 worldColor = tex2D(worldTex, uv1);
		float4 color1 = tex2D(rock, uv2.xy*3);
		float4 color2 = tex2D(rock, uv2.yz*3);
		float4 color3;
		if(normal.y < 0.0)
			color3 = tex2D(rock, uv2.xz*3);
		else
			color3 = tex2D(detailTex, uv2.xz);
//		float4 color1 = float4(1,0,0,1);
//		float4 color2 = float4(0,1,0,1);
//		float4 color3 = float4(0,0,1,1);

        float3 lightDir = 	normalize(
                lightPos.xyz -  (uv2.xyz * lightPos.w));

        float4 lightcolour = ambient + saturate(dot(lightDir, normal)) * lightDiffuseColour;

        //fog = ambient;
		color = color1*blendWeights.z + color2*blendWeights.x + color3*blendWeights.y;
//		color = color1*normal.z + color2*normal.x + color3*normal.y;
		color *= lightcolour;
		color = color * (1-fog) + ambient*fog;
        //color *= 0.8;


        // SHADOWS -------------------------------
        // compute light ray vector in texture space
        //float depth = 1.0;
        //float4 scale= float4(tangent.w, binormal.w, depth, 1.0)
        //float light_depth_bias=(2*lightPos.z-lightPos.z*lightPos.z);
        //lightPos.xy*=light_depth_bias;
        //light.z=-light.z;
        //light*=scale.z/(scale*lightPos.z);

        // compute light ray entry point in texture space
        //float3 lp=uv2-lightPos*uv2.z;
        //ray_intersect_rm_linear(normTex,lp,lightPos);


        //const int linear_search_steps=10;

        //lightPos/=linear_search_steps;

        //for( int i=0;i<linear_search_steps-1;i++ )
        //{
        //    float4 t=tex2D(normTex,lp.xy);
        //    if (lp.z<t.w)
        //        lp+=v;
        //}


        //if (lp.z<uv2.z-0.05) // if pixel in shadow
        //{
        //  color*=0.25;
        //}
        // ------------------------------------

	}
