//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <memory>
#include "GUI.h"
#include "Core/Engine.h"
#include "TextEditor/TextEditor.h"
#include "Rendering/RendererSettings.h"
#include "Rendering/SceneViewport.h"
#include "Rendering/ModelImporter.h"
#include "Utilities/FilesUtil.h"
#include "../Resources/Icons/Icons.h"
#include "../Utilities/Image.h"
#include <ImGui/ImGui/ImGuizmo.h>
#include <FontAwesome/IconsFontAwesome4.h>
#include <stb/stb_image.h>
#include <filesystem>



// Namespace Case_Engine
namespace Case_Engine
{
    struct Material;
    struct WindowEventData;
    struct EditorLogger;
    
	struct EditorInit
	{
		EngineInit engine_init;
	};
	class Editor
	{
		enum
		{
			Flag_Profiler,
			Flag_Camera,
			Flag_Log,
			Flag_Entities,
			Flag_HotReload,
			Flag_Renderer,
            Flag_Terrain,
			Flag_Ocean,
			Flag_Decal,
			Flag_Particles,
			Flag_Sky,
			Flag_AddEntities,
			Flag_Count
		};
	public:

        explicit Editor(EditorInit const& init);
        ~Editor();
        void OnWindowEvent(WindowEventData const&);
        void Run();
     

	public:

		bool ShowPrefernces = false;
		std::unique_ptr<Engine> engine;
		bool buttonClicked = false;
		std::string folder = "../../source/Resources";

	private:
		std::unique_ptr<GUI> gui;
        EditorLogger* editor_logger;
        tecs::entity selected_entity = tecs::null_entity;
        bool scene_focused = false;
        ImGuizmo::OPERATION gizmo_op = ImGuizmo::TRANSLATE;
        RendererSettings renderer_settings{};
        SceneViewport scene_viewport_data;

        std::array<bool, Flag_Count> window_flags = { false };


	private:

        void HandleInput();
        void MenuBar();
        void ObjectOptions();
        void SceneObjects();
        void Explorer();
        void Scene();
		void World();
		void Preferences();
        void Log();
		void ShowFolder(const std::filesystem::path& folderPath, bool isFirstNode);
		bool LoadTextureFromMemory(unsigned char* Memory, UINT size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

	};
}


