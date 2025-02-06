#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main( 
float4 pos : POSITION,float2 uv : TEXCOORD,float3 normal : NORMAL,
float4 color : COLOR,float3 tangent :TANGENT,uint4 skinIndex : SKININDEX,float4 skinWeight : SKINWEIGHT)
{
    // スキンメッシュ処理
    if (g_IsSkinMeshObj)
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
    Out.Pos = mul(pos, g_mWorld);
    Out.wPos = Out.Pos.xyz;
    Out.svPos = mul(Out.Pos, g_mView);
    Out.wvPos = Out.svPos.xyz;
    Out.svPos = mul(Out.svPos, g_mProj);
    
    // UV座標
    Out.UV = uv;
    
    Out.Tangent = tangent;
    
    // 頂点色
    Out.Color = color;
    
    // 法線
    Out.wN = normalize(mul(normal, (float3x3) g_mWorld));
    Out.wT = normalize(mul(tangent, (float3x3) g_mWorld));
    float3 binormal = cross(normal, tangent);
    Out.wB = normalize(mul(binormal, (float3x3)g_mWorld));
    
    return Out;
}

float4 Shadow(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD
, uint4 skinIndex : SKININDEX, float4 skinWeight : SKINWEIGHT) : SV_Position
{
    // スキンメッシュ処理
    if (g_IsSkinMeshObj)
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
    
    pos = mul(pos, g_mWorld);
    
    return mul(pos, g_DL_mVP);
}