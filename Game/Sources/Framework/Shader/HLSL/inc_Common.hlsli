//=====================================================
//
// 全シェーダ共通
//
//=====================================================

cbuffer cbCamera : register(b0)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
}