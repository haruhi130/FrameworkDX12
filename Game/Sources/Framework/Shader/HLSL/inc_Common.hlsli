//=====================================================
//
// 全シェーダーに共通で渡す情報
//
//=====================================================

// カメラ
cbuffer cbCamera : register(b0)
{
    row_major float4x4 g_mView;     // ビュー変換行列
    row_major float4x4 g_mProj;     // 射影変換行列
    row_major float4x4 g_mProjInv;  // 射影変換逆行列
    float3             g_camPos;    // カメラ座標
}

// ライト
cbuffer cbLight : register(b5)
{
    // 環境光
    float   g_AL_Power;
    
    // 平行光
    float3  g_DL_Dir;    // 光の方向
    float3  g_DL_Color;  // 光の色
    
    // ライトの使用判別
    int     g_isUseLight;
}