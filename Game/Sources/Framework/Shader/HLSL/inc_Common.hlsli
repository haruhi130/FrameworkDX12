//=====================================================
//
// 全シェーダー共通
//
//=====================================================

// カメラ
cbuffer cbCamera : register(b0)
{
    row_major float4x4 g_mView;
    row_major float4x4 g_mProj;
    row_major float4x4 g_mProjInv;
    float3 g_camPos;
}

// ライト
cbuffer cbLight : register(b5)
{
    // 環境光
    float g_AL_Power;
    
    // 平行光
    float3 g_DL_Dir;
    float3 g_DL_Color;
    
    int g_isUseLight;
}