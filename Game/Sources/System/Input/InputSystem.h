#pragma once

using inputTypeID = size_t;

// ���̏��
struct AxisStatus
{
	AxisStatus() = default;
	AxisStatus(const float width, const float height)
		:m_width(width), m_height(height) {}

	float m_width;
	float m_height;

	AxisStatus operator+(const AxisStatus& value)
	{
		return AxisStatus(m_width + value.m_width, m_height + value.m_height);
	}
	AxisStatus operator+=(const AxisStatus& value)
	{
		m_width += value.m_width;
		m_height += value.m_height;
		return *this;
	}
};

// �L�[�̏��
enum KeyStatus
{
	Free = 1 << 0,		// ������Ă��Ȃ�
	Press = 1 << 1,		// �������u��
	Hold = 1 << 2,		// �����Ă����
	Release = 1 << 3	// �������u��
};

// ���ɔ��s����t�H�[�}�b�g��ID���Ǘ�
class InputTypeManager
{
public:
	static inline const inputTypeID GenerateID()
	{
		return ++m_nextInputTypeID;
	}

private:
	static inputTypeID m_nextInputTypeID;
};

inputTypeID InputTypeManager::m_nextInputTypeID = 0;

class InputCollector
{
public:
	// �R���N�^���L����
	inline const bool CheckActive() const
	{
		return m_isActive;
	}

	// �R���N�^�̏�Ԃ�ݒ�
	inline void SetActive(const bool isActive)
	{
		m_isActive = isActive;
	}

	// �R���N�^�̖��O���擾
	inline std::string_view GetName()
	{
		return m_name;
	}

	// �R���N�^�̖��O���Z�b�g
	inline void SetName(std::string_view name)
	{
		m_name = name;
	}

	//------------------------------------------------
	// Key
	//------------------------------------------------

	// �{�^�����[�N��ǉ�
	inline void AddButtonWork(std::string_view work)
	{
		m_umWorkToButtons[work.data()];
	}

	// �{�^�����[�N�Ƀ{�^����ǉ�
	inline void AddButtonToButtonWork(std::string_view work, std::string_view device, std::string_view button)
	{
		m_umWorkToButtons[work.data()][device.data()].emplace(button.data());
	}

	// �{�^�����[�N���폜
	inline void RemoveButtonWork(std::string_view work)
	{
		m_umWorkToButtons.erase(work.data());
	}

	// �{�^�����[�N����{�^�����폜
	inline void RemoveButtonFromButtonWork(std::string_view work, std::string_view device, std::string_view button)
	{
		m_umWorkToButtons[work.data()][device.data()].erase(button.data());
	}

	// �S�Ẵ{�^�����[�N���擾
	inline const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>& GetAllButtonWork()
	{
		return m_umWorkToButtons;
	}

	//------------------------------------------------
	// Axis
	//------------------------------------------------

	// �����[�N��ǉ�
	inline void AddAxisWork(std::string_view work)
	{
		m_umWorkToAxises[work.data()];
	}

	// �����[�N�Ɏ���ǉ�
	inline void AddAxisToAxisWork(std::string_view work, std::string_view device, std::string_view axis)
	{
		m_umWorkToAxises[work.data()][device.data()].emplace(axis.data());
	}

	// �����[�N���폜
	inline void RemoveAxisWork(std::string_view work)
	{
		m_umWorkToAxises.erase(work.data());
	}

	// �����[�N���玲���폜
	inline void RemoveAxisFromAxisWork(std::string_view work, std::string_view device, std::string_view axis)
	{
		m_umWorkToAxises[work.data()][device.data()].erase(axis.data());
	}

	// �S�Ă̎����[�N���擾
	inline const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>& GetAllAxisWork()
	{
		return m_umWorkToAxises;
	}

private:
	// �L��
	bool m_isActive = false;

	// �R���N�^�̖��O
	std::string m_name;

	// Work�ƃL�[��R�Â���R���e�i
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> m_umWorkToButtons;
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> m_umWorkToAxises;
};

template<typename KeyCodeType>
class InputButtonFormatBase
{
public:
	// �{�^���ǉ�
	inline void AddButton(std::string_view buttonName, KeyCodeType keyCode)
	{
		m_umButtonNameToKeyCode[buttonName.data()] = keyCode;
	}

	// �{�^���폜
	inline void RemoveButton(std::string_view buttonName)
	{
		m_umButtonNameToKeyCode.erase(buttonName.data());
	}

	// �{�^���̏�Ԃ��m�F����
	virtual const bool CheckButtonState(KeyCodeType keyCode) = 0;

	// �^���Ƃ̃t�H�[�}�b�gID���擾
	static inline const inputTypeID GetTypeID()
	{
		// ID�����s����Ă��Ȃ���Δ��s
		if (!m_buttonFormatTypeID)
		{
			InputButtonFormatBase<KeyCodeType>::m_buttonFormatTypeID =
				InputTypeManager::GenerateID();
		}

		return InputButtonFormatBase<KeyCodeType>::m_buttonFormatTypeID;
	}

	inline const std::unordered_map<std::string, KeyCodeType>& GetButtonNameToKeyCode()
	{
		return m_umButtonNameToKeyCode;
	}

private:
	// �{�^���̖��O�ƃL�[�R�[�h��R�Â���R���e�i
	std::unordered_map<std::string, KeyCodeType> m_umButtonNameToKeyCode;
	// ���̃t�H�[�}�b�g�̌^ID
	static inputTypeID m_buttonFormatTypeID;
};
// �^���Ƃ�FormatTypeID��������
template<typename KeyCodeType>
inputTypeID InputButtonFormatBase<KeyCodeType>::m_buttonFormatTypeID = 0;

template<typename KeyCodeType>
class InputAxisFormatBase
{
public:
	// ���ǉ�
	inline void AddAxis(std::string_view axisName, KeyCodeType keyCode)
	{
		m_umAxisNameToKeyCode[axisName.data()] = keyCode;
	}

	// ���폜
	inline void RemoveAxis(std::string_view axisName)
	{
		m_umAxisNameToKeyCode.erase(axisName.data());
	}

	// ���̏�Ԃ��m�F����
	virtual const AxisStatus CheckAxisState(KeyCodeType keyCode) = 0;

	// �^���Ƃ̃t�H�[�}�b�gID���擾
	static inline const inputTypeID GetTypeID()
	{
		// ID�����s����Ă��Ȃ���Δ��s
		if (!m_axisFormatTypeID)
		{
			InputAxisFormatBase<KeyCodeType>::m_axisFormatTypeID =
				InputTypeManager::GenerateID();
		}

		return InputAxisFormatBase<KeyCodeType>::m_axisFormatTypeID;
	}

	inline const std::unordered_map<std::string, KeyCodeType>& GetAxisNameToKeyCode()
	{
		return m_umAxisNameToKeyCode;
	}

private:
	// ���̖��O�ƃL�[�R�[�h��R�Â���R���e�i
	std::unordered_map<std::string, KeyCodeType> m_umAxisNameToKeyCode;
	// ���̃t�H�[�}�b�g�̌^ID
	static inputTypeID m_axisFormatTypeID;
};
// �^���Ƃ�FormatTypeID�̏�����
template<typename KeyCodeType>
inputTypeID InputAxisFormatBase<KeyCodeType>::m_axisFormatTypeID = 0;

class IInputDeviceBase
{
public:
	virtual void Update() = 0;

	virtual short GetButtonState(std::string_view buttonName) = 0;

	virtual const bool IsButtonFree(std::string_view buttonName) = 0;
	virtual const bool IsButtonPress(std::string_view buttonName) = 0;
	virtual const bool IsButtonHold(std::string_view buttonName) = 0;
	virtual const bool IsButtonRelease(std::string_view buttonName) = 0;
	virtual const std::unordered_map<std::string, short>& GetAllButtonResult() = 0;

	virtual const AxisStatus GetAxisState(std::string_view axisName) = 0;
	virtual const std::unordered_map<std::string, AxisStatus>& GetAllAxisResult() = 0;
};

template<typename KeyCodeType>
class InputDeviceBase : public IInputDeviceBase
{
public:
	virtual void Update()override final
	{
		// �{�^�����͂̌��m

		// �S�Ẵ{�^���t�H�[�}�b�g������
		for (auto&& format : m_umNameToButtonFormat)
		{
			// �S�Ẵ{�^��������
			for (auto&& button : format.second->GetButtonNameToKeyCode())
			{
				// �O�̃t���[���̓��͏�Ԃ��擾
				short result = m_umButtonToResult[button.first];
				// �{�^����������Ă���ꍇ
				if (format.second->CheckButtonState(button.second))
				{
					// �O�t���[���Ń{�^����������Ă��Ȃ�
					if (result & KeyStatus::Free)
					{
						// ���ʂ�Press��ǉ�
						result |= KeyStatus::Press;
					}
					// �O�t���[���ł��{�^����������Ă���
					else
					{
						// ���ʂ���Press�����O
						result &= ~KeyStatus::Press;
					}
					// ���ʂ���Free�����O
					result &= ~KeyStatus::Free;
					// ���ʂ�Hold��ǉ�
					result |= KeyStatus::Hold;
				}
				// �{�^����������Ă��Ȃ�
				else
				{
					// �O�t���[���Ń{�^����������Ă���
					if (result & KeyStatus::Hold)
					{
						// ���ʂ�Release��ǉ�
						result |= KeyStatus::Release;
					}
					// �O�t���[���ł��{�^����������Ă��Ȃ�
					else
					{
						// ���ʂ���Release�����O
						result &= ~KeyStatus::Release;
					}
					// ���ʂ�Hold��ǉ�
					result &= ~KeyStatus::Hold;
					// ���ʂ���Free�����O
					result |= KeyStatus::Free;
				}
				// �{�^�����͂̌��ʂ��R���e�i�Ɋi�[
				m_umButtonToResult[button.first] = result;
			}
		}

		//------------------------------------------------
		// �����͂̌��m

		// �S�Ă̎�ނ̎����͂�����
		for (auto&& format : m_umNameToAxisFormat)
		{
			// ���̏�Ԃ�����
			for (auto&& axis : format.second->GetAxisNamtToKeyCode())
			{
				// �����͂̌��ʂ��R���e�i�Ɋi�[
				m_umAxisToResult[axis.first] = format.second->CheckAxisState(axis.second);
			}
		}
	}

	//------------------------------------------------
	// Button
	//------------------------------------------------

	// �{�^���̏�Ԃ��擾
	short GetButtonState(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second;
		}
		return 0;
	}

	// �{�^����Free�Ȃ�true
	const bool IsButtonFree(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Free;
		}
		return false;
	}

	// �{�^����Press�Ȃ�true
	const bool IsButtonPress(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Press;
		}
		return false;
	}

	// �{�^����Hold�Ȃ�true
	const bool IsButtonHold(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Hold;
		}
		return false;
	}

	// �{�^����Release�Ȃ�true
	const bool IsButtonRelease(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Release;
		}
		return false;
	}

	// �S�Ẵ{�^�����͂̌��ʂ��擾
	const std::unordered_map<std::string, short>& GetAllButtonResult()override final
	{
		return m_umButtonToResult;
	}

	//------------------------------------------------
	// Axis
	//------------------------------------------------

	// ���̏�Ԃ��擾
	const AxisStatus GetAxisState(std::string_view axisName)override final
	{
		auto it = m_umAxisToResult.find(axisName.data());
		if (it != m_umAxisToResult.end())
		{
			return it->second;
		}
		return AxisStatus();
	}

	// �S�Ă̎����͂̌��ʂ��擾
	const std::unordered_map<std::string, AxisStatus>& GetAllAxisResult()override final
	{
		return m_umAxisToResult;
	}

	//------------------------------------------------
	// �o�^�E�擾
	//------------------------------------------------

	// �{�^���̃t�H�[�}�b�g��ǉ�
	template<typename FormatType>
	void AddInputButtonFormat(std::string_view formatName)
	{
		inputTypeID type = InputButtonFormatBase<FormatType>::GetTypeID();
		m_umFormatTypeToName[type] = formatName.data();
		m_umNameToFormatType[formatName.data()] = type;
		m_umNameToButtonFormat[formatName.data()] = std::make_shared<FormatType>();
	}

	// ���̃t�H�[�}�b�g��ǉ�
	template<typename FormatType>
	void AddInputAxisFormat(std::string_view formatName)
	{
		inputTypeID type = InputAxisFormatBase<FormatType>::GetTypeID();
		m_umFormatTypeToName[type] = formatName.data();
		m_umNameToFormatType[formatName.data()] = type;
		m_umNameToAxisFormat[formatName.data()] = std::make_shared<FormatType>();
	}

	// �f�o�C�X�̌^ID���擾
	static inline const inputTypeID GetTypeID()
	{
		// �^ID�����s����Ă��Ȃ���Δ��s
		if (!InputDeviceBase<KeyCodeType>::m_deviceTypeID)
		{
			InputDeviceBase<KeyCodeType>::m_deviceTypeID =
				InputTypeManager::GenerateID();
		}
		return InputDeviceBase<KeyCodeType>::m_deviceTypeID;
	}

protected:
	// ���͂̎�ނƊǗ��N���X��R�Â���R���e�i
	std::unordered_map<std::string, std::shared_ptr<InputButtonFormatBase<KeyCodeType>>> m_umNameToButtonFormat;
	std::unordered_map<std::string, std::shared_ptr<InputAxisFormatBase<KeyCodeType>>> m_umNameToAxisFormat;

	// ���͌`���̌^�Ɩ��O��R�Â���R���e�i
	std::unordered_map<inputTypeID, std::string> m_umFormatTypeToName;
	std::unordered_map<std::string, inputTypeID> m_umNameToFormatType;

	// ���͂̌��ʂ��i�[����R���e�i
	std::unordered_map<std::string, short> m_umButtonToResult;
	std::unordered_map<std::string, AxisStatus> m_umAxisToResult;

	// �f�o�C�X�̌^ID
	static inputTypeID m_deviceTypeID;
};
template<typename KeyCodeType>
inputTypeID InputDeviceBase<KeyCodeType>::m_deviceTypeID = 0;

class InputDeviceCentor
{
public:

	// ���̓f�o�C�X��ǉ�
	template<typename DeviceType>
	inline std::shared_ptr<DeviceType> AddDevice(std::string_view deviceName)
	{
		// �C���X�^���X���쐬���ăR���e�i�ɃZ�b�g
		std::shared_ptr<DeviceType> spNewDevice = std::make_shared<DeviceType>();
		m_umNameToDevice[deviceName.data()] = spNewDevice;

		// �f�o�C�X�̌^ID�Ɩ��O��R�Â�
		m_umDeviceTypeToName[DeviceType::GetTypeID()] = deviceName.data();

		return spNewDevice;
	}

	// ���̓f�o�C�X���擾
	template<typename DeviceType>
	inline std::shared_ptr<DeviceType> GetDevice()
	{
		inputTypeID type = DeviceType::GetTypeID();
		auto it = m_umDeviceTypeToName.find(type);
		if (it != m_umDeviceTypeToName.end())
		{
			return std::static_pointer_cast<DeviceType>(m_umNameToDevice[it->second]);
		}
		return nullptr;
	}

	// ���̓f�o�C�X���擾
	inline std::shared_ptr<IInputDeviceBase> GetDevice(std::string_view name)
	{
		auto it = m_umNameToDevice.find(name.data());
		if (it != m_umNameToDevice.end())
		{
			return it->second;
		}
		return nullptr;
	}

	// �S�Ẵf�o�C�X���i�[����R���e�i�̎Q�Ƃ��擾
	inline const std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>>& GetAllDevice()
	{
		return m_umNameToDevice;
	}

	// �X�V
	void Update()
	{
		for (auto&& device : m_umNameToDevice)
		{
			device.second->Update();
		}
	}

private:
	// �f�o�C�X�̖��O�Ǝ��̂�R�Â���R���e�i
	std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>> m_umNameToDevice;
	// �f�o�C�X�̌^�Ɩ��O��R�Â���R���e�i
	std::unordered_map<inputTypeID, std::string> m_umDeviceTypeToName;
};

class Input
{
public:
	//-------------------------------------------
	// �f�o�C�X
	//-------------------------------------------

	// �f�o�C�X��ǉ�
	template<typename DeviceType>
	inline void AddDevice(std::string_view deviceName)
	{
		m_deviceCentor.AddDevice<DeviceType>(deviceName);
	}

	// �f�o�C�X���擾
	template<typename DeviceType>
	inline std::shared_ptr<DeviceType> GetDevice()
	{
		return m_deviceCentor.GetDevice<DeviceType>();
	}

	// �f�o�C�X���擾
	std::shared_ptr<IInputDeviceBase> GetDevice(std::string_view deviceName)
	{
		return m_deviceCentor.GetDevice(deviceName);
	}

	// �S�Ẵf�o�C�X���擾
	inline const std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>>& GetAllDevice()
	{
		return m_deviceCentor.GetAllDevice();
	}

	//-------------------------------------------
	// �R���N�^
	//-------------------------------------------

	// �R���N�^��ǉ�
	inline std::shared_ptr<InputCollector> AddCollector(std::string_view collectorName)
	{
		std::shared_ptr<InputCollector> spCollector = std::make_shared<InputCollector>();
		m_umNameToCollector[collectorName.data()] = spCollector;
		spCollector->SetName(collectorName);
		spCollector->SetActive(true);
		return spCollector;
	}

	// �R���N�^���擾
	inline std::shared_ptr<InputCollector>GetCollector(std::string_view collectorName)
	{
		return m_umNameToCollector[collectorName.data()];
	}

	// �S�ẴR���N�^���擾
	inline std::unordered_map<std::string, std::shared_ptr<InputCollector>>& GetAllCollector()
	{
		return m_umNameToCollector;
	}

	//-------------------------------------------
	// Work
	//-------------------------------------------

	// ���[�N�̏�Ԃ��擾
	inline const short GetButtonWorkState(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()];
	}

	// �{�^�����[�N��Free�Ȃ�true
	inline const bool IsButtonWorkFree(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Free;
	}
	// �{�^�����[�N��Press�Ȃ�true
	inline const bool IsButtonWorkPress(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Free;
	}
	// �{�^�����[�N��Hold�Ȃ�true
	inline const bool IsButtonWorkHold(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Hold;
	}
	// �{�^�����[�N��Release�Ȃ�true
	inline const bool IsButtonWorkRelease(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Release;
	}

	// ���̏�Ԃ��擾
	inline const AxisStatus GetAxisStatus(std::string_view work)
	{
		return m_umWorkToAxisResult[work.data()];
	}

	//-------------------------------------------
	// �X�V
	//-------------------------------------------

	void Update()
	{
		// �f�o�C�X�̓��͂����m
		m_deviceCentor.Update();

		// �O�t���[���̃��[�N�̃��U���g�����Z�b�g
		m_umWorkToButtonResult.clear();
		m_umWorkToAxisResult.clear();

		// �R���N�^�̓��͌��ʂ��Z�b�g
		for (auto&& collector : m_umNameToCollector)
		{
			if (!collector.second->CheckActive())
			{
				continue;
			}
			// �L�[�̓��͌��ʂ��Z�b�g
			for (auto&& work : collector.second->GetAllButtonWork())
			{
				for (auto&& deviceToButtons : work.second)
				{
					for (auto&& button : deviceToButtons.second)
					{
						m_umWorkToButtonResult[work.first] |= m_deviceCentor.GetDevice(deviceToButtons.first)->GetButtonState(button);
					}
				}
			}
			// ���̓��͌��ʂ��Z�b�g
			for (auto&& work : collector.second->GetAllAxisWork())
			{
				for (auto&& deviceToAxises : work.second)
				{
					for (auto&& axis : deviceToAxises.second)
					{
						m_umWorkToAxisResult[work.first] += m_deviceCentor.GetDevice(deviceToAxises.first)->GetAxisState(axis);
					}
				}
			}
		}
	}

private:

	// �f�o�C�X���Ǘ�����N���X�̃C���X�^���X
	InputDeviceCentor m_deviceCentor;

	// �R���N�^�̖��O�ƃC���X�^���X��R�Â���R���e�i
	std::unordered_map<std::string, std::shared_ptr<InputCollector>> m_umNameToCollector;
	// Work�ƌ��ʂ�R�Â���R���e�i
	std::unordered_map<std::string, short> m_umWorkToButtonResult;
	std::unordered_map<std::string, AxisStatus> m_umWorkToAxisResult;
};
