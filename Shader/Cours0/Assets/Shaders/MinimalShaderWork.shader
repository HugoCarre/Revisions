Shader "Custom/MinimalShaderWork"
{
    Properties
    {
		_MainTex("Main tex", 2D) = "white" {}
	}

		SubShader
	{
		Tags
		{
			"RenderType" = "Transparent"
			"Queue" = "Transparent+0"
		}

		Pass
		{
			Blend One OneMinusSrcAlpha
			Cull Off
			Lighting Off
			ZWrite On
			ZTest LEqual

			CGPROGRAM

			#pragma target 5.0
			#pragma vertex vert
			#pragma fragment frag

			uniform texture2D _MainTex;
			uniform SamplerState linear_clamp_sampler;

			uniform SamplerState linear_repeat_sampler;

            struct appdata
            {
                float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float4 vertex : POSITION;
				float2 uv : TEXCOORD0;

            };

            v2f vert (appdata v)
            {
                v2f o;

				float3 vertex = v.vertex.xyz;
				o.vertex = UnityObjectToClipPos(vertex);
				//o.uv = v.uv*0.5 + float2(0.25,0.25); //-------- Modify UVs (center of texture, part of texture)---------
				/*float2x2 rot = float2x2(cos(_Time.y), sin(_Time.y), -sin(_Time.y), cos(_Time.y)); //ROTATION
				o.uv = mul(rot,v.uv);*/
				o.uv = v.uv;
                return o;
            } 

			float4 frag(v2f IN) : COLOR
			{
				//return float4(IN.uv,0,1); //------- show UVs --------
				//+float2(_Time.w, _Time.w) add to uv to offset textures
				
				float4 tex = _MainTex.Sample(linear_repeat_sampler, IN.uv );
				return tex;
            }
			
            ENDCG
        }
    }
}
