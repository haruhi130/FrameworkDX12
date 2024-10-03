#pragma once

using inputTypeID = size_t;

// 軸の状態
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

// キーの状態
enum KeyStatus
{
	Free = 1 << 0,		// 押されていない
	Press = 1 << 1,		// 押した瞬間
	Hold = 1 << 2,		// 押している間
	Release = 1 << 3	// 離した瞬間
};

// 次に発行するフォーマットのIDを管理
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
	// コレクタが有効か
	inline const bool CheckActive() const
	{
		return m_isActive;
	}

	// コレクタの状態を設定
	inline void SetActive(const bool isActive)
	{
		m_isActive = isActive;
	}

	// コレクタの名前を取得
	inline std::string_view GetName()
	{
		return m_name;
	}

	// コレクタの名前をセット
	inline void SetName(std::string_view name)
	{
		m_name = name;
	}

	//------------------------------------------------
	// Key
	//------------------------------------------------

	// ボタンワークを追加
	inline void AddButtonWork(std::string_view work)
	{
		m_umWorkToButtons[work.data()];
	}

	// ボタンワークにボタンを追加
	inline void AddButtonToButtonWork(std::string_view work, std::string_view device, std::string_view button)
	{
		m_umWorkToButtons[work.data()][device.data()].emplace(button.data());
	}

	// ボタンワークを削除
	inline void RemoveButtonWork(std::string_view work)
	{
		m_umWorkToButtons.erase(work.data());
	}

	// ボタンワークからボタンを削除
	inline void RemoveButtonFromButtonWork(std::string_view work, std::string_view device, std::string_view button)
	{
		m_umWorkToButtons[work.data()][device.data()].erase(button.data());
	}

	// 全てのボタンワークを取得
	inline const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>& GetAllButtonWork()
	{
		return m_umWorkToButtons;
	}

	//------------------------------------------------
	// Axis
	//------------------------------------------------

	// 軸ワークを追加
	inline void AddAxisWork(std::string_view work)
	{
		m_umWorkToAxises[work.data()];
	}

	// 軸ワークに軸を追加
	inline void AddAxisToAxisWork(std::string_view work, std::string_view device, std::string_view axis)
	{
		m_umWorkToAxises[work.data()][device.data()].emplace(axis.data());
	}

	// 軸ワークを削除
	inline void RemoveAxisWork(std::string_view work)
	{
		m_umWorkToAxises.erase(work.data());
	}

	// 軸ワークから軸を削除
	inline void RemoveAxisFromAxisWork(std::string_view work, std::string_view device, std::string_view axis)
	{
		m_umWorkToAxises[work.data()][device.data()].erase(axis.data());
	}

	// 全ての軸ワークを取得
	inline const std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>>& GetAllAxisWork()
	{
		return m_umWorkToAxises;
	}

private:
	// 有効
	bool m_isActive = false;

	// コレクタの名前
	std::string m_name;

	// Workとキーを紐づけるコンテナ
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> m_umWorkToButtons;
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_set<std::string>>> m_umWorkToAxises;
};

template<typename KeyCodeType>
class InputButtonFormatBase
{
public:
	// ボタン追加
	inline void AddButton(std::string_view buttonName, KeyCodeType keyCode)
	{
		m_umButtonNameToKeyCode[buttonName.data()] = keyCode;
	}

	// ボタン削除
	inline void RemoveButton(std::string_view buttonName)
	{
		m_umButtonNameToKeyCode.erase(buttonName.data());
	}

	// ボタンの状態を確認する
	virtual const bool CheckButtonState(KeyCodeType keyCode) = 0;

	// 型ごとのフォーマットIDを取得
	static inline const inputTypeID GetTypeID()
	{
		// IDが発行されていなければ発行
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
	// ボタンの名前とキーコードを紐づけるコンテナ
	std::unordered_map<std::string, KeyCodeType> m_umButtonNameToKeyCode;
	// このフォーマットの型ID
	static inputTypeID m_buttonFormatTypeID;
};
// 型ごとのFormatTypeIDを初期化
template<typename KeyCodeType>
inputTypeID InputButtonFormatBase<KeyCodeType>::m_buttonFormatTypeID = 0;

template<typename KeyCodeType>
class InputAxisFormatBase
{
public:
	// 軸追加
	inline void AddAxis(std::string_view axisName, KeyCodeType keyCode)
	{
		m_umAxisNameToKeyCode[axisName.data()] = keyCode;
	}

	// 軸削除
	inline void RemoveAxis(std::string_view axisName)
	{
		m_umAxisNameToKeyCode.erase(axisName.data());
	}

	// 軸の状態を確認する
	virtual const AxisStatus CheckAxisState(KeyCodeType keyCode) = 0;

	// 型ごとのフォーマットIDを取得
	static inline const inputTypeID GetTypeID()
	{
		// IDが発行されていなければ発行
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
	// 軸の名前とキーコードを紐づけるコンテナ
	std::unordered_map<std::string, KeyCodeType> m_umAxisNameToKeyCode;
	// このフォーマットの型ID
	static inputTypeID m_axisFormatTypeID;
};
// 型ごとのFormatTypeIDの初期化
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
		// ボタン入力の検知

		// 全てのボタンフォーマットを検索
		for (auto&& format : m_umNameToButtonFormat)
		{
			// 全てのボタンを検索
			for (auto&& button : format.second->GetButtonNameToKeyCode())
			{
				// 前のフレームの入力状態を取得
				short result = m_umButtonToResult[button.first];
				// ボタンが押されている場合
				if (format.second->CheckButtonState(button.second))
				{
					// 前フレームでボタンが押されていない
					if (result & KeyStatus::Free)
					{
						// 結果にPressを追加
						result |= KeyStatus::Press;
					}
					// 前フレームでもボタンが押されている
					else
					{
						// 結果からPressを除外
						result &= ~KeyStatus::Press;
					}
					// 結果からFreeを除外
					result &= ~KeyStatus::Free;
					// 結果にHoldを追加
					result |= KeyStatus::Hold;
				}
				// ボタンが押されていない
				else
				{
					// 前フレームでボタンが押されている
					if (result & KeyStatus::Hold)
					{
						// 結果にReleaseを追加
						result |= KeyStatus::Release;
					}
					// 前フレームでもボタンが押されていない
					else
					{
						// 結果からReleaseを除外
						result &= ~KeyStatus::Release;
					}
					// 結果にHoldを追加
					result &= ~KeyStatus::Hold;
					// 結果からFreeを除外
					result |= KeyStatus::Free;
				}
				// ボタン入力の結果をコンテナに格納
				m_umButtonToResult[button.first] = result;
			}
		}

		//------------------------------------------------
		// 軸入力の検知

		// 全ての種類の軸入力を検索
		for (auto&& format : m_umNameToAxisFormat)
		{
			// 軸の状態を検索
			for (auto&& axis : format.second->GetAxisNamtToKeyCode())
			{
				// 軸入力の結果をコンテナに格納
				m_umAxisToResult[axis.first] = format.second->CheckAxisState(axis.second);
			}
		}
	}

	//------------------------------------------------
	// Button
	//------------------------------------------------

	// ボタンの状態を取得
	short GetButtonState(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second;
		}
		return 0;
	}

	// ボタンがFreeならtrue
	const bool IsButtonFree(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Free;
		}
		return false;
	}

	// ボタンがPressならtrue
	const bool IsButtonPress(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Press;
		}
		return false;
	}

	// ボタンがHoldならtrue
	const bool IsButtonHold(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Hold;
		}
		return false;
	}

	// ボタンがReleaseならtrue
	const bool IsButtonRelease(std::string_view buttonName)override final
	{
		auto it = m_umButtonToResult.find(buttonName.data());
		if (it != m_umButtonToResult.end())
		{
			return it->second & KeyStatus::Release;
		}
		return false;
	}

	// 全てのボタン入力の結果を取得
	const std::unordered_map<std::string, short>& GetAllButtonResult()override final
	{
		return m_umButtonToResult;
	}

	//------------------------------------------------
	// Axis
	//------------------------------------------------

	// 軸の状態を取得
	const AxisStatus GetAxisState(std::string_view axisName)override final
	{
		auto it = m_umAxisToResult.find(axisName.data());
		if (it != m_umAxisToResult.end())
		{
			return it->second;
		}
		return AxisStatus();
	}

	// 全ての軸入力の結果を取得
	const std::unordered_map<std::string, AxisStatus>& GetAllAxisResult()override final
	{
		return m_umAxisToResult;
	}

	//------------------------------------------------
	// 登録・取得
	//------------------------------------------------

	// ボタンのフォーマットを追加
	template<typename FormatType>
	void AddInputButtonFormat(std::string_view formatName)
	{
		inputTypeID type = InputButtonFormatBase<FormatType>::GetTypeID();
		m_umFormatTypeToName[type] = formatName.data();
		m_umNameToFormatType[formatName.data()] = type;
		m_umNameToButtonFormat[formatName.data()] = std::make_shared<FormatType>();
	}

	// 軸のフォーマットを追加
	template<typename FormatType>
	void AddInputAxisFormat(std::string_view formatName)
	{
		inputTypeID type = InputAxisFormatBase<FormatType>::GetTypeID();
		m_umFormatTypeToName[type] = formatName.data();
		m_umNameToFormatType[formatName.data()] = type;
		m_umNameToAxisFormat[formatName.data()] = std::make_shared<FormatType>();
	}

	// デバイスの型IDを取得
	static inline const inputTypeID GetTypeID()
	{
		// 型IDが発行されていなければ発行
		if (!InputDeviceBase<KeyCodeType>::m_deviceTypeID)
		{
			InputDeviceBase<KeyCodeType>::m_deviceTypeID =
				InputTypeManager::GenerateID();
		}
		return InputDeviceBase<KeyCodeType>::m_deviceTypeID;
	}

protected:
	// 入力の種類と管理クラスを紐づけるコンテナ
	std::unordered_map<std::string, std::shared_ptr<InputButtonFormatBase<KeyCodeType>>> m_umNameToButtonFormat;
	std::unordered_map<std::string, std::shared_ptr<InputAxisFormatBase<KeyCodeType>>> m_umNameToAxisFormat;

	// 入力形式の型と名前を紐づけるコンテナ
	std::unordered_map<inputTypeID, std::string> m_umFormatTypeToName;
	std::unordered_map<std::string, inputTypeID> m_umNameToFormatType;

	// 入力の結果を格納するコンテナ
	std::unordered_map<std::string, short> m_umButtonToResult;
	std::unordered_map<std::string, AxisStatus> m_umAxisToResult;

	// デバイスの型ID
	static inputTypeID m_deviceTypeID;
};
template<typename KeyCodeType>
inputTypeID InputDeviceBase<KeyCodeType>::m_deviceTypeID = 0;

class InputDeviceCentor
{
public:

	// 入力デバイスを追加
	template<typename DeviceType>
	inline std::shared_ptr<DeviceType> AddDevice(std::string_view deviceName)
	{
		// インスタンスを作成してコンテナにセット
		std::shared_ptr<DeviceType> spNewDevice = std::make_shared<DeviceType>();
		m_umNameToDevice[deviceName.data()] = spNewDevice;

		// デバイスの型IDと名前を紐づけ
		m_umDeviceTypeToName[DeviceType::GetTypeID()] = deviceName.data();

		return spNewDevice;
	}

	// 入力デバイスを取得
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

	// 入力デバイスを取得
	inline std::shared_ptr<IInputDeviceBase> GetDevice(std::string_view name)
	{
		auto it = m_umNameToDevice.find(name.data());
		if (it != m_umNameToDevice.end())
		{
			return it->second;
		}
		return nullptr;
	}

	// 全てのデバイスを格納するコンテナの参照を取得
	inline const std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>>& GetAllDevice()
	{
		return m_umNameToDevice;
	}

	// 更新
	void Update()
	{
		for (auto&& device : m_umNameToDevice)
		{
			device.second->Update();
		}
	}

private:
	// デバイスの名前と実体を紐づけるコンテナ
	std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>> m_umNameToDevice;
	// デバイスの型と名前を紐づけるコンテナ
	std::unordered_map<inputTypeID, std::string> m_umDeviceTypeToName;
};

class Input
{
public:
	//-------------------------------------------
	// デバイス
	//-------------------------------------------

	// デバイスを追加
	template<typename DeviceType>
	inline void AddDevice(std::string_view deviceName)
	{
		m_deviceCentor.AddDevice<DeviceType>(deviceName);
	}

	// デバイスを取得
	template<typename DeviceType>
	inline std::shared_ptr<DeviceType> GetDevice()
	{
		return m_deviceCentor.GetDevice<DeviceType>();
	}

	// デバイスを取得
	std::shared_ptr<IInputDeviceBase> GetDevice(std::string_view deviceName)
	{
		return m_deviceCentor.GetDevice(deviceName);
	}

	// 全てのデバイスを取得
	inline const std::unordered_map<std::string, std::shared_ptr<IInputDeviceBase>>& GetAllDevice()
	{
		return m_deviceCentor.GetAllDevice();
	}

	//-------------------------------------------
	// コレクタ
	//-------------------------------------------

	// コレクタを追加
	inline std::shared_ptr<InputCollector> AddCollector(std::string_view collectorName)
	{
		std::shared_ptr<InputCollector> spCollector = std::make_shared<InputCollector>();
		m_umNameToCollector[collectorName.data()] = spCollector;
		spCollector->SetName(collectorName);
		spCollector->SetActive(true);
		return spCollector;
	}

	// コレクタを取得
	inline std::shared_ptr<InputCollector>GetCollector(std::string_view collectorName)
	{
		return m_umNameToCollector[collectorName.data()];
	}

	// 全てのコレクタを取得
	inline std::unordered_map<std::string, std::shared_ptr<InputCollector>>& GetAllCollector()
	{
		return m_umNameToCollector;
	}

	//-------------------------------------------
	// Work
	//-------------------------------------------

	// ワークの状態を取得
	inline const short GetButtonWorkState(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()];
	}

	// ボタンワークがFreeならtrue
	inline const bool IsButtonWorkFree(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Free;
	}
	// ボタンワークがPressならtrue
	inline const bool IsButtonWorkPress(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Free;
	}
	// ボタンワークがHoldならtrue
	inline const bool IsButtonWorkHold(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Hold;
	}
	// ボタンワークがReleaseならtrue
	inline const bool IsButtonWorkRelease(std::string_view work)
	{
		return m_umWorkToButtonResult[work.data()] & KeyStatus::Release;
	}

	// 軸の状態を取得
	inline const AxisStatus GetAxisStatus(std::string_view work)
	{
		return m_umWorkToAxisResult[work.data()];
	}

	//-------------------------------------------
	// 更新
	//-------------------------------------------

	void Update()
	{
		// デバイスの入力を検知
		m_deviceCentor.Update();

		// 前フレームのワークのリザルトをリセット
		m_umWorkToButtonResult.clear();
		m_umWorkToAxisResult.clear();

		// コレクタの入力結果をセット
		for (auto&& collector : m_umNameToCollector)
		{
			if (!collector.second->CheckActive())
			{
				continue;
			}
			// キーの入力結果をセット
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
			// 軸の入力結果をセット
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

	// デバイスを管理するクラスのインスタンス
	InputDeviceCentor m_deviceCentor;

	// コレクタの名前とインスタンスを紐づけるコンテナ
	std::unordered_map<std::string, std::shared_ptr<InputCollector>> m_umNameToCollector;
	// Workと結果を紐づけるコンテナ
	std::unordered_map<std::string, short> m_umWorkToButtonResult;
	std::unordered_map<std::string, AxisStatus> m_umWorkToAxisResult;
};
