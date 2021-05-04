Shader "Custom/Eval14"
{
Properties
{
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
        ZWrite Off
        ZTest LEqual

        CGPROGRAM
        #pragma target 5.0
        #pragma vertex vert
        #pragma fragment frag
        
		#define ID_PER_PRIMITIVE 6

        struct v2f
        {
            float4 vertex : POSITION;
            float2 uv : TEXCOORD0;
        };

        float2 GetCorner(uint index)
        {
        #if 0
            const float2 corners[ID_PER_PRIMITIVE] = { float2(-0.5, -0.5), float2(-0.5, 0.5), float2(0.5, 0.5), float2(0.5, 0.5), float2(0.5, -0.5), float2(-0.5, -0.5) };
            return corners[index % ID_PER_PRIMITIVE];
        #else
            return float2((index >= 2 && index <= 4) ? 0.5 : -0.5, (index >= 1 && index <= 3) ? 0.5 : -0.5);
        #endif
        }

        #define PI 3.141592

        v2f vert (uint id : SV_VertexID)
        {
            v2f o;
            
            const uint quadIndex = id / ID_PER_PRIMITIVE;
            const uint vertexIndex = id % ID_PER_PRIMITIVE;

            const float2 corner = GetCorner(vertexIndex);
            const float3 direction0 = float3(1, 0, 0);
            const float3 direction1 = float3(0, 1, 0);
			const float3 direction2 = float3(0, 0, 1);

            float angle = quadIndex * PI * 0.1f + _Time.y;
            float distance = 1.0f / ( 1 + 0.1 * quadIndex);
            float size = 0.5f / ( 1 + 0.1 * quadIndex);
            
            const float3 vertex = size * corner.x * direction0 +
                                  size * corner.y * direction1 + 
                                  distance * sin(angle) * direction0 + distance * cos(angle) * direction1 + 
								  quadIndex * direction2 * -0.01f;
            
            o.vertex = UnityObjectToClipPos(float4(vertex, 1));
            o.uv = 0.5f + corner;
            return o;
        }
            
        float4 frag (v2f IN) : COLOR
        {
            return float4(IN.uv, 0, 1);
        }
        ENDCG
    }
}
}
