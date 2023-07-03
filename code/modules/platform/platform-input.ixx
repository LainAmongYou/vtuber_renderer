module;

export module platform:input;

import cstd;
import core;

enum class MouseButton : u8
{
    left,
    right,
    middle,
    max
};

export enum InputKey : u16
{
	l_mouse  = 0x01,
	r_mouse  = 0x02,
	m_mouse  = 0x04,
	x1_mouse = 0x05,
	x2_mouse = 0x06,
    
	tab     = 0x09,
    
	escape  = 0x1B,
    
	space   = 0x20,
    
	left    = 0x25,
	up      = 0x26,
	right   = 0x27,
	down    = 0x28,
    
	insert  = 0x2D,
	del     = 0x2E, //c++ reserves delete so shortening
    
	zero = 0x30, one, two, three, four, five, siz, seven, eight, nine, 
    
	a = 0X41, b, c, d, e, f, g, h, i, j, k, l, m, 
	n, o, p, q, r, s, t, u, v, w, x, y, z,
    
	numpad_0 = 0x60, numpad_1, numpad_2, numpad_3, numpad_4, numpad_5, numpad_6, 
	numpad_7, numpad_8, numpad_9,
    
	f1 = 0x70, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
	f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24,
    
	lshift = 0xA0,
	rshift = 0xA1,
    
	lcontrol = 0xA2,
	rcontrol = 0xA3,
    
	lalt = 0xA4,
	ralt = 0xA5,
};

struct KeyboardState
{
    std::array<bool, 256> keys;
};

struct MouseState
{
    s16 x, y;
    std::array<bool, int(MouseButton::max)> buttons;
};

export class InputSystem
{
public:
    InputSystem()
    {
        m_current_keyboard.keys.fill(false);
        
        m_current_mouse.x = 0;
        m_current_mouse.y = 0;
        m_current_mouse.buttons.fill(false);

        m_previous_keyboard = m_current_keyboard;
        m_previous_mouse = m_current_mouse;
    }

    ~InputSystem() { /* nothing to do */ }

    void update()
    {
        m_previous_keyboard = m_current_keyboard;
        m_previous_mouse = m_current_mouse;
    }

    bool process_key(const InputKey key, const bool is_pressed)
    {
        if (m_current_keyboard.keys[int(key)] != is_pressed)
        {
            m_current_keyboard.keys[int(key)] = is_pressed;
            return true;
        }
        return false;
    }

    bool process_mouse_button(const MouseButton button, const bool is_pressed)
    {
        if (m_current_mouse.buttons[int(button)] != is_pressed)
        {
            m_current_mouse.buttons[int(button)] = is_pressed;
            return true;
        }
        return false;
    }

    bool process_mouse_move(const s16 screen_x, const s16 screen_y)
    {
        if (m_current_mouse.x != screen_x || m_current_mouse.y != screen_y)
        {
            m_current_mouse.x = screen_x;
            m_current_mouse.y = screen_y;
            return true;
        }
        return false;
    }

    bool is_key_pressed(const InputKey key) const
    {
        return m_current_keyboard.keys[int(key)];
    }

    bool is_button_pressed(const MouseButton button) const
    {
        return m_current_mouse.buttons[int(button)];
    }

    void get_mouse_position(s16* mouse_x, s16* mouse_y) const
    {
        *mouse_x = m_current_mouse.x;
        *mouse_y = m_current_mouse.y;
    }

private:
    KeyboardState m_current_keyboard{};
    KeyboardState m_previous_keyboard{};
    MouseState    m_current_mouse{};
    MouseState    m_previous_mouse{};
};