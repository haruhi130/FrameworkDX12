#pragma once

class Camera
{
public:
	Camera() { SetProjMatrix(60.0f); }
	~Camera() {}

	/// <summary>
	/// カメラをシェーダーにセット
	/// </summary>
	void SetToShader() const;

	/// <summary>
	/// ビュー行列をセット
	/// </summary>
	/// <param name="mView">設定するビュー行列</param>
	inline void SetViewMatrix(const Math::Matrix& mView)
	{ m_mView = mView; }

	/// <summary>
	/// 引数からプロジェクション行列を計算してセット
	/// </summary>
	/// <param name="fov">視野角</param>
	/// <param name="minRange">ニアクリップ</param>
	/// <param name="maxRange">ファークリップ</param>
	/// <param name="aspectRatio">アスペクト比</param>
	void SetProjMatrix(float fov, float minRange = 0.01f, float maxRange = 500.0f, float aspectRatio = 16.0f / 9.0f);
	/// <summary>
	/// プロジェクション行列をセット
	/// </summary>
	/// <param name="mProj">設定するプロジェクション行列</param>
	void SetProjMatrix(const DirectX::SimpleMath::Matrix& mProj);
	
	/// <summary>
	/// クライアント座標から3Dワールド座標を求めるレイ情報を生成
	/// </summary>
	/// <param name="clientPos">マウス座標</param>
	/// <param name="rayPos">レイの位置</param>
	/// <param name="rayDir">レイの方向</param>
	/// <param name="rayRange">レイの長さ</param>
	void GenerateRayInfoFromClient(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) const;

	/// <summary>
	/// ビュー行列を返す
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const Math::Matrix& GetViewMatrix() const 
	{ return m_mView; }

	/// <summary>
	/// プロジェクション行列を返す
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	inline const Math::Matrix& GetProjMatrix() const
	{ return m_mProj; }

private:
	Math::Matrix m_mView;

	Math::Matrix m_mProj;
};