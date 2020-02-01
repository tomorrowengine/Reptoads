#pragma once





#include "KeyCodes.h"


class EventArgs
{
public:
    EventArgs()
    {}

};

class KeyEventArgs : public EventArgs
{
public:
    enum KeyState
    {
        Released = 0,
        Pressed = 1
    };

    typedef EventArgs base;
    KeyEventArgs( KeyCode::Key key, unsigned int c, KeyState state, bool control, bool shift, bool alt )
        : Key(key)
        , Char(c)
        , State(state)
        , Control(control)
        , Shift(shift)
        , Alt(alt)
    {}

    KeyCode::Key    Key;    
    unsigned int    Char;   
    KeyState        State;  
    bool            Control;
    bool            Shift;  
    bool            Alt;    
};

class MouseMotionEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    MouseMotionEventArgs( bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y )
        : LeftButton( leftButton )
        , MiddleButton( middleButton )
        , RightButton( rightButton )
        , Control( control )
        , Shift( shift )
        , X( x )
        , Y( y )
    {}

    bool LeftButton;    
    bool MiddleButton;  
    bool RightButton;   
    bool Control;       
    bool Shift;         

    int X;              
    int Y;              
    int RelX;           
    int RelY;           

};

class MouseButtonEventArgs : public EventArgs
{
public:
    enum MouseButton
    {
        None = 0,
        Left = 1,
        Right = 2,
        Middel = 3
    };
    enum ButtonState
    {
        Released = 0,
        Pressed = 1
    };

    typedef EventArgs base;
    MouseButtonEventArgs( MouseButton buttonID, ButtonState state, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y )
        : Button( buttonID )
        , State( state )
        , LeftButton( leftButton )
        , MiddleButton( middleButton )
        , RightButton( rightButton )
        , Control( control )
        , Shift( shift )
        , X( x )
        , Y( y )
    {}

    MouseButton Button; 
    ButtonState State;  
    bool LeftButton;    
    bool MiddleButton;  
    bool RightButton;   
    bool Control;       
    bool Shift;         

    int X;              
    int Y;              
};

class MouseWheelEventArgs: public EventArgs
{
public:
    typedef EventArgs base;
    MouseWheelEventArgs( float wheelDelta, bool leftButton, bool middleButton, bool rightButton, bool control, bool shift, int x, int y )
        : WheelDelta( wheelDelta )
        , LeftButton( leftButton )
        , MiddleButton( middleButton )
        , RightButton( rightButton )
        , Control( control )
        , Shift( shift )
        , X( x )
        , Y( y )
    {}

    float WheelDelta;   
    bool LeftButton;    
    bool MiddleButton;  
    bool RightButton;   
    bool Control;       
    bool Shift;         

    int X;              
    int Y;              

};

class ResizeEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    ResizeEventArgs( int width, int height )
        : Width( width )
        , Height( height )
    {}

    
    int Width;
    
    int Height;

};

class UpdateEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    UpdateEventArgs(double fDeltaTime, double fTotalTime, uint64_t frameNumber)
        : ElapsedTime(fDeltaTime)
        , TotalTime(fTotalTime)
        , FrameNumber(frameNumber)
    {}

    double ElapsedTime;
    double TotalTime;
    uint64_t FrameNumber;
};

class RenderEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    RenderEventArgs(double fDeltaTime, double fTotalTime, uint64_t frameNumber)
        : ElapsedTime(fDeltaTime)
        , TotalTime(fTotalTime)
        , FrameNumber(frameNumber)
    {}

    double ElapsedTime;
    double TotalTime;
    uint64_t FrameNumber;
};

class UserEventArgs : public EventArgs
{
public:
    typedef EventArgs base;
    UserEventArgs( int code, void* data1, void* data2 )
        : Code( code )
        , Data1( data1 )
        , Data2( data2 )
    {}

    int     Code;
    void*   Data1;
    void*   Data2;
};
