#pragma once

class InputCollector;
class InputButtonBase;
class InputAxisBase;

/// <summary>
/// ���͂��Ǘ�����N���X:������InputCollector���Ǘ�
/// �L�[�{�[�h��Q�[���p�b�h�������̓��͂𓯎��Ɏ�t�\
/// �^�p��Update�𖈃��[�v�̂͂��߂ɌĂяo���K�v������
/// </summary>
class InputManager
{
public:
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	void Update();

	// �S�Ă̗L���ȓ��͑��u����̃{�^�����͏�Ԃ��擾
	short GetButtonState(std::string_view name) const;

	bool IsFree(std::string_view name) const;
	bool IsPress(std::string_view name) const;
	bool IsHold(std::string_view name) const;
	bool IsRelease(std::string_view name) const;

	// �S�Ă̗L���ȓ��͑��u����̎����͏�Ԃ��擾
	Math::Vector2 GetAxisState(std::string_view name) const;

	// ���͑��u�̓o�^
	void AddDevice(std::string_view name, InputCollector* pDevice);
	void AddDevice(std::string_view name, std::unique_ptr<InputCollector>& upDevice);

	const std::unique_ptr<InputCollector>& GetDevice(std::string_view name) const;
	std::unique_ptr<InputCollector>& WorkDevice(std::string_view name);

	void Release();

private:
	InputManager(){}
	~InputManager() { Release(); }

	std::unordered_map<std::string, std::unique_ptr<InputCollector>> m_omInputDevices;
};

/// <summary>
/// �P��̓��̓f�o�C�X����̓��͂��R���N�V��������N���X
/// �L�[�{�[�h��Q�[���p�b�h�����ꂼ���InputCollector���K�v
/// �Q�[���Ŏg�p�������Index�Ǘ�
/// </summary>
class InputCollector
{
public:
	enum class ActiveState
	{
		Disable,	// ����:���S�ɒ�~���Ă�����
		Monitoring,	// �Ď�:�f�o�C�X�̓��͍X�V�A�A�v���ւ̉e������
		Enable,		// �L��:�A�v���ɓ��͂̉e����^����
	};

	InputCollector(){}
	~InputCollector(){}

	void Update();

	// ���͌��m
	bool IsInput();

	// �C�ӂ̓��͏󋵂̎擾
	short GetButtonState(std::string_view name)const;
	Math::Vector2 GetAxisState(std::string_view name)const;

	// ���̓f�o�C�X�̏�Ԃ̎擾�Ɛݒ�
	ActiveState GetActiveState() const { return m_state; }
	void SetActiveState(ActiveState state) { m_state = state; }

	// �A�v���P�[�V�����{�^���̒ǉ�:�㏑��
	void AddButton(std::string_view name, InputButtonBase* pButton);
	void AddButton(std::string_view name, std::shared_ptr<InputButtonBase> spButton);

	// ���͎��̒ǉ�:�㏑��
	void AddAxis(std::string_view name, InputAxisBase* pAxis);
	void AddAxis(std::string_view name, std::shared_ptr<InputAxisBase> spAxis);

	const std::shared_ptr<InputButtonBase> GetButton(std::string_view name) const;
	const std::shared_ptr<InputAxisBase> GetAxis(std::string_view name) const;

private:
	void Release();

	std::unordered_map<std::string, std::shared_ptr<InputButtonBase>> m_omButtons;
	std::unordered_map<std::string, std::shared_ptr<InputAxisBase>> m_omAxes;

	ActiveState m_state = ActiveState::Enable;
};

/// <summary>
/// �P��̃{�^���̓��͏�Ԃ�ێ�����@�\�����������N���X
/// �{�^���̓��͏�Ԃ�����BitFlag
/// BitFlag�̑g�ݍ��킹�Ń{�^���̓��͏�Ԃ��p�����[�^�Ƃ��ĕێ�����@�\
/// </summary>
class InputButtonBase
{
public:
	enum ButtonState
	{
		Free,					// ���͂��Ȃ�
		Press,					// �����ꂽ�u��
		Hold = Press << 1,		// �����Ă����
		Release = Press << 2	// �����ꂽ�u��
	};

	InputButtonBase(){}
	virtual ~InputButtonBase(){}

	void PreUpdate() { m_isNeedUpdate = true; }

	virtual void Update() = 0;

	// �������͉���
	void NoInput() { m_state = ButtonState::Free; }

	short GetState() const { return m_state; }

	virtual void GetCode(std::vector<int>& ret) const = 0;

protected:
	// �{�^���̓��͏��
	short m_state = ButtonState::Free;

	// �X�V�d���΍�
	bool m_isNeedUpdate = true;
};

/// <summary>
/// WinAPI��GetAsyncKeyState�𗘗p�����L�[����:�}�E�X�ƃL�[�{�[�h�̓���
/// �����̓��̓L�[�R�[�h��o�^�ł���
/// </summary>
class InputButtonForWindows : public InputButtonBase
{
public:
	InputButtonForWindows(int keyCode);
	InputButtonForWindows(std::initializer_list<int> keyCodeList);
	InputButtonForWindows(const std::vector<int>& keyCodeList);

	void Update() override;

	void GetCode(std::vector<int>& ret) const override;

private:
	std::list<int> m_keyCodeList;
};

/// <summary>
/// �P��̎��̓��͏�Ԃ�ێ�����@�\�����������N���X
/// �W���C�X�e�B�b�N��\���L�[�ȂǓ��͂�񎟌��̈ړ��x�N�g���ɕϊ����ĕێ�
/// </summary>
class InputAxisBase
{
public:
	InputAxisBase(){}
	virtual ~InputAxisBase(){}

	virtual void PreUpdate(){}

	virtual void Update() = 0;

	void NoInput() { m_axis = Math::Vector2::Zero; }

	void SetValueRate(float rate) { m_valueRate = rate; }
	void SetLimitValue(float limit) { m_limitValue = limit; }

	Math::Vector2 GetState() const;

protected:
	Math::Vector2 m_axis;

	// ���̐��l�ɏ�����␳
	float m_valueRate = 1.0f;

	// ���̌��E�l
	float m_limitValue = FLT_MAX;
};

/// <summary>
/// WinAPI��GetAsyncKeyState���͂𗘗p����������
/// �w�肵���㉺���E�̃L�[���͏󋵂������Ƃ��ĕێ�
/// </summary>
class InputAxisForWindows : public InputAxisBase
{
public:
	InputAxisForWindows(int upCode, int rightCode, int downCode, int leftCode);
	InputAxisForWindows(std::initializer_list<int> upCodes, std::initializer_list<int> rightCodes,
		std::initializer_list<int> downCodes, std::initializer_list<int> leftCodes);
	InputAxisForWindows(const std::vector<int>& upCodes, const std::vector<int>& rightCodes,
		const std::vector<int>& downCodes, const std::vector<int>& leftCodes);
	InputAxisForWindows(const std::shared_ptr<InputButtonBase> upButton, const std::shared_ptr<InputButtonBase> rightButton,
		const std::shared_ptr<InputButtonBase> downButton, const std::shared_ptr<InputButtonBase> leftButton);

	void PreUpdate() override;

	void Update() override;

private:
	enum AxisDIR
	{
		Up,
		Right,
		Down,
		Left,
		Max
	};

	std::vector<std::shared_ptr<InputButtonBase>> m_dirButtons;
};

/// <summary>
/// Windows�̃}�E�X�ړ��𗘗p����������
/// �}�E�X�̈ړ��ʂ������Ƃ��ĕێ�
/// </summary>
class InputAxisForWindowsMouse : public InputAxisBase
{
public:
	
	InputAxisForWindowsMouse(){}
	InputAxisForWindowsMouse(int fixCode);
	InputAxisForWindowsMouse(std::initializer_list<int> fixCodes);
	InputAxisForWindowsMouse(const std::vector<int>& fixCodes);
	InputAxisForWindowsMouse(const std::shared_ptr<InputButtonBase> fixButton);

	void PreUpdate() override;

	void Update() override;

private:
	POINT m_prevMousePos = {};

	bool m_isBeginFrame = true;

	std::shared_ptr<InputButtonBase> m_spFixButton;
};