#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <Xinput.h>
#endif
#include "core/Assertion.h"
#include "core/Input.h"
#include <brofiler/Brofiler.h>
#include <cctype>

namespace tbsg {
	Input* Input::m_Instance = nullptr;

	Input::Input(int refWinSizeX, int refWinSizeY) : m_RefWinSizeX(refWinSizeX), m_RefWinSizeY(refWinSizeY)
	{
		m_Instance = this;
		const auto count = static_cast<int>(InputType::EInput_Last);

		for (bool& i : m_BeingPressed)
		{
			i = false;
		}
		
#ifdef _WIN32

		m_ControllerMapping.emplace(XINPUT_GAMEPAD_DPAD_UP, InputType::EGamepad_DPad_Up);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_DPAD_DOWN, InputType::EGamepad_DPad_Down);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_DPAD_LEFT, InputType::EGamepad_DPad_Left);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_DPAD_RIGHT, InputType::EGamepad_DPad_Right);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_START, InputType::EGamepad_Special_Right);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_BACK, InputType::EGamepad_Special_Left);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_LEFT_THUMB, InputType::EGamepad_Left_Thumb_Button);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_RIGHT_THUMB, InputType::EGamepad_Right_Thumb_Button);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_LEFT_SHOULDER, InputType::EGamepad_Left_Shoulder);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_RIGHT_SHOULDER, InputType::EGamepad_Right_Shoulder);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_A, InputType::EGamepad_Face_Button_Down);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_B, InputType::EGamepad_Face_Button_Right);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_X, InputType::EGamepad_Face_Button_Left);
		m_ControllerMapping.emplace(XINPUT_GAMEPAD_Y, InputType::EGamepad_Face_Button_Up);

#endif

		AddListener(BINDAXIS(&Input::SetControllerX, this), InputType::EGamepad_Left_Thumb_X);
		AddListener(BINDAXIS(&Input::SetControllerY, this), InputType::EGamepad_Left_Thumb_Y);

	}

	Input::~Input()
	{
	}

	void Input::Update()
	{BROFILER_CATEGORY("Input::Update", Profiler::Color::Blue)
		HandleKeyboard();
		HandleMouse();
		HandleController();
		HandleKeyboardDialog();
	}

	void Input::AddListener(BinderAxis listener, InputType inputType)
	{
		auto it = m_AxisListeners.find(inputType);

		if (it == m_AxisListeners.end())
			m_AxisListeners.emplace(inputType, ptl::vector<BinderAxis>());
		auto res = std::find(m_AxisListeners[inputType].begin(), m_AxisListeners[inputType].end(), listener);
		if (res == std::end(m_AxisListeners[inputType]))
			m_AxisListeners[inputType].push_back(listener);
	}

	void Input::AddListener(BinderInputAction listener, InputType inputType)
	{
		auto it = m_ActionListeners.find(inputType);

		if (it == m_ActionListeners.end())
			m_ActionListeners.emplace(inputType, ptl::vector<BinderInputAction>());

		m_ActionListeners[inputType].push_back(listener);
	}

	void Input::AddListener(BinderCharacter listener)
	{
		characterListeners.push_back(listener);
	}

	void Input::AddListener(BinderTextDialog listener)
	{
		textDialogListeners.push_back(listener);
	}


	void Input::PushListener(BinderAxis listener, InputType inputType)
	{
		auto it = m_AxisListeners.find(inputType);

		if (it == m_AxisListeners.end())
			m_AxisListeners.emplace(inputType, ptl::vector<BinderAxis>());

		m_AxisListeners[inputType].insert(m_AxisListeners[inputType].begin(), listener);
	}

	void Input::PushListener(BinderInputAction listener, InputType inputType)
	{
		auto it = m_ActionListeners.find(inputType);

		if (it == m_ActionListeners.end())
			m_ActionListeners.emplace(inputType, ptl::vector<BinderInputAction>());

		m_ActionListeners[inputType].insert(m_ActionListeners[inputType].begin(), listener);
	}

	void Input::PushListener(BinderCharacter listener)
	{
		characterListeners.insert(characterListeners.begin(), listener);
	}

	void Input::PushListener(BinderTextDialog listener)
	{
		textDialogListeners.insert(textDialogListeners.begin(), listener);
	}


	void Input::RemoveListener(BinderAxis listener, InputType inputType)
	{
		auto it = m_AxisListeners.find(inputType);

		if (it != m_AxisListeners.end())
		{
			auto listenerIt = it->second.begin();

			while (listenerIt != it->second.end())
			{
				if ((*listenerIt) == listener)
				{
					listenerIt = it->second.erase(listenerIt);
				}
				else
				{
					++listenerIt;
				}
			}

			if (it->second.size() == 0)
			{
				it = m_AxisListeners.erase(it);
			}
		}
	}

	void Input::RemoveListener(BinderInputAction listener, InputType inputType)
	{
		auto it = m_ActionListeners.find(inputType);

		if (it != m_ActionListeners.end())
		{
			auto listenerIt = it->second.begin();

			while (listenerIt != it->second.end())
			{
				if ((*listenerIt) == listener)
				{
					listenerIt = it->second.erase(listenerIt);
				}
				else
				{
					++listenerIt;
				}
			}

			if (it->second.size() == 0)
			{
				it = m_ActionListeners.erase(it);
			}
		}
	}

	void Input::RemoveListener(BinderCharacter listener)
	{
		auto it = characterListeners.begin();

		while (it != characterListeners.end())
		{
			if ((*it) == listener)
			{
				it = characterListeners.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void Input::RemoveListener(BinderTextDialog listener)
	{
		auto it = textDialogListeners.begin();

		while (it != textDialogListeners.end())
		{
			if ((*it) == listener)
			{
				it = textDialogListeners.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void Input::RemoveListener(void* instance, InputType inputType)
	{
		{
			auto it = m_ActionListeners.find(inputType);

			if (it != m_ActionListeners.end())
			{
				auto listenerIt = it->second.begin();

				while (listenerIt != it->second.end())
				{
					if ((*listenerIt).m_Target == instance)
					{
						listenerIt = it->second.erase(listenerIt);
					}
					else
					{
						++listenerIt;
					}
				}


				if (it->second.size() == 0)
				{
					it = m_ActionListeners.erase(it);
				}
			}
		}

		{
			auto it = m_AxisListeners.find(inputType);

			if (it != m_AxisListeners.end())
			{
				auto listenerIt = it->second.begin();

				while (listenerIt != it->second.end())
				{
					if ((*listenerIt).m_Target == instance)
					{
						listenerIt = it->second.erase(listenerIt);
					}
					else
					{
						++listenerIt;
					}
				}


				if (it->second.size() == 0)
				{
					it = m_AxisListeners.erase(it);
				}
			}
		}
	}

	void Input::RemoveListener(void* instance)
	{
		{
			auto it = m_ActionListeners.begin();


			while (it != m_ActionListeners.end())
			{
				auto listenerIt = it->second.begin();

				while (listenerIt != it->second.end())
				{
					if ((*listenerIt).m_Target == instance)
					{
						listenerIt = it->second.erase(listenerIt);
					}
					else
					{
						++listenerIt;
					}
				}

				if (it->second.size() == 0)
				{
					it = m_ActionListeners.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		{
			auto it = m_AxisListeners.begin();

			while (it != m_AxisListeners.end())
			{
				auto listenerIt = it->second.begin();

				while (listenerIt != it->second.end())
				{
					if ((*listenerIt).m_Target == instance)
					{
						listenerIt = it->second.erase(listenerIt);
					}
					else
					{
						++listenerIt;
					}
				}

				if (it->second.size() == 0)
				{
					it = m_AxisListeners.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}

	void Input::SetDeadZone(float x, float y) noexcept
	{
		m_DeadzoneX = x;
		m_DeadzoneY = y;

	}

	void Input::GetDeadZone(float* x, float* y) noexcept
	{
		if (x != nullptr)
			*x = m_DeadzoneX;

		if (y != nullptr)
			*y = m_DeadzoneY;
	}


#ifdef _WIN32
	void Input::OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_KEYDOWN: {
			const InputType type = InputType(wParam);
			SetTypeDown(type);

			ptl::string txt{};
			txt.resize(8);
			int gotTxt = GetKeyNameTextA(lParam, &txt[0], txt.size());
		} break;

		case WM_KEYUP: {
			const InputType type = InputType(wParam);
			SetTypeUp(type);
		} break;

		case WM_LBUTTONDOWN: {
			SetTypeDown(InputType::EMouse_Left);
		} break;

		case WM_LBUTTONUP: {
			SetTypeUp(InputType::EMouse_Left);
		} break;

		case WM_RBUTTONDOWN: {
			SetTypeDown(InputType::EMouse_Right);
		} break;

		case WM_RBUTTONUP: {
			SetTypeUp(InputType::EMouse_Right);
		} break;

		case WM_MBUTTONDOWN: {
			SetTypeDown(InputType::EMouse_Middle);
		} break;

		case WM_MBUTTONUP: {
			SetTypeUp(InputType::EMouse_Middle);
		} break;

		case WM_XBUTTONDOWN: {
			const UINT button = GET_XBUTTON_WPARAM(wParam);

			if (button == XBUTTON1)
			{
				SetTypeDown(InputType::EMouse_X1);
			}
			else if (button == XBUTTON2)
			{
				SetTypeDown(InputType::EMouse_X2);
			}
		} break;

		case WM_XBUTTONUP: {
			const UINT button = GET_XBUTTON_WPARAM(wParam);

			if (button == XBUTTON1)
			{
				SetTypeUp(InputType::EMouse_X1);
			}
			else if (button == XBUTTON2)
			{
				SetTypeUp(InputType::EMouse_X2);
			}
		} break;

		case WM_MOUSEWHEEL: {
			const short dir = short(HIWORD(wParam));

			FireEvents(dir > 0 ? 1.0f : -1.0f, InputType::EMouse_Wheel);

		} break;
		case WM_UNICHAR:
		case WM_CHAR: {
			char character = static_cast<char>(wParam);

			if (isprint(character))
			{
				auto it = characterListeners.begin();

				while (it != characterListeners.end())
				{
					(*it).Dispatch(character);
					it++;
				}
			}
			
		} break;
		default:;
		}
	}
#endif
	int Input::GetCursorX()
	{

		int x, y;

		GetCursorPosition(x, y);

		return x;
	}

	int Input::GetCursorY()
	{
		int x, y;
		GetCursorPosition(x, y);
		return y;
	}

	float Input::GetControllerX()
	{
		if (GetCursorX() < 0) {
			return m_JoyX;
		}
		return  static_cast<float>(GetCursorX());
	}

	float Input::GetControllerY()
	{
		if (GetCursorY() < 0) {
			return m_JoyY;
		}
		return static_cast<float>(GetCursorY());
	}

	void Input::GetCursorPosition(int& x, int& y) noexcept
	{
#ifdef PLATFORM_WINDOWS
		POINT p;
		if (GetCursorPos(&p))
		{
			if (ScreenToClient(GetActiveWindow(), &p))
			{
				RECT rect;
				if (GetClientRect(GetActiveWindow(), &rect)) {
					int width = rect.right - rect.left;
					int height = rect.bottom - rect.top;
						
					float someX = static_cast<float>(p.x) / float(width);
					float someY = static_cast<float>(p.y) / float(height);
					 
					x = static_cast<int>(someX * 1920);
					y = static_cast<int>(1080 - (someY * 1080));

				}
				else {
					x = 0;
					y = 0;
				}

			}
		}
#else
		x = 0;
		y = 0;
#endif
	}

	void Input::SetCursorPosition(int x, int y) const noexcept
	{
#ifdef PLATFORM_WINDOWS

		POINT p;

		p.x = x;
		p.y = y;

		if (ClientToScreen(GetActiveWindow(), &p)) {
			SetCursorPosition(x, y);
		}
#endif
	}

	float Input::GetControllerSpeed() const noexcept
	{
		return m_ControllerSpeed;
	}

	void Input::SetControllerSpeed(float controllerSpeed) noexcept
	{
		m_ControllerSpeed = controllerSpeed;
	}

	bool Input::SetControllerX(float x) noexcept
	{
		m_JoyX += x * m_ControllerSpeed;
		return false;
	}

	bool Input::SetControllerY(float y) noexcept
	{
		m_JoyY += -y * m_ControllerSpeed;
		return false;
	}

#ifdef PLATFORM_WINDOWS
	void Input::HandleController()
	{
		static int controllerId = -1;
		bool connected = false;

		XINPUT_STATE state{};

		if (controllerId == -1) {
			for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
			{
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				if (XInputGetState(i, &state) == ERROR_SUCCESS) {
					controllerId = i;
					connected = true;
				}
			}
		}
		else {
			connected = (XInputGetState(controllerId, &state) == ERROR_SUCCESS);
		}

		if (!connected) return;
        bool pressed = false;
		for (auto mapping : m_ControllerMapping) {
            pressed = (state.Gamepad.wButtons & mapping.first) != 0;

			if (m_BeingPressed[static_cast<int>(mapping.second)])
			{
				if (pressed)
				{
					FireEventsCross(InputAction::EBeingPressed, mapping.second);
				}
				else
				{
					m_BeingPressed[static_cast<int>(mapping.second)] = false;
					FireEventsCross(InputAction::EReleased, mapping.second);
				}
			}
			else if (pressed)
			{
				m_BeingPressed[static_cast<int>(mapping.second)] = true;
				FireEventsCross(InputAction::EBeingPressed, mapping.second);
				FireEventsCross(InputAction::EPressed, mapping.second);
			}
		}

		const float normLX = fmaxf(-1, static_cast<float>(state.Gamepad.sThumbLX) / 32767);
		const float normLY = fmaxf(-1, static_cast<float>(state.Gamepad.sThumbLY) / 32767);

		float leftStickX = (abs(normLX) < m_DeadzoneX ? 0 : (abs(normLX) - m_DeadzoneX) * (normLX / abs(normLX)));
		float leftStickY = (abs(normLY) < m_DeadzoneY ? 0 : (abs(normLY) - m_DeadzoneY) * (normLY / abs(normLY)));

		if (m_DeadzoneX > 0) leftStickX *= 1 / (1 - m_DeadzoneX);
		if (m_DeadzoneY > 0) leftStickY *= 1 / (1 - m_DeadzoneY);

		const float normRX = fmaxf(-1, static_cast<float>(state.Gamepad.sThumbRX) / 32767);
		const float normRY = fmaxf(-1, static_cast<float>(state.Gamepad.sThumbRY) / 32767);

		float rightStickX = (abs(normRX) < m_DeadzoneX ? 0 : (abs(normRX) - m_DeadzoneX) * (normRX / abs(normRX)));
		float rightStickY = (abs(normRY) < m_DeadzoneY ? 0 : (abs(normRY) - m_DeadzoneY) * (normRY / abs(normRY)));

		if (m_DeadzoneX > 0) rightStickX *= 1 / (1 - m_DeadzoneX);
		if (m_DeadzoneY > 0) rightStickY *= 1 / (1 - m_DeadzoneY);

		const float leftTrigger = static_cast<float>(state.Gamepad.bLeftTrigger) / 255;
		const float rightTrigger = static_cast<float>(state.Gamepad.bRightTrigger) / 255;

		FireEventsCross(leftStickX, InputType::EGamepad_Left_Thumb_X);
		FireEventsCross(leftStickY, InputType::EGamepad_Left_Thumb_Y);

		FireEventsCross(rightStickX, InputType::EGamepad_Right_Thumb_X);
		FireEventsCross(rightStickY, InputType::EGamepad_Right_Thumb_Y);

		FireEventsCross(leftTrigger, InputType::EGamepad_Left_Trigger);
		FireEventsCross(rightTrigger, InputType::EGamepad_Right_Trigger);



	}

#endif
	void Input::HandleKeyboard()
	{
		const int count = static_cast<int>(InputType::EKeyboard_Last);

		for (int i = 0; i < count; i++)
		{
			if (m_BeingPressed[i])
			{
				FireEventsCross(InputAction::EBeingPressed, InputType(m_BeingPressed[i]));
			}
		}
	}

	void Input::HandleKeyboardDialog()
	{
	}

	void Input::OpenInputDialog(bool obsecure, ptl::string currentText)
	{
	}

	void Input::HandleMouse()
	{
		for (int i = 1; i <= 6; i++)
		{
			if (i != 3 && m_BeingPressed[i]) {
				FireEventsCross(InputAction::EBeingPressed, InputType(i));
			}
		}
	}


	void Input::FireEvents(float value, InputType inputType)
	{

		auto typeIt = m_AxisListeners.find(inputType);

		if (typeIt != m_AxisListeners.end())
		{
			auto it = typeIt->second.begin();

			while (it != typeIt->second.end())
			{
				if (!(*it).m_Listener)
				{
					it = typeIt->second.erase(it);
				}
				else
				{
					if ((*it).Dispatch(value))
					{
						break;
					}

					++it;
				}
			}
		}
	}

	void Input::FireEvents(InputAction action, InputType inputType)
	{
		auto typeIt = m_ActionListeners.find(inputType);

		if (typeIt != m_ActionListeners.end())
		{
			auto it = typeIt->second.begin();

			while (it != typeIt->second.end())
			{
				if (!(*it).m_Listener)
				{
					it = typeIt->second.erase(it);
				}
				else
				{
					if ((*it).Dispatch(action))
					{
						break;
					}

					++it;
				}
			}
		}
	}

	void Input::FireEventsCross(float value, InputType inputType)
	{
		FireEvents(value, inputType);

		if (std::abs(value) < 0.01f)
		{
			if (m_BeingPressed[static_cast<int>(inputType)])
			{
				m_BeingPressed[static_cast<int>(inputType)] = false;
				FireEvents(InputAction::EReleased, inputType);
			}
		}
		else
		{
			if (m_BeingPressed[static_cast<int>(inputType)])
			{
				FireEvents(InputAction::EBeingPressed, inputType);
			}
			else
			{
				m_BeingPressed[static_cast<int>(inputType)] = true;
				FireEvents(InputAction::EPressed, inputType);
				FireEvents(InputAction::EBeingPressed, inputType);
			}
		}

	}

	void Input::FireEventsCross(InputAction action, InputType inputType)
	{
		float value = 0.0f;

		if (action == InputAction::EBeingPressed)
			value = 1.0f;

		FireEvents(value, inputType);

		FireEvents(action, inputType);
	}

	void Input::SetTypeDown(InputType type)
	{
		m_BeingPressed[static_cast<int>(type)] = true;
		FireEventsCross(InputAction::EPressed, type);
		FireEventsCross(InputAction::EBeingPressed, type);
	}

	void Input::SetTypeUp(InputType type)
	{
		m_BeingPressed[static_cast<int>(type)] = false;
		FireEventsCross(InputAction::EReleased, type);
	}

	bool Input::HasInterestCross(InputType inputType)
	{
		return m_ActionListeners.find(inputType) != m_ActionListeners.end() || m_AxisListeners.find(inputType) != m_AxisListeners.end();
	}
}
