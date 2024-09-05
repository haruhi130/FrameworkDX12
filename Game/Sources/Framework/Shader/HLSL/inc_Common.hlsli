//=====================================================
//
// 全シェーダ共通
//
//=====================================================

// カメラ用レジスター
cbuffer cbCamera : register(b0)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
    row_major matrix g_mProjInv;
    float3 g_camPos;
}