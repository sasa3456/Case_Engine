//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Window.h"
#include "resource.h"
#include "Utilities/StringUtil.h"
#include <SDL_syswm.h>


// Namespace Case_Engine
namespace Case_Engine
{

	Window::Window(WindowInit const& init)
	{
		const std::string window_title = init.title;
		const uint32_t window_width = init.width;
		const uint32_t window_height = init.height;


		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, init.width, init.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
		if (window == nullptr) {
			// ќбработка ошибки
		}

		SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

		renderer = SDL_CreateRenderer(window, -1, 0);

          // Загрузка изображения иконки
        SDL_Surface* icon = SDL_LoadBMP("../../source/Resources/Icons/case-engine_logo.bmp");

        // Установка иконки окна
        SDL_SetWindowIcon(window, icon);

		// Get the window information
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		if (SDL_GetWindowWMInfo(window, &wmInfo)) {
			// Access the HWND on Windows
			hwnd = wmInfo.info.win.window;
			//printf("Got HWND: %p\n", hwnd);
		}
		else {
			//printf("SDL_GetWindowWMInfo failed: %s\n", SDL_GetError());
		}

        

		if (!hwnd)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Create window failed", nullptr);
			return;
		}

	}

	Window::~Window()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);     
		SDL_Quit();                  
	}

	uint32_t Window::Width() const
	{
		RECT rect{};
		GetClientRect(hwnd, &rect);
		return static_cast<uint32_t>(rect.right - rect.left);
	}
	uint32_t Window::Height() const
	{
		RECT rect{};
		GetClientRect(hwnd, &rect);
		return static_cast<uint32_t>(rect.bottom - rect.top);
	}
	uint32_t Window::PositionX() const
	{
		RECT rect{};
		GetClientRect(hwnd, &rect);
		ClientToScreen(hwnd, (LPPOINT)&rect.left);
		ClientToScreen(hwnd, (LPPOINT)&rect.right);
		return rect.left;
	}
	uint32_t Window::PositionY() const
	{
		RECT rect{};
		GetClientRect(hwnd, &rect);
		ClientToScreen(hwnd, (LPPOINT)&rect.left);
		ClientToScreen(hwnd, (LPPOINT)&rect.right);
		return rect.top;
	}

	bool Window::Loop()
	{

		return true;
	}

	void Window::Quit()
	{
		SDL_Quit();
	}

	void* Window::Handle() const
	{
		return static_cast<void*>(hwnd);
	}
	bool Window::IsActive() const
	{
		return GetForegroundWindow() == hwnd;
	}

	void Window::BroadcastEvent(WindowEventData const& data)
	{
		window_event.Broadcast(data);
	}

}
