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

	// 初期化
	bool Init(int w,int h);

	// 更新
	void Update();

	// 描画
	void Draw();

	// エフェクトを再生しインスタンスを返す
	std::shared_ptr<EffekseerObject> Play(const std::string& fileName, const Math::Vector3& pos,bool isLoop = true);

	// 全てのエフェクトを停止
	void StopAllEffect();
	// 指定したエフェクトを停止
	void StopEffect(const std::string& name);

	// 一時停止
	void OnPauseEfkUpdate()
	{ m_isPause = true; }

	// エフェクト情報設定
	void SetPos(const std::string& name, const Math::Vector3& pos);
	void SetRotation(const std::string& name, const Math::Vector3& axis, const float angle);
	void SetMatrix(const std::string& name, const Math::Matrix& mWorld);

	void SetScale(const std::string& name, const Math::Vector3& scale);
	void SetScale(const std::string& name, const float scale);

	void SetSpeed(const std::string& name, const float speed = 1.0f);

	void SetPause(const std::string& name, const bool isPause);

	// 再生中か
	const bool IsPlaying(const std::string& name) const;

	// カメラ設定
	void SetCamera(const std::shared_ptr<Camera>& camera) 
	{ m_wpCamera = camera; }
	
	// 解放
	void Reset();
	void Release();

private:
	// Math::Vector3をEffekseer::Vector3Dに変換
	const Effekseer::Vector3D ConvertToEfkVec3D(const Math::Vector3& vec) const
	{
		return Effekseer::Vector3D(vec.x, vec.y, vec.z);
	}

	// エフェクト再生
	std::shared_ptr<EffekseerObject> Play(const EffectInfo& info);
	std::shared_ptr<EffekseerObject> Play(const std::shared_ptr<EffekseerObject> spEfkObj);

	// エフェクト更新
	void UpdateEffect();
	void UpdateEfkCameraMatrix();

	// エフェクトレンダラー
	EffekseerRenderer::RendererRef m_pEfkRenderer = nullptr;

	// エフェクトマネージャー
	Effekseer::ManagerRef m_pEfkManager = nullptr;

	/// <summary>
	/// コマンドリストを使用するライブラリ用
	/// </summary>
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
	// 再生中か
	bool IsPlaying();

	// 登録したマネージャー設定
	void SetParentManager(Effekseer::ManagerRef& parentManager)
	{ m_parentManager = parentManager; }

	// エフェクト設定
	void SetEffect(Effekseer::EffectRef& effect) { m_effect = effect; }

	// エフェクトハンドル設定
	void SetHandle(Effekseer::Handle& handle) { m_handle = handle; }

	// エフェクト情報設定
	void SetPos(const Math::Vector3& pos);
	void SetScale(const float scale = 1.0f);
	void SetSpeed(const float speed = 1.0f);
	void SetMatrix(const Math::Matrix& mWorld) const;
	void SetLoop(const bool isLoop = false) 
	{ m_info.IsLoop = isLoop; }
	// エフェクト情報一括設定
	void SetEffectInfo(const EffekseerManager::EffectInfo& info) { m_info = info; }

	// エフェクト本体取得
	const Effekseer::EffectRef& GetEffect() const { return m_effect; }
	// エフェクトハンドル取得
	const Effekseer::Handle& GetHandle() const { return m_handle; }
	
	// エフェクト情報取得
	const EffekseerManager::EffectInfo& GetEffectInfo() const { return m_info; }
	EffekseerManager::EffectInfo& WorkEffectInfo() { return m_info; }

	// エフェクト各種単一情報取得
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

constexpr auto EffectPath = "Assets/Effects/";