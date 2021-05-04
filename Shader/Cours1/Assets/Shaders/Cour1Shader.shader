
Shader "Custom/Cours1"
{
Properties
{
    _GridTex("Grid", 2D) = "white" {}
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
        
		#define ID_PER_PRIMITIVE 6

        uniform Texture2D _GridTex;
        uniform SamplerState linear_mirror_sampler;

        struct v2f
        {
            float4 vertex : POSITION;
            float2 uv : TEXCOORD0;
            float3 normal : NORMAL;

            float debugChan : DEBUG_CHAN;
        };

        uniform uint _VertexCount;
        uniform uint _InstanceCount;

        float2 GetCorner(uint index)
        {
        #if 0
            const float2 corners[ID_PER_PRIMITIVE] = { float2(-0.5, -0.5), float2(-0.5, 0.5), float2(0.5, 0.5), float2(0.5, 0.5), float2(0.5, -0.5), float2(-0.5, -0.5) };
            return corners[index % ID_PER_PRIMITIVE];
        #else
            return float2((index >= 2 && index <= 4) ? 0.5 : -0.5, (index >= 1 && index <= 3) ? 0.5 : -0.5);
        #endif
        }

        float functionToPlot(float2 abcissa) {
            float lengthAbcissa = length(abcissa) + abs(cos(_Time.y)*10);
            return 10 * (lengthAbcissa < 0.0000001f ? 1 :sin(lengthAbcissa) / lengthAbcissa);
        }


        v2f vert (uint id : SV_VertexID)
        {
            v2f o;
            
            const uint quadIndex = id / ID_PER_PRIMITIVE;
            const uint vertexIndex = id % ID_PER_PRIMITIVE;

            const uint gridCount = sqrt(_InstanceCount);
            const float gridSize = 10.0f / gridCount;
            const uint lineIndex = (uint)(quadIndex / gridCount);
            const uint colIndex = (uint)quadIndex % gridCount;

            const float2 corner = GetCorner(vertexIndex);
            const float3 direction0 = float3(1, 0, 0);
            const float3 direction1 = float3(0, 1, 0);
			const float3 direction2 = float3(0, 0, 1);

            //Cell position
            float3 localVertex = corner.x * direction0 +
                                 corner.y * direction1;

            float3 quadPos = direction0 * colIndex + direction1 * lineIndex;
            float3 vertex = localVertex + quadPos;
            vertex *= gridSize;

            //Deformation de la grille
            float multiplier = 5;
            float2 abcissa = (vertex.xy - 5) * multiplier;
           
            float deformation = functionToPlot(abcissa);
            vertex += deformation * direction2;

            //Calcul Normals
            float epsilon = 0.01f;
            float dfdx = 0;
            {
                float2 epsilonX = float2(epsilon,0);
                dfdx =( functionToPlot(abcissa + epsilonX) - functionToPlot(abcissa - epsilonX)) / (2 * epsilon);
            }

            float dfdy = 0;
            {
                float2 epsilonY = float2(0, epsilon);
                dfdy = (functionToPlot(abcissa + epsilonY) - functionToPlot(abcissa - epsilonY)) / (2 * epsilon);
            }

            float3 normal = normalize(float3(-dfdx,-dfdy,1));

            o.vertex = UnityObjectToClipPos(float4(vertex, 1));
            o.uv = 0.5f + corner;
            o.normal = mul(unity_ObjectToWorld, float4(normal,0)).xyz;
            o.debugChan = dfdx + dfdy;
            return o;
        }
            
        float4 frag (v2f IN) : COLOR
        {   
            float4 tex = _GridTex.Sample(linear_mirror_sampler, IN.uv);
            return tex * float4(IN.normal, 1);
        }
        ENDCG
    }
}
}
