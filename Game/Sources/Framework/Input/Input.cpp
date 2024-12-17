#include "Input.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputManager
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
void InputManager::Update()
{
	for (auto& device : m_omInputDevices)
	{
		device.second->Update();
	}
}

short InputManager::GetButtonState(std::string_view name) const
{
	short buttonState = InputButtonBase::Free;

	for (auto& device : m_omInputDevices)
	{
		// 有効時のみ入力に影響を与える
		if (device.second->GetActiveState() == InputCollector::ActiveState::Enable)
		{
			buttonState |= device.second->GetButtonState(name);
		}
	}

	return buttonState;
}

bool InputManager::IsFree(std::string_view name) const
{
	return (GetButtonState(name) == InputButtonBase::Free);
}

bool InputManager::IsPress(std::string_view name) const
{
	return (GetButtonState(name) & InputButtonBase::Press);
}

bool InputManager::IsHold(std::string_view name) const
{
	return (GetButtonState(name) & InputButtonBase::Hold);
}

bool InputManager::IsRelease(std::string_view name) const
{
	return (GetButtonState(name) & InputButtonBase::Release);
}

Math::Vector2 InputManager::GetAxisState(std::string_view name) const
{
	float leftValue = 0.0f, rightValue = 0.0f, topValue = 0.0f, bottomValue = 0.0f;

	for (auto& collector : m_omInputDevices)
	{
		// 有効時のみ入力に影響を与える
		if (collector.second->GetActiveState() == InputCollector::ActiveState::Enable)
		{
			Math::Vector2 nowDeviceAxis;

			nowDeviceAxis = collector.second->GetAxisState(name);

			// 入力されていなければスキップ
			if (nowDeviceAxis.LengthSquared() == 0.0f) { continue; }

			// 左右入力をX軸数値で判定
			if (nowDeviceAxis.x < 0.0f)
			{
				// 左の場合最小値
				leftValue = std::min(nowDeviceAxis.x, leftValue);
			}
			else
			{
				// 右の場合最大値
				rightValue = std::max(nowDeviceAxis.x, rightValue);
			}

			// 上下入力をY軸数値で判定
			if (nowDeviceAxis.y < 0.0f)
			{
				// 下の場合最小値
				bottomValue = std::min(nowDeviceAxis.y, bottomValue);
			}
			else
			{
				// 上の場合最大値
				topValue = std::max(nowDeviceAxis.y, topValue);
			}
		}
	}

	// 左右と上下の入力値を合成して出力
	return Math::Vector2(leftValue + rightValue, topValue + bottomValue);
}

void InputManager::AddDevice(std::string_view name, InputCollector* pDevice)
{
	std::unique_ptr<InputCollector> upNewDevice(pDevice);

	AddDevice(name, upNewDevice);
}

void InputManager::AddDevice(std::string_view name, std::unique_ptr<InputCollector>& upDevice)
{
	m_omInputDevices[name.data()] = std::move(upDevice);
}

const std::unique_ptr<InputCollector>& InputManager::GetDevice(std::string_view name) const
{
	auto device = m_omInputDevices.find(name.data());

	if (device == m_omInputDevices.end())
	{
		assert(0 && "InputManager::GetDevice:未登録のデバイス");
	}

	return device->second;
}

std::unique_ptr<InputCollector>& InputManager::WorkDevice(std::string_view name)
{
	auto device = m_omInputDevices.find(name.data());

	if (device == m_omInputDevices.end())
	{
		assert(0 && "InputManager::WorkDevice:未登録のデバイス");
	}

	return device->second;
}

void InputManager::Release()
{
	m_omInputDevices.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputCollector
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
void InputCollector::Update()
{
	{
		for (auto& button : m_omButtons)
		{
			button.second->PreUpdate();
		}
		for (auto& axis : m_omAxes)
		{
			axis.second->PreUpdate();
		}
	}

	// 有効or監視:ボタンの状態更新
	if (GetActiveState() != ActiveState::Disable)
	{
		for (auto& button : m_omButtons)
		{
			button.second->Update();
		}
		for (auto& axis : m_omAxes)
		{
			axis.second->Update();
		}
	}
	// 無効:全て入力されていない状態に更新
	else
	{
		for (auto& button : m_omButtons)
		{
			button.second->NoInput();
		}
		for (auto& axis : m_omAxes)
		{
			axis.second->NoInput();
		}
	}
}

bool InputCollector::IsInput()
{
	for (auto& button : m_omButtons)
	{
		if (!button.second) { continue; }

		// 入力を受け付けていたらtrue
		if (button.second->GetState() != InputButtonBase::ButtonState::Free) { return true; }
	}
	for (auto& axis : m_omAxes)
	{
		if (!axis.second) { continue; }

		// 入力を受け付けていたらtrue
		if (axis.second->GetState().LengthSquared() != 0.0f) { return true; }
	}

	// 入力が検知されなければfalse
	return false;
}

short InputCollector::GetButtonState(std::string_view name) const
{
	const std::shared_ptr<InputButtonBase>& spButton = GetButton(name);
	
	if (!spButton)
	{
		return InputButtonBase::ButtonState::Free;
	}
	
	return spButton->GetState();
}

Math::Vector2 InputCollector::GetAxisState(std::string_view name) const
{
	const std::shared_ptr<InputAxisBase>& spAxis = GetAxis(name);

	if (!spAxis)
	{
		return Math::Vector2::Zero;
	}

	return spAxis->GetState();
}

void InputCollector::AddButton(std::string_view name, InputButtonBase* pButton)
{
	AddButton(name.data(), std::shared_ptr<InputButtonBase>(pButton));
}

void InputCollector::AddButton(std::string_view name, std::shared_ptr<InputButtonBase> spButton)
{
	m_omButtons[name.data()] = spButton;
}

void InputCollector::AddAxis(std::string_view name, InputAxisBase* pAxis)
{
	AddAxis(name.data(), std::shared_ptr<InputAxisBase>(pAxis));
}

void InputCollector::AddAxis(std::string_view name, std::shared_ptr<InputAxisBase> spAxis)
{
	m_omAxes[name.data()] = spAxis;
}

const std::shared_ptr<InputButtonBase> InputCollector::GetButton(std::string_view name) const
{
	auto it = m_omButtons.find(name.data());

	if (it == m_omButtons.end())
	{
		assert(0 && "InputCollector::GetButton:ボタン未登録");
		
		return nullptr;
	}

	return it->second;
}

const std::shared_ptr<InputAxisBase> InputCollector::GetAxis(std::string_view name) const
{
	auto it = m_omAxes.find(name.data());

	if (it == m_omAxes.end())
	{
		assert(0 && "InputCollector::GetAxis:軸未登録");

		return nullptr;
	}

	return it->second;
}

void InputCollector::Release()
{
	m_omButtons.clear();
	m_omAxes.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputButtonForWindows
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
InputButtonForWindows::InputButtonForWindows(int keyCode)
{
	m_keyCodeList.push_back(keyCode);
}

InputButtonForWindows::InputButtonForWindows(std::initializer_list<int> keyCodeList)
{
	for (int keyCode : keyCodeList)m_keyCodeList.push_back(keyCode);
}

InputButtonForWindows::InputButtonForWindows(const std::vector<int>& keyCodeList)
{
	for (int keyCode : keyCodeList)m_keyCodeList.push_back(keyCode);
}

void InputButtonForWindows::Update()
{
	// GetAsyncKeyStateでアプリケーションに紐づけたキーコードの入力状況取得
	// 情報を基にアプリケーションボタンの入力状況を更新
	
	// 入力状況を更新済みならスキップ
	if (!m_isNeedUpdate) { return; }

	short keyState = 0;

	for (int keyCode : m_keyCodeList)
	{
		keyState |= GetAsyncKeyState(keyCode);
	}

	// ボタンが押されている
	if (keyState & 0x8000)
	{
		// 以前にボタンが押されている
		if (m_state & ButtonState::Hold)
		{
			// Pressを除外
			m_state &= ~ButtonState::Press;
		}
		// 初めて押された
		else
		{
			// PressとHoldを追加
			m_state |= ButtonState::Press;
			m_state |= ButtonState::Hold;
		}

		// Freeを除外
		m_state &= ~ButtonState::Free;
	}
	// ボタンが押されていない
	else
	{
		// 以前にボタンが押されていた
		if (m_state & ButtonState::Hold)
		{
			// Pressを除外
			m_state &= ~ButtonState::Press;
			// Holdを除外
			m_state &= ~ButtonState::Hold;
			// Releaseを追加
			m_state |= ButtonState::Release;
		}
		// 既に離されている
		else
		{
			// Releaseを除外
			m_state &= ~ButtonState::Release;
		}

		// Freeを追加
		m_state |= ButtonState::Free;
	}

	m_isNeedUpdate = false;
}

void InputButtonForWindows::GetCode(std::vector<int>& ret) const
{
	for (int code : m_keyCodeList)
	{
		ret.push_back(code);
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputAxisBase
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
Math::Vector2 InputAxisBase::GetState() const
{
	Math::Vector2 retAxis = m_axis * m_valueRate;

	retAxis.x = std::clamp(retAxis.x, -m_limitValue, m_limitValue);
	retAxis.y = std::clamp(retAxis.y, -m_limitValue, m_limitValue);

	return retAxis;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputAxisForWindows
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
InputAxisForWindows::InputAxisForWindows(int upCode, int rightCode, int downCode, int leftCode)
{
	m_dirButtons.resize(AxisDIR::Max);

	m_dirButtons[AxisDIR::Up] = std::make_shared<InputButtonForWindows>(upCode);
	m_dirButtons[AxisDIR::Right] = std::make_shared<InputButtonForWindows>(rightCode);
	m_dirButtons[AxisDIR::Down] = std::make_shared<InputButtonForWindows>(downCode);
	m_dirButtons[AxisDIR::Left] = std::make_shared<InputButtonForWindows>(leftCode);
}

InputAxisForWindows::InputAxisForWindows(std::initializer_list<int> upCodes, std::initializer_list<int> rightCodes, std::initializer_list<int> downCodes, std::initializer_list<int> leftCodes)
{
	m_dirButtons.resize(AxisDIR::Max);

	m_dirButtons[AxisDIR::Up] = std::make_shared<InputButtonForWindows>(upCodes);
	m_dirButtons[AxisDIR::Right] = std::make_shared<InputButtonForWindows>(rightCodes);
	m_dirButtons[AxisDIR::Down] = std::make_shared<InputButtonForWindows>(downCodes);
	m_dirButtons[AxisDIR::Left] = std::make_shared<InputButtonForWindows>(leftCodes);
}

InputAxisForWindows::InputAxisForWindows(const std::vector<int>& upCodes, const std::vector<int>& rightCodes, const std::vector<int>& downCodes, const std::vector<int>& leftCodes)
{
	m_dirButtons.resize(AxisDIR::Max);

	m_dirButtons[AxisDIR::Up] = std::make_shared<InputButtonForWindows>(upCodes);
	m_dirButtons[AxisDIR::Right] = std::make_shared<InputButtonForWindows>(rightCodes);
	m_dirButtons[AxisDIR::Down] = std::make_shared<InputButtonForWindows>(downCodes);
	m_dirButtons[AxisDIR::Left] = std::make_shared<InputButtonForWindows>(leftCodes);
}

InputAxisForWindows::InputAxisForWindows(const std::shared_ptr<InputButtonBase> upButton, const std::shared_ptr<InputButtonBase> rightButton, const std::shared_ptr<InputButtonBase> downButton, const std::shared_ptr<InputButtonBase> leftButton)
{
	m_dirButtons.push_back(upButton);
	m_dirButtons.push_back(rightButton);
	m_dirButtons.push_back(downButton);
	m_dirButtons.push_back(leftButton);
}

void InputAxisForWindows::PreUpdate()
{
	for (auto& button : m_dirButtons)
	{
		button->PreUpdate();
	}
}

void InputAxisForWindows::Update()
{
	// GetAsyncKeyStateで軸に紐づけた上下左右の入力状況を更新
	// 情報を基に軸の入力状況を更新
	m_axis = Math::Vector2::Zero;

	for (auto& dirButton : m_dirButtons)
	{
		dirButton->Update();
	}

	if (m_dirButtons[AxisDIR::Up]->GetState()) { m_axis.y += 1.0f; }
	if (m_dirButtons[AxisDIR::Right]->GetState()) { m_axis.x += 1.0f; }
	if (m_dirButtons[AxisDIR::Down]->GetState()) { m_axis.y -= 1.0f; }
	if (m_dirButtons[AxisDIR::Left]->GetState()) { m_axis.x -= 1.0f; }
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
// InputAxisForWindowsMouse
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// 
InputAxisForWindowsMouse::InputAxisForWindowsMouse(int fixCode)
{
	m_spFixButton = std::make_shared<InputButtonForWindows>(fixCode);
}

InputAxisForWindowsMouse::InputAxisForWindowsMouse(std::initializer_list<int> fixCodes)
{
	m_spFixButton = std::make_shared<InputButtonForWindows>(fixCodes);
}

InputAxisForWindowsMouse::InputAxisForWindowsMouse(const std::vector<int>& fixCodes)
{
	m_spFixButton = std::make_shared<InputButtonForWindows>(fixCodes);
}

InputAxisForWindowsMouse::InputAxisForWindowsMouse(const std::shared_ptr<InputButtonBase> fixButton)
{
	m_spFixButton = fixButton;
}

void InputAxisForWindowsMouse::PreUpdate()
{
	if (!m_spFixButton) { return; }

	m_spFixButton->PreUpdate();
}

void InputAxisForWindowsMouse::Update()
{
	// マウスの毎フレームの移動量で軸の入力状況を更新
	// はじめの1フレームは0ベクトル
	bool isNeedCreateAxisState = true;
	bool isNeedUpdatePrevPos = true;

	// 軸固定でボタンが押されている場合は軸情報を作成し中心を更新しない
	if (m_spFixButton)
	{
		m_spFixButton->Update();

		if(m_spFixButton->GetState())
		{
			isNeedUpdatePrevPos = false;
		}
		else
		{
			isNeedCreateAxisState = false;
		}
	}

	POINT nowPos;

	GetCursorPos(&nowPos);

	// 開始フレームではない + 軸情報の生成が必要な場合
	if (!m_isBeginFrame && isNeedCreateAxisState)
	{
		m_axis.x = float(nowPos.x - m_prevMousePos.x);
		m_axis.y = float(m_prevMousePos.y - nowPos.y);
	}
	else
	{
		m_axis = Math::Vector2::Zero;
	}

	if(isNeedUpdatePrevPos)
	{
		// 座標を保持
		// 次回以降はこの座標との差で移動量を計算
		m_prevMousePos = nowPos;
	}

	m_isBeginFrame = false;
}
