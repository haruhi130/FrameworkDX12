#include "inc_SimpleShader.hlsli"
#include "inc_Common.hlsli"

VSOutput main( 
float4 pos : POSITION,float2 uv : TEXCOORD,float3 normal : NORMAL,
float4 color : COLOR,float3 tangent :TANGENT,uint4 skinIndex : SKININDEX,float4 skinWeight : SKINWEIGHT)
{
    // スキンメッシュ処理
    if(g_isSkinMeshObj)
    {
        row_major float4x4 mBones = 0;
        [unroll]
        for (int i = 0; i < 4; i++)
        {
            mBones += g_mBones[skinIndex[i]] * skinWeight[i];
        }
    
        pos = mul(pos, mBones);
        normal = mul(normal, (float3x3) mBones);
    }
    
    VSOutput Out;
    Out.pos = mul(pos, g_mWorld);
    Out.pos = mul(Out.pos, g_mView);
    Out.pos = mul(Out.pos, g_mProj);
    Out.uv = uv;
    Out.color = color;
    Out.normal = normal;
    Out.tangent = tangent;
	return Out;
}