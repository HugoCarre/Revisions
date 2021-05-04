Shader "Custom/D20"
{
Properties
{
    _VertexCount("_VertexCount", Int) = 6
    _InstanceCount("_InstanceCount", Int) = 1
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
        Cull Front
        Lighting Off
        ZWrite On
        ZTest LEqual

        CGPROGRAM
        #pragma target 5.0
        #pragma vertex vert
        #pragma fragment frag
        
		#define ID_PER_PRIMITIVE 3

		#include "Assets/[Tools]/Shaders/Include/IcosahedreUtils.cginc"


        uniform uint _VertexCount;
        uniform uint _InstanceCount;

        struct v2f
        {
            float4 vertex : POSITION;
            float3 normal : NORMAL;
			float kill : KILL;
        };

        v2f vert (uint id : SV_VertexID)
        {
            v2f o;
            
            const uint triIndex = id / ID_PER_PRIMITIVE;
            const uint vertexIndex = id % ID_PER_PRIMITIVE;
			const uint instanceIndex = triIndex / 20;

			float3 position;
			float3 sideNormal;

			GetIcosahedreVertexInfo(vertexIndex, triIndex, position, sideNormal);

			float3 sizeFactor = 1 / (1 + instanceIndex * float3(0.1, 0.2, 0.1));
			
			o.kill = 0;

            o.vertex = UnityObjectToClipPos(float4(position, 1));
			o.normal = normalize(mul(unity_ObjectToWorld, float4(sideNormal, 0)).xyz);
			
            return o;
        }
            
        float4 frag (v2f IN) : COLOR
        {
			clip(0.5 - IN.kill);
			float3 color0 = float3(1, 0.5, 0.5) * 0.3;
			float3 color2 = float3(0.5, 0.5, 1) * 0.3;
			float3 color1 = float3(0.5, 1, 0.5) * 0.3;
			float3 colors = 
				max(0, dot(float3(1, 0, 0), IN.normal)) * color0 +
				max(0, dot(float3(0, 1, 0), IN.normal)) * color1 +
				max(0, dot(float3(0, 0, 1), IN.normal)) * color2;
            return float4(saturate(0.1 + colors), 1);
        }
        ENDCG
    }
}
}
