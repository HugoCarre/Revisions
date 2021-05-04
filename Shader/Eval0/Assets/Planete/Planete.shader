Shader "Custom/planete"
{
Properties
{
	_CollCount("ColumnCount", Int) = 10
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
        
		#define ID_PER_PRIMITIVE 6

		#include "Assets/[Tools]/Shaders/Include/OctwrapUtils.cginc"
		
		uniform uint _CollCount;

        struct v2f
        {
            float4 vertex : POSITION;
            float3 normal : TEXCOORD0;
			float3 worldpos : WORLDPOS;
			uint quadIndex : QUADINDEX;
        };

        uint2 GetCorner(uint index)
        {
            return uint2(index >= 2 && index <= 4, index >= 1 && index <= 3);
        }

        v2f vert (uint id : SV_VertexID)
        {
            v2f o;
            
            const uint quadIndex = id / ID_PER_PRIMITIVE;
            const uint vertexIndex = id % ID_PER_PRIMITIVE;

            const uint2 corner = GetCorner(vertexIndex);

			const uint2 octIntPos = (uint2(quadIndex % _CollCount, quadIndex / _CollCount) + corner);
			const float2 octCoord = octIntPos / (float2)_CollCount;
			const float3 vertex = DecodeOct(octCoord);
			o.worldpos = mul(unity_ObjectToWorld, float4(vertex, 1));;
			o.vertex = UnityObjectToClipPos(float4(vertex, 1));
            o.normal = normalize(mul(unity_ObjectToWorld, float4(vertex, 0)).xyz);
			o.quadIndex = quadIndex;
            return o;
        }
            
        float4 frag (v2f IN) : COLOR
        {
			float3 colorRGB = lerp(IN.normal * 0.5 + 0.5, (IN.quadIndex % 3) / 2.0, 0.2);

			float3 ddxWorldPos = ddx(IN.worldpos);
			float3 ddyWorldPos = ddy(IN.worldpos);
			float3 normal = normalize(cross(ddxWorldPos, ddyWorldPos));

            return float4(normal * 0.5 + 0.5, 1);
        }
        ENDCG
    }
}
}
