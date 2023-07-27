#ifndef EVENT_HPP
#define EVENT_HPP


#include <chrono>
#include "../geometry/point2d.hpp"
#include "../sfmlwrap/rectangle.hpp"


enum class EventType
{
    NoEvent,
    Closed,
    KeyPressed, 
    KeyReleased,
    MouseWheelScrolled, 
    MouseButtonPressed, 
    MouseButtonReleased,
    MouseMoved,
    MouseEntered,
    MouseLeft, 
    TimerTicked,
    Paint,
    Error
};

enum class KeyboardKey
{
    Unknown = -1, 
    A = 0,        
    B,            
    C,            
    D,            
    E,            
    F,            
    G,            
    H,            
    I,            
    J,            
    K,            
    L,            
    M,            
    N,            
    O,            
    P,            
    Q,            
    R,            
    S,            
    T,            
    U,            
    V,            
    W,            
    X,            
    Y,            
    Z,            
    Num0,         
    Num1,         
    Num2,         
    Num3,         
    Num4,         
    Num5,         
    Num6,         
    Num7,         
    Num8,         
    Num9,         
    Escape,       
    LControl,     
    LShift,       
    LAlt,         
    LSystem,      
    RControl,     
    RShift,       
    RAlt,         
    RSystem,      
    Menu,         
    LBracket,     
    RBracket,     
    Semicolon,    
    Comma,        
    Period,       
    Quote,        
    Slash,        
    Backslash,    
    Tilde,        
    Equal,        
    Hyphen,       
    Space,        
    Enter,        
    Backspace,    
    Tab,          
    PageUp,       
    PageDown,     
    End,          
    Home,         
    Insert,       
    Delete,       
    Add,          
    Subtract,     
    Multiply,     
    Divide,       
    Left,         
    Right,        
    Up,           
    Down,         
    Numpad0,      
    Numpad1,      
    Numpad2,      
    Numpad3,      
    Numpad4,      
    Numpad5,      
    Numpad6,      
    Numpad7,      
    Numpad8,      
    Numpad9,      
    F1,           
    F2,           
    F3,           
    F4,           
    F5,           
    F6,           
    F7,           
    F8,           
    F9,           
    F10,          
    F11,          
    F12,          
    F13,          
    F14,          
    F15,          
    Pause
};

struct KeyEventData
{
    KeyboardKey key_code;
    bool alt;
    bool ctrl;
    bool shift;
};


enum class MouseButton
{
    Left,
    Right,
    Wheel,
    Additional
};

struct MouseButtonEventData
{
    MouseButton button;
    Point2d position;
};

struct MouseMoveEventData
{
    Point2d position;
};

struct MouseWheelEventData
{
    int delta;
    Point2d position;
};



enum class EventHandlerState
{
    Skipped,
    Accepted
};


class Event
{
public:
//------------------------------------------------------------------
    Event()
      : type_(EventType::Error)
    {}

    ~Event()
    {}

//------------------------------------------------------------------
Point2d get_rel_point(const Point2d &point, const Rectangle &area)
{
    return Point2d{point.x - area.get_x(),
                   point.y - area.get_y()
                  };
}
//------------------------------------------------------------------

    EventType type_ = EventType::Error;

    union
    {
        MouseButtonEventData mbedata_;
        MouseMoveEventData   mmedata_;
        MouseWheelEventData  mwedata_;
        KeyEventData         kedata_;
        std::chrono::system_clock::time_point timer_{}; 
    };
};

#endif
