#pragma once

class InputCollector;
class InputButtonBase;
class InputAxisBase;

/// <summary>
/// 入力を管理するクラス:複数のInputCollectorを管理
/// キーボードやゲームパッド等複数の入力を同時に受付可能
/// 運用はUpdateを毎ループのはじめに呼び出す必要がある
/// </summary>
class InputManager
{
public:
	static InputManager& GetInstance()
	{
		static InputManager instance;
		return instance;
	}

	/// <summary>
	/// ボタン更新
	/// </summary>
	void Update();

	/// <summary>
	/// 全ての有効な入力装置からのボタン入力状態を取得 
	/// </summary>
	/// <param name="name">取得するボタン名</param>
	/// <returns>指定したボタンの入力状態</returns>
	short GetButtonState(std::string_view name) const;

	/// <summary>
	/// ボタン入力状態を取得
	/// </summary>
	/// <param name="name">取得するボタン名</param>
	/// <returns>指定したボタンがそれぞれの入力状態と一致していればtrue</returns>
	bool IsFree(std::string_view name) const;
	bool IsPress(std::string_view name) const;
	bool IsHold(std::string_view name) const;
	bool IsRelease(std::string_view name) const;

	/// <summary>
	/// 全ての有効な入力装置からの軸入力状態を取得
	/// </summary>
	/// <param name="name">取得する軸名</param>
	/// <returns>指定した軸の入力状態</returns>
	Math::Vector2 GetAxisState(std::string_view name) const;

	/// <summary>
	/// 入力装置の登録
	/// </summary>
	/// <param name="name">登録名</param>
	/// <param name="pDevice">登録するデバイス</param>
	void AddDevice(std::string_view name, InputCollector* pDevice);
	void AddDevice(std::string_view name, std::unique_ptr<InputCollector>& upDevice);

	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <param name="name">取得するデバイス名</param>
	/// <returns>デバイスのスマートポインタ</returns>
	const std::unique_ptr<InputCollector>& GetDevice(std::string_view name) const;
	std::unique_ptr<InputCollector>& WorkDevice(std::string_view name);

	void Release();

private:
	InputManager(){}
	~InputManager() { Release(); }

	std::unordered_map<std::string, std::unique_ptr<InputCollector>> m_omInputDevices;
};

/// <summary>
/// 単一の入力デバイスからの入力をコレクションするクラス
/// キーボードやゲームパッド等それぞれのInputCollectorが必要
/// ゲームで使用する入力Index管理
/// </summary>
class InputCollector
{
public:
	enum class ActiveState
	{
		Disable,	// 無効:完全に停止している状態
		Monitoring,	// 監視:デバイスの入力更新、アプリへの影響無し
		Enable,		// 有効:アプリに入力の影響を与える
	};

	InputCollector(){}
	~InputCollector(){}

	/// <summary>
	/// ボタン・軸の入力更新
	/// </summary>
	void Update();

	/// <summary>
	/// 入力検知
	/// </summary>
	/// <returns>入力が検知されたらtrue</returns>
	bool IsInput();

	/// <summary>
	/// 入力状態の取得
	/// </summary>
	/// <param name="name">取得するボタン・軸名</param>
	/// <returns>指定したボタン・軸の入力状態</returns>
	short GetButtonState(std::string_view name)const;
	Math::Vector2 GetAxisState(std::string_view name)const;

	/// <summary>
	/// 入力デバイスの現在のステート取得
	/// </summary>
	/// <returns>現在の入力デバイスのステート</returns>
	ActiveState GetActiveState() const { return m_state; }
	/// <summary>
	/// 入力デバイスのステート設定
	/// </summary>
	/// <param name="state">設定するデバイスステート</param>
	void SetActiveState(ActiveState state) { m_state = state; }

	/// <summary>
	/// ボタン追加・上書き
	/// </summary>
	/// <param name="name">追加するボタン名</param>
	/// <param name="pButton">ボタンパラメータ</param>
	void AddButton(std::string_view name, InputButtonBase* pButton);
	void AddButton(std::string_view name, std::shared_ptr<InputButtonBase> spButton);

	/// <summary>
	/// 入力軸追加・上書き
	/// </summary>
	/// <param name="name">追加する軸名</param>
	/// <param name="pAxis">軸パラメータ</param>
	void AddAxis(std::string_view name, InputAxisBase* pAxis);
	void AddAxis(std::string_view name, std::shared_ptr<InputAxisBase> spAxis);

	/// <summary>
	/// ボタン・軸取得
	/// </summary>
	/// <param name="name">取得するボタン・軸</param>
	/// <returns>指定したボタン・軸のスマートポインタ</returns>
	const std::shared_ptr<InputButtonBase> GetButton(std::string_view name) const;
	const std::shared_ptr<InputAxisBase> GetAxis(std::string_view name) const;

private:
	void Release();

	std::unordered_map<std::string, std::shared_ptr<InputButtonBase>> m_omButtons;
	std::unordered_map<std::string, std::shared_ptr<InputAxisBase>> m_omAxes;

	ActiveState m_state = ActiveState::Enable;
};

/// <summary>
/// 単一のボタンの入力状態を保持する機能を持った基底クラス
/// ボタンの入力状態を示すBitFlag
/// BitFlagの組み合わせでボタンの入力状態をパラメータとして保持する機能
/// </summary>
class InputButtonBase
{
public:
	enum ButtonState
	{
		Free,					// 入力がない
		Press,					// 押された瞬間
		Hold = Press << 1,		// 押している間
		Release = Press << 2	// 離された瞬間
	};

	InputButtonBase(){}
	virtual ~InputButtonBase(){}

	void PreUpdate() { m_isNeedUpdate = true; }

	virtual void Update() = 0;

	/// <summary>
	/// 強制的に入力状況を解除
	/// </summary>
	void NoInput() { m_state = ButtonState::Free; }

	short GetState() const { return m_state; }

	virtual void GetCode(std::vector<int>& ret) const = 0;

protected:
	// ボタンの入力状態
	short m_state = ButtonState::Free;

	// 更新重複対策
	bool m_isNeedUpdate = true;
};

/// <summary>
/// WinAPIのGetAsyncKeyStateを利用したキー制御:マウスとキーボードの入力
/// 複数の入力キーコードを登録できる
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
/// 単一の軸の入力状態を保持する機能を持った基底クラス
/// ジョイスティックや十字キーなど入力を二次元の移動ベクトルに変換して保持
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

	// 軸の数値に書ける補正
	float m_valueRate = 1.0f;

	// 軸の限界値
	float m_limitValue = FLT_MAX;
};

/// <summary>
/// WinAPIのGetAsyncKeyState入力を利用した軸制御
/// 指定した上下左右のキー入力状況を軸情報として保持
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
/// Windowsのマウス移動を利用した軸制御
/// マウスの移動量を軸情報として保持
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