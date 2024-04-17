//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_internal.h"


// Namespace Case_Engine
namespace Case_Engine
{
	class GfxDevice;
	struct WindowEventData;

	class GUI
	{
	public:

		explicit GUI(GfxDevice* gfx);
		~GUI();
		void Begin(GfxDevice* gfx) const;
		void End() const;
		void HandleWindowMessage(WindowEventData const&) const;
		void ToggleVisibility();
		bool IsVisible() const;

	private:
		std::string ini_file;
		bool visible = true;
	};
}