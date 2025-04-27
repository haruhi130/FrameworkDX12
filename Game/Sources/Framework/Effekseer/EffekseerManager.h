#pragma once

class EffekseerObject;

class EffekseerManager
{
public:
	// エフェクト情報
	struct EffectInfo
	{
		std::string FileName = "";
		Math::Vector3 Pos = {};
		float Scale = 1.0f;
		float Speed = 1.0f;
		bool IsLoop = false;
	};

	/// <summary>
	/// Effekseer初期化
	/// </summary>
	/// <param name="w">ウィンドウ横サイズ</param>
	/// <param name="h">ウィンドウ縦サイズ</param>
	/// <returns></returns>
	bool Init(int w,int h);

	
	/// <summary>
	/// Effekseer更新
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクト描画
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトを再生
	/// </summary>
	/// <param name="fileName">エフェクト名</param>
	/// <param name="pos">エフェクト座標</param>
	/// <param name="isLoop">ループ設定</param>
	/// <returns>EffekseerObjectインスタンス</returns>
	std::shared_ptr<EffekseerObject> Play(const std::string& fileName, const Math::Vector3& pos,bool isLoop = true);

	/// <summary>
	/// エフェクトを全て停止
	/// </summary>
	void StopAllEffect();
	
	/// <summary>
	/// エフェクトを停止
	/// </summary>
	/// <param name="name">エフェクト名</param>
	void StopEffect(const std::string& name);

	/// <summary>
	/// エフェクトを一時停止
	/// </summary>
	void OnPauseEfkUpdate()
	{ m_isPause = true; }

	/// <summary>
	/// エフェクトの座標をセット
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="pos">座標</param>
	void SetPos(const std::string& name, const Math::Vector3& pos);
	
	/// <summary>
	/// エフェクトの回転をセット
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度</param>
	void SetRotation(const std::string& name, const Math::Vector3& axis, const float angle);
	
	/// <summary>
	/// エフェクトの行列をセット
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="mWorld">行列</param>
	void SetMatrix(const std::string& name, const Math::Matrix& mWorld);

	/// <summary>
	/// エフェクトの拡縮をセット
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="scale">拡縮数値</param>
	void SetScale(const std::string& name, const Math::Vector3& scale);
	void SetScale(const std::string& name, const float scale);

	/// <summary>
	/// エフェクトの再生速度をセット
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="speed">再生速度</param>
	void SetSpeed(const std::string& name, const float speed = 1.0f);

	/// <summary>
	/// エフェクトを一時停止
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <param name="isPause">有効化</param>
	void SetPause(const std::string& name, const bool isPause);

	/// <summary>
	/// エフェクトが再生中か判別
	/// </summary>
	/// <param name="name">エフェクト名</param>
	/// <returns>再生中ならtrue</returns>
	const bool IsPlaying(const std::string& name) const;

	/// <summary>
	/// カメラ情報をセット
	/// </summary>
	/// <param name="spCamera">カメラのスマートポインタ</param>
	void SetCamera(const std::shared_ptr<Camera>& spCamera) 
	{ m_wpCamera = spCamera; }
	
	/// <summary>
	/// 再生中のエフェクトを全て停止しエフェクトリストをクリア
	/// </summary>
	void Release();

private:
	/// <summary>
	/// Math::Vector3をEffekseer::Vector3Dに変換
	/// </summary>
	/// <param name="vec">Math::Vector3</param>
	/// <returns>Effekseer::Vector3D</returns>
	const Effekseer::Vector3D ConvertToEfkVec3D(const Math::Vector3& vec) const
	{ return Effekseer::Vector3D(vec.x, vec.y, vec.z); }

	/// <summary>
	/// エフェクト再生
	/// </summary>
	/// <param name="info">エフェクト情報</param>
	/// <returns>EffekseerObjectインスタンス</returns>
	std::shared_ptr<EffekseerObject> Play(const EffectInfo& info);
	std::shared_ptr<EffekseerObject> Play(const std::shared_ptr<EffekseerObject> spEfkObj);

	/// <summary>
	/// エフェクト更新
	/// </summary>
	void UpdateEffect();

	/// <summary>
	/// Effekseerで使用するカメラ更新
	/// </summary>
	void UpdateEfkCameraMatrix();

	// エフェクトレンダラー
	EffekseerRenderer::RendererRef m_pEfkRenderer = nullptr;

	// エフェクトマネージャー
	Effekseer::ManagerRef m_pEfkManager = nullptr;

	// メモリプール
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_pEfkMemoryPool = nullptr;
	// コマンドリスト
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_pEfkCmdList = nullptr;

	// エフェクトリスト
	std::unordered_map<std::string, std::shared_ptr<EffekseerObject>> m_omEffects;

	// カメラ情報
	std::weak_ptr<Camera> m_wpCamera;

	// 一時停止中か
	bool m_isPause = false;

	EffekseerManager(){}
	~EffekseerManager() { Release(); }
public:
	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}
};

class EffekseerObject
{
public:
	/// <summary>
	/// エフェクトが再生中か
	/// </summary>
	/// <returns>再生中ならtrue</returns>
	bool IsPlaying();

	/// <summary>
	/// EffekseerManagerをセット
	/// </summary>
	/// <param name="parentManager">EffekseerManager</param>
	void SetParentManager(Effekseer::ManagerRef& parentManager)
	{ m_parentManager = parentManager; }

	/// <summary>
	/// エフェクトをセット
	/// </summary>
	/// <param name="effect">エフェクト本体</param>
	void SetEffect(Effekseer::EffectRef& effect) { m_effect = effect; }

	/// <summary>
	/// エフェクトハンドルをセット
	/// </summary>
	/// <param name="handle">エフェクトハンドル</param>
	void SetHandle(Effekseer::Handle& handle) { m_handle = handle; }

	/// <summary>
	/// エフェクトの座標をセット
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// エフェクトの拡縮をセット
	/// </summary>
	/// <param name="scale">拡縮数値</param>
	void SetScale(const float scale = 1.0f);

	/// <summary>
	/// エフェクトの再生速度をセット
	/// </summary>
	/// <param name="speed">再生速度</param>
	void SetSpeed(const float speed = 1.0f);

	/// <summary>
	/// エフェクトの行列をセット
	/// </summary>
	/// <param name="mWorld">行列</param>
	void SetMatrix(const Math::Matrix& mWorld) const;

	/// <summary>
	/// エフェクトのループ設定
	/// </summary>
	/// <param name="isLoop">ループ設定</param>
	void SetLoop(const bool isLoop = false) 
	{ m_info.IsLoop = isLoop; }

	/// <summary>
	/// エフェクト情報を一括設定
	/// </summary>
	/// <param name="info">エフェクト情報</param>
	void SetEffectInfo(const EffekseerManager::EffectInfo& info) { m_info = info; }

	/// <summary>
	/// エフェクトを取得
	/// </summary>
	/// <returns>エフェクト本体</returns>
	const Effekseer::EffectRef& GetEffect() const { return m_effect; }
	
	/// <summary>
	/// エフェクトハンドルを取得
	/// </summary>
	/// <returns>エフェクトハンドル</returns>
	const Effekseer::Handle& GetHandle() const { return m_handle; }
	
	/// <summary>
	/// エフェクト情報を全て取得
	/// </summary>
	/// <returns>エフェクト情報</returns>
	const EffekseerManager::EffectInfo& GetEffectInfo() const { return m_info; }
	EffekseerManager::EffectInfo& WorkEffectInfo() { return m_info; }

	/// <summary>
	/// エフェクトの情報を一部取得
	/// </summary>
	/// <returns>一部のエフェクト情報</returns>
	const Math::Vector3& GetPos() const { return m_info.Pos; }
	const float GetSize() const { return m_info.Scale; }
	const float GetSpeed() const { return m_info.Speed; }
	const bool IsLoop() const { return m_info.IsLoop; }
	
private:
	// エフェクトマネージャー
	Effekseer::ManagerRef m_parentManager = nullptr;
	// エフェクト本体
	Effekseer::EffectRef m_effect = nullptr;
	// エフェクトハンドル
	Effekseer::Handle m_handle = -1;

	// エフェクト情報
	EffekseerManager::EffectInfo m_info = {};
};

// エフェクトファイルパス
constexpr auto EffectPath = "Assets/Effects/";