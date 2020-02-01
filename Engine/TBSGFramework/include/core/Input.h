#pragma once
#include <functional>

#include "memory/Containers.h"
#include "memory/String.h"

namespace tbsg {
    enum class InputAction
    {
        EPressed,
        EReleased,
        EBeingPressed
    };

    enum class InputType
    {
        None = 0x00, 
        EMouse_Left = 0x01, 
        EMouse_Right = 0x02, 
        Cancel = 0x03, 
        EMouse_Middle = 0x04, 
        EMouse_X1 = 0x05, 
        EMouse_X2 = 0x06, 
        
        Back = 0x08,
        Tab = 0x09,
        
        Clear = 0x0c, 
        Enter = 0x0d, 
        
        ShiftKey = 0x10, 
        ControlKey = 0x11, 
        AltKey = 0x12, 
        Pause = 0x13, 
        Capital = 0x14, 
        CapsLock = 0x14, 
        KanaMode = 0x15, 
        HanguelMode = 0x15, 
        HangulMode = 0x15, 
        
        JunjaMode = 0x17, 
        FinalMode = 0x18, 
        HanjaMode = 0x19, 
        KanjiMode = 0x19, 
        
        Escape = 0x1b, 
        IMEConvert = 0x1c, 
        IMINoConvert = 0x1d, 
        IMEAccept = 0x1e, 
        IMIModeChange = 0x1f, 
        Space = 0x20, 
        Prior = 0x21, 
        PageUp = 0x21, 
        Next = 0x22, 
        PageDown = 0x22, 
        End = 0x23, 
        Home = 0x24, 
        Left = 0x25, 
        Up = 0x26, 
        Right = 0x27, 
        Down = 0x28, 
        Select = 0x29, 
        Print = 0x2a, 
        Execute = 0x2b, 
        PrintScreen = 0x2c, 
        Snapshot = 0x2c, 
        Insert = 0x2d, 
        Delete = 0x2e, 
        Help = 0x2f, 
        D0 = 0x30, 
        D1 = 0x31, 
        D2 = 0x32, 
        D3 = 0x33, 
        D4 = 0x34, 
        D5 = 0x35, 
        D6 = 0x36, 
        D7 = 0x37, 
        D8 = 0x38, 
        D9 = 0x39, 
        
        A = 0x41, 
        B = 0x42, 
        C = 0x43, 
        D = 0x44, 
        E = 0x45, 
        F = 0x46, 
        G = 0x47, 
        H = 0x48, 
        I = 0x49, 
        J = 0x4a, 
        K = 0x4b, 
        L = 0x4c, 
        M = 0x4d, 
        N = 0x4e, 
        O = 0x4f, 
        P = 0x50, 
        Q = 0x51, 
        R = 0x52, 
        S = 0x53, 
        T = 0x54, 
        U = 0x55, 
        V = 0x56, 
        W = 0x57, 
        X = 0x58, 
        Y = 0x59, 
        Z = 0x5a, 
        LWin = 0x5b, 
        RWin = 0x5c, 
        Apps = 0x5d, 
        
        Sleep = 0x5f, 
        NumPad0 = 0x60, 
        NumPad1 = 0x61, 
        NumPad2 = 0x62, 
        NumPad3 = 0x63, 
        NumPad4 = 0x64, 
        NumPad5 = 0x65, 
        NumPad6 = 0x66, 
        NumPad7 = 0x67, 
        NumPad8 = 0x68, 
        NumPad9 = 0x69, 
        Multiply = 0x6a, 
        Add = 0x6b, 
        Separator = 0x6c, 
        Subtract = 0x6d, 
        Decimal = 0x6e, 
        Divide = 0x6f, 
        F1 = 0x70, 
        F2 = 0x71, 
        F3 = 0x72, 
        F4 = 0x73, 
        F5 = 0x74, 
        F6 = 0x75, 
        F7 = 0x76, 
        F8 = 0x77, 
        F9 = 0x78, 
        F10 = 0x79, 
        F11 = 0x7a, 
        F12 = 0x7b, 
        F13 = 0x7c, 
        F14 = 0x7d, 
        F15 = 0x7e, 
        F16 = 0x7f, 
        F17 = 0x80, 
        F18 = 0x81, 
        F19 = 0x82, 
        F20 = 0x83, 
        F21 = 0x84, 
        F22 = 0x85, 
        F23 = 0x86, 
        F24 = 0x87, 
        
        NumLock = 0x90, 
        Scroll = 0x91, 
        
        
        LShiftKey = 0xa0, 
        RShiftKey = 0xa1, 
        LControlKey = 0xa2, 
        RControlKey = 0xa3, 
        LMenu = 0xa4, 
        RMenu = 0xa5, 
        BrowserBack = 0xa6, 
        BrowserForward = 0xa7, 
        BrowserRefresh = 0xa8, 
        BrowserStop = 0xa9, 
        BrowserSearch = 0xaa, 
        BrowserFavorites = 0xab, 
        BrowserHome = 0xac, 
        VolumeMute = 0xad, 
        VolumeDown = 0xae, 
        VolumeUp = 0xaf, 
        MediaNextTrack = 0xb0, 
        MediaPreviousTrack = 0xb1, 
        MediaStop = 0xb2, 
        MediaPlayPause = 0xb3, 
        LaunchMail = 0xb4, 
        SelectMedia = 0xb5, 
        LaunchApplication1 = 0xb6, 
        LaunchApplication2 = 0xb7, 
        
        OemSemicolon = 0xba, 
        Oem1 = 0xba, 
        OemPlus = 0xbb, 
        OemComma = 0xbc, 
        OemMinus = 0xbd, 
        OemPeriod = 0xbe, 
        OemQuestion = 0xbf, 
        Oem2 = 0xbf, 
        OemTilde = 0xc0, 
        Oem3 = 0xc0, 
        
        
        OemOpenBrackets = 0xdb, 
        Oem4 = 0xdb, 
        OemPipe = 0xdc, 
        Oem5 = 0xdc, 
        OemCloseBrackets = 0xdd, 
        Oem6 = 0xdd, 
        OemQuotes = 0xde, 
        Oem7 = 0xde, 
        Oem8 = 0xdf, 
        
        
        OemBackslash = 0xe2, 
        Oem102 = 0xe2, 
        
        ProcessKey = 0xe5, 
        
        Packet = 0xe7, 
        
        
        Attn = 0xf6, 
        CrSel = 0xf7, 
        ExSel = 0xf8, 
        EraseEof = 0xf9, 
        Play = 0xfa, 
        Zoom = 0xfb, 
        NoName = 0xfc, 
        Pa1 = 0xfd, 
        OemClear = 0xfe, 

        EKeyboard_Last = 276,

        EMouse_Wheel,

        EGamepad_Left_Thumb_X,
        EGamepad_Left_Thumb_Y,
        EGamepad_Left_Trigger,
        EGamepad_Left_Shoulder,
        EGamepad_Left_Thumb_Button,

        EGamepad_Right_Thumb_X,
        EGamepad_Right_Thumb_Y,
        EGamepad_Right_Trigger,
        EGamepad_Right_Shoulder,
        EGamepad_Right_Thumb_Button,

        EGamepad_DPad_Up,
        EGamepad_DPad_Down,
        EGamepad_DPad_Left,
        EGamepad_DPad_Right,

        EGamepad_Face_Button_Up,
        EGamepad_Face_Button_Down,
        EGamepad_Face_Button_Left,
        EGamepad_Face_Button_Right,

        EGamepad_Special_Left,
        EGamepad_Special_Right,


        EInput_Last
    };


    struct Binder
    {
        Binder(void* target, ptl::string name)
        {
            m_Target = target;
            m_Name = name;
        }

        void* m_Target = nullptr;
        ptl::string m_Name;
    };


    struct BinderInputAction : Binder
    {
        template<typename T, typename F>
        BinderInputAction(F listener, ptl::string name, T target) : Binder(target, name)
        {
            m_Listener = std::bind(listener, target, std::placeholders::_1);
        }

        bool Dispatch(InputAction action) const
        {
            return m_Listener(action);
        }

        std::function<bool(InputAction)> m_Listener;

        bool operator==(const BinderInputAction& rhs) const noexcept
        {
            return m_Target == rhs.m_Target && m_Name.compare(rhs.m_Name) == 0;
        }
    };

    struct BinderAxis : Binder {

        template<typename T, typename F>
        BinderAxis(F listener, ptl::string name, T target) : Binder(target, name)
        {
            m_Listener = std::bind(listener, target, std::placeholders::_1);
        }

        bool Dispatch(float val) const
        {
            return m_Listener(val);
        }

        std::function<bool(float)> m_Listener;

        bool operator==(const BinderAxis& rhs) const noexcept
        {
            return m_Target == rhs.m_Target && m_Name.compare(rhs.m_Name) == 0;
        }
    };

	struct BinderCharacter : Binder
	{

		template<typename T, typename F>
		BinderCharacter(F listener, ptl::string name, T target) : Binder(target, name)
		{
			m_Listener = std::bind(listener, target, std::placeholders::_1);
		}

		bool Dispatch(char val) const
		{
			return m_Listener(val);
		}

		std::function<bool(char)> m_Listener;

		bool operator==(const BinderCharacter& rhs) const noexcept
		{
			return m_Target == rhs.m_Target && m_Name.compare(rhs.m_Name) == 0;
		}
	};

	struct BinderTextDialog : Binder
	{

		template<typename T, typename F>
		BinderTextDialog(F listener, ptl::string name, T target) : Binder(target, name)
		{
			m_Listener = std::bind(listener, target, std::placeholders::_1);
		}

		bool Dispatch(ptl::string val) const
		{
			return m_Listener(val);
		}

		std::function<bool(ptl::string)> m_Listener;

		bool operator==(const BinderTextDialog& rhs) const noexcept
		{
			return m_Target == rhs.m_Target && m_Name.compare(rhs.m_Name) == 0;
		}
	};


#define BINDACTION(X, Y) tbsg::BinderInputAction(X, #X, Y)
#define BINDAXIS(X, Y) tbsg::BinderAxis(X, #X, Y)
#define BINDCHARACTER(X, Y) tbsg::BinderCharacter(X, #X, Y)
#define BINDTEXTDIALOG(X, Y) tbsg::BinderTextDialog(X, #X, Y)

    class Input
    {
    public:
        Input(int refWinSizeX, int refWinSizeY);
        ~Input();

        void Update();

        void AddListener(BinderAxis listener, InputType inputType);
        void AddListener(BinderInputAction listener, InputType inputType);
		void AddListener(BinderCharacter listener);
		void AddListener(BinderTextDialog listener);

        void PushListener(BinderAxis listener, InputType inputType);
        void PushListener(BinderInputAction listener, InputType inputType);
		void PushListener(BinderCharacter listener);
		void PushListener(BinderTextDialog listener);

        void RemoveListener(BinderAxis listener, InputType inputType);
        void RemoveListener(BinderInputAction listener, InputType inputType);
		void RemoveListener(BinderCharacter listener);
		void RemoveListener(BinderTextDialog listener);

        void RemoveListener(void* instance, InputType inputType);
        void RemoveListener(void* instance);

        void SetDeadZone(float x, float y) noexcept;
        void GetDeadZone(float* x, float* y) noexcept;

        int GetCursorX();
        int GetCursorY();

        float GetControllerX();
        float GetControllerY();

        void GetCursorPosition(int& x, int& y) noexcept;
        void SetCursorPosition(int x, int y) const noexcept;


        float GetControllerSpeed() const noexcept;
        void SetControllerSpeed(float controllerSpeed) noexcept;
#ifdef _WIN32
        void OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

		static Input* m_Instance;
		void OpenInputDialog(bool obsecure = false, ptl::string currentText = "");
    private:
        void HandleController();
        void HandleKeyboard();
        void HandleMouse();
		void HandleKeyboardDialog();

        bool HasInterestCross(InputType inputType);

        void FireEvents(float value, InputType inputType);
        void FireEvents(InputAction action, InputType inputType);

        void FireEventsCross(float value, InputType inputType);
        void FireEventsCross(InputAction action, InputType inputType);

        void SetTypeDown(InputType type);
        void SetTypeUp(InputType type);

        bool SetControllerX(float x) noexcept;
        bool SetControllerY(float y) noexcept;

        float m_DeadzoneX = 0.07f;
        float m_DeadzoneY = 0.07f;

        ptl::map<InputType, ptl::vector<BinderAxis>> m_AxisListeners;
        ptl::map<InputType, ptl::vector<BinderInputAction>> m_ActionListeners;
		ptl::vector<BinderCharacter> characterListeners;
		ptl::vector<BinderTextDialog> textDialogListeners;

        bool m_BeingPressed[int(InputType::EInput_Last)] = {};

        float m_ControllerSpeed = 1.5f;
        float m_JoyX = 0.0f;
        float m_JoyY = 0.0f;
        int m_RefWinSizeX, m_RefWinSizeY;
		bool dialogOpened{};
		ptl::wstring dialogTextBuffer;

#ifdef PLATFORM_WINDOWS
        ptl::map<WORD, InputType> m_ControllerMapping;
#endif
    };
}
