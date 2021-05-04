Shader "Custom/octshpere"
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
        Cull Front
        Lighting Off
        ZWrite On
        ZTest LEqual

        CGPROGRAM
        #pragma target 5.0
        #pragma vertex vert
        #pragma fragment frag
        
		#define ID_PER_PRIMITIVE 6
		#define ID_PER_CUBE 36

        struct v2f
        {
            float4 vertex : POSITION;
            float3 normal : TEXCOORD0;
        };

        uint2 GetCorner(uint index)
        {
            return uint2(index >= 2 && index <= 4, index >= 1 && index <= 3);
        }

		void GetCubeVertexInfo(uint vertexIndex, uint triIndex, out float3 position, out float3 sidedNormal)
		{
			const uint symetrie = triIndex % 2;
			const uint axeIndex = (triIndex / 2) % 3;
			
			float2 corner = GetCorner(vertexIndex) - 0.5f;
			float3 axis0 = axeIndex.xxx == uint3(0, 1, 2);
			float3 axis1 = axeIndex.xxx == uint3(1, 2, 0);
			float3 axis2 = axeIndex.xxx == uint3(2, 0, 1);

			//handling culling order
			corner.x = symetrie ? corner.x : -corner.x;
			position = (symetrie ? -axis0 : axis0) * 0.5 + corner.x * axis1 + corner.y * axis2;
			sidedNormal = symetrie ? -axis0 : axis0;
		}
		

        v2f vert (uint id : SV_VertexID)
        {
            v2f o;
            
            const uint vertexIndex = id % ID_PER_PRIMITIVE;
            const uint triIndex = id / ID_PER_PRIMITIVE;
			const uint cubeIndex = id / ID_PER_CUBE;

			float3 vertex;
			float3 baseNormal;
			GetCubeVertexInfo(vertexIndex, triIndex, vertex, baseNormal);

			o.vertex = UnityObjectToClipPos(float4(vertex, 1));
            o.normal = normalize(mul(unity_ObjectToWorld, float4(baseNormal, 0)).xyz);
			return o;
        }
            
        float4 frag (v2f IN) : COLOR
        {
			float3 colorRGB = IN.normal * 0.5 + 0.5;
            return float4(colorRGB, 1);
        }
        ENDCG
    }
}
}
