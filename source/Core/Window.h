//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include "Windows.h"
#include "Utilities/Delegate.h"


// Namespace Case_Engine
namespace Case_Engine
{

	struct WindowEventData
	{
		void* handle = nullptr;
		SDL_Event Event;
		float width = 0.0f;
		float height = 0.0f;
	};

	struct WindowInit
	{
		char const* title;
		uint32_t width, height;
		bool maximize;
	};

	DECLARE_EVENT(WindowEvent, Window, WindowEventData const&);

	class Window
	{
	public:
		Window(WindowInit const& init);
		~Window();


		uint32_t PositionX() const;
		uint32_t PositionY() const;

		bool Loop();
		void Quit();

		void* Handle() const;
		uint32_t Width() const;
		uint32_t Height() const;
		bool  IsActive() const;

		WindowEvent& GetWindowEvent() { return window_event; }


		SDL_Window* window;
		SDL_Renderer* renderer = nullptr;

	

	private:


		HWND hwnd = nullptr;
		WindowEvent window_event;

	private:
		void BroadcastEvent(WindowEventData const&);
	};
}