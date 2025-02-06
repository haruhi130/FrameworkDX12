//=====================================================
//
// シェーダーに共通で渡す情報
//
//=====================================================

// カメラ
cbuffer cbCamera : register(b4)
{
    row_major float4x4 g_mView;     // ビュー変換行列
    row_major float4x4 g_mProj;     // 射影変換行列
    row_major float4x4 g_mProjInv;  // 射影変換逆行列
    float3             g_CamPos;    // カメラ座標
}

struct PointLightInfo
{
    float3 Color;
    float Radius;
    float3 Pos;
    float IsBright;
};

struct SpotLightInfo
{
    int IsEnable;
    float3 Color;
    float Range;
    float3 Pos;
    float Angle;
    float3 Dir;
};

// ライト
cbuffer cbLight : register(b5)
{
    // ライトの使用判別
    int g_IsUseLight;
    
    // 平行光
    float3  g_DL_Dir;    // 光の方向
    float3  g_DL_Color;  // 光の色
    
    // 環境光
    float g_AL_Power;
    
    row_major float4x4 g_DL_mVP;
    
    //-------------------------------
    // 点光
    //-------------------------------
    int4 g_PointLightNum;
    
    PointLightInfo g_PointLights[100];
    
    //-------------------------------
    // 集中光
    //-------------------------------
    int4 g_SpotLightNum;
    
    SpotLightInfo g_SpotLights[100];
}