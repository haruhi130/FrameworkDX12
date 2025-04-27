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

	/// <summary>
	/// Effekseer������
	/// </summary>
	/// <param name="w">�E�B���h�E���T�C�Y</param>
	/// <param name="h">�E�B���h�E�c�T�C�Y</param>
	/// <returns></returns>
	bool Init(int w,int h);

	
	/// <summary>
	/// Effekseer�X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �G�t�F�N�g�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �G�t�F�N�g���Đ�
	/// </summary>
	/// <param name="fileName">�G�t�F�N�g��</param>
	/// <param name="pos">�G�t�F�N�g���W</param>
	/// <param name="isLoop">���[�v�ݒ�</param>
	/// <returns>EffekseerObject�C���X�^���X</returns>
	std::shared_ptr<EffekseerObject> Play(const std::string& fileName, const Math::Vector3& pos,bool isLoop = true);

	/// <summary>
	/// �G�t�F�N�g��S�Ē�~
	/// </summary>
	void StopAllEffect();
	
	/// <summary>
	/// �G�t�F�N�g���~
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	void StopEffect(const std::string& name);

	/// <summary>
	/// �G�t�F�N�g���ꎞ��~
	/// </summary>
	void OnPauseEfkUpdate()
	{ m_isPause = true; }

	/// <summary>
	/// �G�t�F�N�g�̍��W���Z�b�g
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="pos">���W</param>
	void SetPos(const std::string& name, const Math::Vector3& pos);
	
	/// <summary>
	/// �G�t�F�N�g�̉�]���Z�b�g
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x</param>
	void SetRotation(const std::string& name, const Math::Vector3& axis, const float angle);
	
	/// <summary>
	/// �G�t�F�N�g�̍s����Z�b�g
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="mWorld">�s��</param>
	void SetMatrix(const std::string& name, const Math::Matrix& mWorld);

	/// <summary>
	/// �G�t�F�N�g�̊g�k���Z�b�g
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="scale">�g�k���l</param>
	void SetScale(const std::string& name, const Math::Vector3& scale);
	void SetScale(const std::string& name, const float scale);

	/// <summary>
	/// �G�t�F�N�g�̍Đ����x���Z�b�g
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="speed">�Đ����x</param>
	void SetSpeed(const std::string& name, const float speed = 1.0f);

	/// <summary>
	/// �G�t�F�N�g���ꎞ��~
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="isPause">�L����</param>
	void SetPause(const std::string& name, const bool isPause);

	/// <summary>
	/// �G�t�F�N�g���Đ���������
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <returns>�Đ����Ȃ�true</returns>
	const bool IsPlaying(const std::string& name) const;

	/// <summary>
	/// �J���������Z�b�g
	/// </summary>
	/// <param name="spCamera">�J�����̃X�}�[�g�|�C���^</param>
	void SetCamera(const std::shared_ptr<Camera>& spCamera) 
	{ m_wpCamera = spCamera; }
	
	/// <summary>
	/// �Đ����̃G�t�F�N�g��S�Ē�~���G�t�F�N�g���X�g���N���A
	/// </summary>
	void Release();

private:
	/// <summary>
	/// Math::Vector3��Effekseer::Vector3D�ɕϊ�
	/// </summary>
	/// <param name="vec">Math::Vector3</param>
	/// <returns>Effekseer::Vector3D</returns>
	const Effekseer::Vector3D ConvertToEfkVec3D(const Math::Vector3& vec) const
	{ return Effekseer::Vector3D(vec.x, vec.y, vec.z); }

	/// <summary>
	/// �G�t�F�N�g�Đ�
	/// </summary>
	/// <param name="info">�G�t�F�N�g���</param>
	/// <returns>EffekseerObject�C���X�^���X</returns>
	std::shared_ptr<EffekseerObject> Play(const EffectInfo& info);
	std::shared_ptr<EffekseerObject> Play(const std::shared_ptr<EffekseerObject> spEfkObj);

	/// <summary>
	/// �G�t�F�N�g�X�V
	/// </summary>
	void UpdateEffect();

	/// <summary>
	/// Effekseer�Ŏg�p����J�����X�V
	/// </summary>
	void UpdateEfkCameraMatrix();

	// �G�t�F�N�g�����_���[
	EffekseerRenderer::RendererRef m_pEfkRenderer = nullptr;

	// �G�t�F�N�g�}�l�[�W���[
	Effekseer::ManagerRef m_pEfkManager = nullptr;

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
	/// <summary>
	/// �G�t�F�N�g���Đ�����
	/// </summary>
	/// <returns>�Đ����Ȃ�true</returns>
	bool IsPlaying();

	/// <summary>
	/// EffekseerManager���Z�b�g
	/// </summary>
	/// <param name="parentManager">EffekseerManager</param>
	void SetParentManager(Effekseer::ManagerRef& parentManager)
	{ m_parentManager = parentManager; }

	/// <summary>
	/// �G�t�F�N�g���Z�b�g
	/// </summary>
	/// <param name="effect">�G�t�F�N�g�{��</param>
	void SetEffect(Effekseer::EffectRef& effect) { m_effect = effect; }

	/// <summary>
	/// �G�t�F�N�g�n���h�����Z�b�g
	/// </summary>
	/// <param name="handle">�G�t�F�N�g�n���h��</param>
	void SetHandle(Effekseer::Handle& handle) { m_handle = handle; }

	/// <summary>
	/// �G�t�F�N�g�̍��W���Z�b�g
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// �G�t�F�N�g�̊g�k���Z�b�g
	/// </summary>
	/// <param name="scale">�g�k���l</param>
	void SetScale(const float scale = 1.0f);

	/// <summary>
	/// �G�t�F�N�g�̍Đ����x���Z�b�g
	/// </summary>
	/// <param name="speed">�Đ����x</param>
	void SetSpeed(const float speed = 1.0f);

	/// <summary>
	/// �G�t�F�N�g�̍s����Z�b�g
	/// </summary>
	/// <param name="mWorld">�s��</param>
	void SetMatrix(const Math::Matrix& mWorld) const;

	/// <summary>
	/// �G�t�F�N�g�̃��[�v�ݒ�
	/// </summary>
	/// <param name="isLoop">���[�v�ݒ�</param>
	void SetLoop(const bool isLoop = false) 
	{ m_info.IsLoop = isLoop; }

	/// <summary>
	/// �G�t�F�N�g�����ꊇ�ݒ�
	/// </summary>
	/// <param name="info">�G�t�F�N�g���</param>
	void SetEffectInfo(const EffekseerManager::EffectInfo& info) { m_info = info; }

	/// <summary>
	/// �G�t�F�N�g���擾
	/// </summary>
	/// <returns>�G�t�F�N�g�{��</returns>
	const Effekseer::EffectRef& GetEffect() const { return m_effect; }
	
	/// <summary>
	/// �G�t�F�N�g�n���h�����擾
	/// </summary>
	/// <returns>�G�t�F�N�g�n���h��</returns>
	const Effekseer::Handle& GetHandle() const { return m_handle; }
	
	/// <summary>
	/// �G�t�F�N�g����S�Ď擾
	/// </summary>
	/// <returns>�G�t�F�N�g���</returns>
	const EffekseerManager::EffectInfo& GetEffectInfo() const { return m_info; }
	EffekseerManager::EffectInfo& WorkEffectInfo() { return m_info; }

	/// <summary>
	/// �G�t�F�N�g�̏����ꕔ�擾
	/// </summary>
	/// <returns>�ꕔ�̃G�t�F�N�g���</returns>
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

// �G�t�F�N�g�t�@�C���p�X
constexpr auto EffectPath = "Assets/Effects/";