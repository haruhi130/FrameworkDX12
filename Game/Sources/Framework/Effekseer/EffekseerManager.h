#pragma once

class EffekseerObject;

class EffekseerManager
{
public:
	// �G�t�F�N�g���
	struct EffectInfo
	{
		std::string FileName = "";
		Math::Vector3 Pos = {};
		float Scale = 1.0f;
		float Speed = 1.0f;
		bool IsLoop = false;
	};

	// ������
	bool Init(int w,int h);

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �G�t�F�N�g���Đ����C���X�^���X��Ԃ�
	std::shared_ptr<EffekseerObject> Play(const std::string& fileName, const Math::Vector3& pos,bool isLoop = true);

	// �S�ẴG�t�F�N�g���~
	void StopAllEffect();
	// �w�肵���G�t�F�N�g���~
	void StopEffect(const std::string& name);

	// �ꎞ��~
	void OnPauseEfkUpdate()
	{ m_isPause = true; }

	// �G�t�F�N�g���ݒ�
	void SetPos(const std::string& name, const Math::Vector3& pos);
	void SetRotation(const std::string& name, const Math::Vector3& axis, const float angle);
	void SetMatrix(const std::string& name, const Math::Matrix& mWorld);

	void SetScale(const std::string& name, const Math::Vector3& scale);
	void SetScale(const std::string& name, const float scale);

	void SetSpeed(const std::string& name, const float speed = 1.0f);

	void SetPause(const std::string& name, const bool isPause);

	// �Đ�����
	const bool IsPlaying(const std::string& name) const;

	// �J�����ݒ�
	void SetCamera(const std::shared_ptr<Camera>& camera) 
	{ m_wpCamera = camera; }
	
	// ���
	void Reset();
	void Release();

private:
	// Math::Vector3��Effekseer::Vector3D�ɕϊ�
	const Effekseer::Vector3D ConvertToEfkVec3D(const Math::Vector3& vec) const
	{
		return Effekseer::Vector3D(vec.x, vec.y, vec.z);
	}

	// �G�t�F�N�g�Đ�
	std::shared_ptr<EffekseerObject> Play(const EffectInfo& info);
	std::shared_ptr<EffekseerObject> Play(const std::shared_ptr<EffekseerObject> spEfkObj);

	// �G�t�F�N�g�X�V
	void UpdateEffect();
	void UpdateEfkCameraMatrix();

	// �G�t�F�N�g�����_���[
	EffekseerRenderer::RendererRef m_pEfkRenderer = nullptr;

	// �G�t�F�N�g�}�l�[�W���[
	Effekseer::ManagerRef m_pEfkManager = nullptr;

	/// <summary>
	/// �R�}���h���X�g���g�p���郉�C�u�����p
	/// </summary>
	// �������v�[��
	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_pEfkMemoryPool = nullptr;
	// �R�}���h���X�g
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_pEfkCmdList = nullptr;

	// �G�t�F�N�g���X�g
	std::unordered_map<std::string, std::shared_ptr<EffekseerObject>> m_omEffects;

	// �J�������
	std::weak_ptr<Camera> m_wpCamera;

	// �ꎞ��~����
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
	// �Đ�����
	bool IsPlaying();

	// �o�^�����}�l�[�W���[�ݒ�
	void SetParentManager(Effekseer::ManagerRef& parentManager)
	{ m_parentManager = parentManager; }

	// �G�t�F�N�g�ݒ�
	void SetEffect(Effekseer::EffectRef& effect) { m_effect = effect; }

	// �G�t�F�N�g�n���h���ݒ�
	void SetHandle(Effekseer::Handle& handle) { m_handle = handle; }

	// �G�t�F�N�g���ݒ�
	void SetPos(const Math::Vector3& pos);
	void SetScale(const float scale = 1.0f);
	void SetSpeed(const float speed = 1.0f);
	void SetMatrix(const Math::Matrix& mWorld) const;
	void SetLoop(const bool isLoop = false) 
	{ m_info.IsLoop = isLoop; }
	// �G�t�F�N�g���ꊇ�ݒ�
	void SetEffectInfo(const EffekseerManager::EffectInfo& info) { m_info = info; }

	// �G�t�F�N�g�{�̎擾
	const Effekseer::EffectRef& GetEffect() const { return m_effect; }
	// �G�t�F�N�g�n���h���擾
	const Effekseer::Handle& GetHandle() const { return m_handle; }
	
	// �G�t�F�N�g���擾
	const EffekseerManager::EffectInfo& GetEffectInfo() const { return m_info; }
	EffekseerManager::EffectInfo& WorkEffectInfo() { return m_info; }

	// �G�t�F�N�g�e��P����擾
	const Math::Vector3& GetPos() const { return m_info.Pos; }
	const float GetSize() const { return m_info.Scale; }
	const float GetSpeed() const { return m_info.Speed; }
	const bool IsLoop() const { return m_info.IsLoop; }
	
private:
	// �G�t�F�N�g�}�l�[�W���[
	Effekseer::ManagerRef m_parentManager = nullptr;
	// �G�t�F�N�g�{��
	Effekseer::EffectRef m_effect = nullptr;
	// �G�t�F�N�g�n���h��
	Effekseer::Handle m_handle = -1;

	// �G�t�F�N�g���
	EffekseerManager::EffectInfo m_info = {};
};

constexpr auto EffectPath = "Assets/Effects/";