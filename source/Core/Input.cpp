//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Input.h"
#include "Window.h"
#include "Logger.h"
#include <Windows.h>


// Namespace Case_Engine
namespace Case_Engine
{

	inline bool IsPressed(int32_t key_code)
	{
		return (::GetKeyState(key_code) & 0x8000) != 0;
	}


	Input::Input() : keys{}, prev_keys{}, input_events{}
	{
		POINT mouse_screen_pos;
		if (GetCursorPos(&mouse_screen_pos))
		{
			mouse_position_x = static_cast<float>(mouse_screen_pos.x);
			mouse_position_y = static_cast<float>(mouse_screen_pos.y);
		}
	}

	void Input::Tick()
	{
		prev_keys = std::move(keys);

		prev_mouse_position_x = mouse_position_x;
		prev_mouse_position_y = mouse_position_y;

		if (window->IsActive())
		{
			POINT mouse_screen_pos;
			if (GetCursorPos(&mouse_screen_pos))
			{
				mouse_position_x = static_cast<float>(mouse_screen_pos.x);
				mouse_position_y = static_cast<float>(mouse_screen_pos.y);
			}

			using enum KeyCode;

			keys[(uint64_t)MouseLeft] = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
			keys[(uint64_t)MouseMiddle] = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
			keys[(uint64_t)MouseRight] = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;

			keys[(uint64_t)F1] = IsPressed(VK_F1);
			keys[(uint64_t)F2] = IsPressed(VK_F2);
			keys[(uint64_t)F3] = IsPressed(VK_F3);
			keys[(uint64_t)F4] = IsPressed(VK_F4);
			keys[(uint64_t)F5] = IsPressed(VK_F5);
			keys[(uint64_t)F6] = IsPressed(VK_F6);
			keys[(uint64_t)F7] = IsPressed(VK_F7);
			keys[(uint64_t)F8] = IsPressed(VK_F8);
			keys[(uint64_t)F9] = IsPressed(VK_F9);
			keys[(uint64_t)F10] = IsPressed(VK_F10);
			keys[(uint64_t)F11] = IsPressed(VK_F11);
			keys[(uint64_t)F12] = IsPressed(VK_F12);
			keys[(uint64_t)Alpha0] = IsPressed('0');
			keys[(uint64_t)Alpha1] = IsPressed('1');
			keys[(uint64_t)Alpha2] = IsPressed('2');
			keys[(uint64_t)Alpha3] = IsPressed('3');
			keys[(uint64_t)Alpha4] = IsPressed('4');
			keys[(uint64_t)Alpha5] = IsPressed('5');
			keys[(uint64_t)Alpha6] = IsPressed('6');
			keys[(uint64_t)Alpha7] = IsPressed('7');
			keys[(uint64_t)Alpha8] = IsPressed('8');
			keys[(uint64_t)Alpha9] = IsPressed('9');
			keys[(uint64_t)Numpad0] = IsPressed(VK_NUMPAD0);
			keys[(uint64_t)Numpad1] = IsPressed(VK_NUMPAD1);
			keys[(uint64_t)Numpad2] = IsPressed(VK_NUMPAD2);
			keys[(uint64_t)Numpad3] = IsPressed(VK_NUMPAD3);
			keys[(uint64_t)Numpad4] = IsPressed(VK_NUMPAD4);
			keys[(uint64_t)Numpad5] = IsPressed(VK_NUMPAD5);
			keys[(uint64_t)Numpad6] = IsPressed(VK_NUMPAD6);
			keys[(uint64_t)Numpad7] = IsPressed(VK_NUMPAD7);
			keys[(uint64_t)Numpad8] = IsPressed(VK_NUMPAD8);
			keys[(uint64_t)Numpad9] = IsPressed(VK_NUMPAD9);
			keys[(uint64_t)Q] = IsPressed('Q');
			keys[(uint64_t)W] = IsPressed('W');
			keys[(uint64_t)E] = IsPressed('E');
			keys[(uint64_t)R] = IsPressed('R');
			keys[(uint64_t)T] = IsPressed('T');
			keys[(uint64_t)Y] = IsPressed('Y');
			keys[(uint64_t)U] = IsPressed('U');
			keys[(uint64_t)I] = IsPressed('I');
			keys[(uint64_t)O] = IsPressed('O');
			keys[(uint64_t)P] = IsPressed('P');
			keys[(uint64_t)A] = IsPressed('A');
			keys[(uint64_t)S] = IsPressed('S');
			keys[(uint64_t)D] = IsPressed('D');
			keys[(uint64_t)F] = IsPressed('F');
			keys[(uint64_t)G] = IsPressed('G');
			keys[(uint64_t)H] = IsPressed('H');
			keys[(uint64_t)J] = IsPressed('J');
			keys[(uint64_t)K] = IsPressed('K');
			keys[(uint64_t)L] = IsPressed('L');
			keys[(uint64_t)Z] = IsPressed('Z');
			keys[(uint64_t)X] = IsPressed('X');
			keys[(uint64_t)C] = IsPressed('C');
			keys[(uint64_t)V] = IsPressed('V');
			keys[(uint64_t)B] = IsPressed('B');
			keys[(uint64_t)N] = IsPressed('N');
			keys[(uint64_t)M] = IsPressed('M');
			keys[(uint64_t)Esc] = IsPressed(VK_ESCAPE);
			keys[(uint64_t)Tab] = IsPressed(VK_TAB);
			keys[(uint64_t)ShiftLeft] = IsPressed(VK_LSHIFT);
			keys[(uint64_t)ShiftRight] = IsPressed(VK_RSHIFT);
			keys[(uint64_t)CtrlLeft] = IsPressed(VK_LCONTROL);
			keys[(uint64_t)CtrlRight] = IsPressed(VK_RCONTROL);
			keys[(uint64_t)AltLeft] = IsPressed(VK_LMENU);
			keys[(uint64_t)AltRight] = IsPressed(VK_RMENU);
			keys[(uint64_t)Space] = IsPressed(VK_SPACE);
			keys[(uint64_t)CapsLock] = IsPressed(VK_CAPITAL);
			keys[(uint64_t)Backspace] = IsPressed(VK_BACK);
			keys[(uint64_t)Enter] = IsPressed(VK_RETURN);
			keys[(uint64_t)Delete] = IsPressed(VK_DELETE);
			keys[(uint64_t)ArrowLeft] = IsPressed(VK_LEFT);
			keys[(uint64_t)ArrowRight] = IsPressed(VK_RIGHT);
			keys[(uint64_t)ArrowUp] = IsPressed(VK_UP);
			keys[(uint64_t)ArrowDown] = IsPressed(VK_DOWN);
			keys[(uint64_t)PageUp] = IsPressed(VK_PRIOR);
			keys[(uint64_t)PageDown] = IsPressed(VK_NEXT);
			keys[(uint64_t)Home] = IsPressed(VK_HOME);
			keys[(uint64_t)End] = IsPressed(VK_END);
			keys[(uint64_t)Insert] = IsPressed(VK_INSERT);

			if (GetKey(KeyCode::Esc)) PostQuitMessage(0);
		}

	}
	void Input::OnWindowEvent(WindowEventData data)
	{
		
	}
	void Input::SetMouseVisibility(bool visible)
	{
		ShowCursor(visible);
	}
	void Input::SetMousePosition(float xpos, float ypos)
	{
		HWND handle = static_cast<HWND>(window->Handle());
		if (handle == GetActiveWindow())
		{
			POINT mouse_screen_pos = POINT{ static_cast<LONG>(xpos), static_cast<LONG>(ypos) };
			if (ClientToScreen(handle, &mouse_screen_pos))  SetCursorPos(mouse_screen_pos.x, mouse_screen_pos.y);
		}
	}

}