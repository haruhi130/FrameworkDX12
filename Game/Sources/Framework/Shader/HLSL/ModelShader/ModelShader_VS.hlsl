#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

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
    
    // 座標変換
    Out.pos = mul(pos, g_mWorld);
    Out.wPos = Out.pos.xyz;
    Out.pos = mul(Out.pos, g_mView);
    Out.pos = mul(Out.pos, g_mProj);
    
    // UV座標
    Out.uv = uv;
    
    Out.tangent = tangent;
    
    // 頂点色
    Out.color = color;
    
    // 法線
    Out.wN = normalize(mul(normal, (float3x3) g_mWorld));
    Out.wT = normalize(mul(tangent, (float3x3) g_mWorld));
    Out.wB = normalize(cross(Out.wN, Out.wT));
    
    return Out;
}