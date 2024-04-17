//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Editor.h"
#include "EditorLogger.h"
#include <nfd.h>
#include "Core/Logger.h"
#include "Core/Paths.h"
#include "Core/Window.h"
#include "Rendering/Renderer.h"
#include "Graphics/GfxDevice.h"
#include "Utilities/StringUtil.h"
#include "Utilities/Random.h"
#include "ImGui/ImGuiUtil.h"
#include <filesystem>


// Namespace Case_Engine
namespace Case_Engine
{

	struct ProfilerState
	{
		bool  show_average;
		struct AccumulatedTimeStamp
		{
			float sum;
			float minimum;
			float maximum;

			AccumulatedTimeStamp()
				: sum(0.0f), minimum(FLT_MAX), maximum(0)
			{
			}
		};

		std::vector<AccumulatedTimeStamp> displayed_timestamps;
		std::vector<AccumulatedTimeStamp> accumulating_timestamps;
		double last_reset_time;
		uint32_t accumulating_frame_count;
	};

    Editor::Editor(EditorInit const& init) : engine()
    {
        engine = std::make_unique<Engine>(init.engine_init);
        gui = std::make_unique<GUI>(engine->gfx.get());
		editor_logger = new EditorLogger();
        CASE_ENGINE_REGISTER_LOGGER(editor_logger);
		SetImGuiStyle();
    }

    Editor::~Editor()
    {

    }

	void Editor::OnWindowEvent(WindowEventData const& msg_data)
    {
        engine->OnWindowEvent(msg_data);
        gui->HandleWindowMessage(msg_data);
    }

    void Editor::Run()
    {
        HandleInput();
        
        if (gui->IsVisible())
        {
            engine->SetSceneViewportData(scene_viewport_data);
            engine->Run(renderer_settings);
            engine->gfx->ClearBackbuffer();
            engine->gfx->SetBackbuffer();
            gui->Begin(engine->gfx.get());
            {
				ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
                MenuBar();
				ObjectOptions();
				Log();
				Explorer();
				TextEditor::EditorText();
                Scene();
				World();
				SceneObjects();
				Preferences();

            }
            gui->End();
            engine->Present();
        }
        else
        {
            engine->SetSceneViewportData(std::nullopt);
            engine->Run(renderer_settings);
            engine->Present();
        }
    }

    void Editor::HandleInput()
    {
		if (scene_focused && g_Input.IsKeyDown(KeyCode::I))
		{
			gui->ToggleVisibility();
			g_Input.SetMouseVisibility(gui->IsVisible());
		}
        if (gui->IsVisible())
        {
            if ( g_Input.IsKeyDown(KeyCode::T)) gizmo_op = ImGuizmo::TRANSLATE;
            if ( g_Input.IsKeyDown(KeyCode::R)) gizmo_op = ImGuizmo::ROTATE;
            if ( g_Input.IsKeyDown(KeyCode::E)) gizmo_op = ImGuizmo::SCALE; 
        }
        engine->camera->Enable(scene_focused);
    }

	void Editor::MenuBar()
    {
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Model"))
				{
					nfdchar_t* file_path = NULL;
					const nfdchar_t* filter_list = "gltf";
					nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
					if (result == NFD_OKAY)
					{
						std::string model_path = file_path;

						ModelParameters params{};
						params.model_path = model_path;
						std::string texture_path = GetParentPath(model_path);
						if (!texture_path.empty()) texture_path.append("/");

						params.textures_path = texture_path;
						engine->model_importer->ImportModel_GLTF(params);
						free(file_path);
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{

				if (ImGui::MenuItem("Preferences"))
				{
					ShowPrefernces = true;

				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Assets"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Objects"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Plugins"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{

				if (ImGui::MenuItem("Profiler", 0, window_flags[Flag_Profiler]))			 window_flags[Flag_Profiler] = !window_flags[Flag_Profiler];

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
    }

	void Editor::Scene()
	{
		auto& camera = *engine->camera;

		Matrix camera_view = camera.View();
		Matrix camera_proj = camera.Proj();

		ImGui::Begin("Scene");
		{


			ImVec2 v_min = ImGui::GetWindowContentRegionMin();
			ImVec2 v_max = ImGui::GetWindowContentRegionMax();
			v_min.x += ImGui::GetWindowPos().x;
			v_min.y += ImGui::GetWindowPos().y;
			v_max.x += ImGui::GetWindowPos().x;
			v_max.y += ImGui::GetWindowPos().y;
			ImVec2 size(v_max.x - v_min.x, v_max.y - v_min.y);

			scene_focused = ImGui::IsWindowFocused();
			ImGui::Image(engine->renderer->GetOffscreenTexture()->SRV(), size);

			ImVec2 mouse_pos = ImGui::GetMousePos();
			scene_viewport_data.mouse_position_x = mouse_pos.x;
			scene_viewport_data.mouse_position_y = mouse_pos.y;
			scene_viewport_data.scene_viewport_focused = scene_focused;
			scene_viewport_data.scene_viewport_pos_x = v_min.x;
			scene_viewport_data.scene_viewport_pos_y = v_min.y;
			scene_viewport_data.scene_viewport_size_x = size.x;
			scene_viewport_data.scene_viewport_size_y = size.y;


		}


		if (scene_focused)
		{
			ImGui::SetCursorPos(ImVec2(20, 250));
			if (ImGui::BeginChild("Guizmo", ImVec2(50, 150)))
			{


				if (ImGui::Button("") || ImGui::IsItemClicked())
				{
					gizmo_op = ImGuizmo::TRANSLATE;
				}


				if (ImGui::Button(ICON_FA_UNDO "") || ImGui::IsItemClicked())
				{
					gizmo_op = ImGuizmo::ROTATE;
				}

				if (ImGui::Button(ICON_FA_EXPAND "") || ImGui::IsItemClicked())
				{
					gizmo_op = ImGuizmo::SCALE;
				}

				ImGui::EndChild();
			}
		}


		if (selected_entity != tecs::null_entity && engine->reg.has<Transform>(selected_entity))
		{
			ImGuizmo::SetDrawlist();

			ImVec2 window_size = ImGui::GetWindowSize();
			ImVec2 window_pos = ImGui::GetWindowPos();

			ImGuizmo::SetRect(window_pos.x, window_pos.y,
				window_size.x, window_size.y);

			Transform& entity_transform = engine->reg.get<Transform>(selected_entity);

			Matrix tr = entity_transform.current_transform;

			ImGuizmo::Manipulate(camera_view.m[0], camera_proj.m[0], gizmo_op, ImGuizmo::LOCAL, tr.m[0]);

			if (ImGuizmo::IsUsing())
			{
				AABB* aabb = engine->reg.get_if<AABB>(selected_entity);
				if (aabb)
				{
					aabb->bounding_box.Transform(aabb->bounding_box, entity_transform.current_transform.Invert());
					aabb->bounding_box.Transform(aabb->bounding_box, tr);
					aabb->UpdateBuffer(engine->gfx.get());
				}

				if (Relationship* relationship = engine->reg.get_if<Relationship>(selected_entity))
				{
					for (uint32_t i = 0; i < relationship->children_count; ++i)
					{
						tecs::entity child = relationship->children[i];
						if (AABB* aabb = engine->reg.get_if<AABB>(child))
						{
							aabb->bounding_box.Transform(aabb->bounding_box, entity_transform.current_transform.Invert());
							aabb->bounding_box.Transform(aabb->bounding_box, tr);
							aabb->UpdateBuffer(engine->gfx.get());


						}
					}
				}
				entity_transform.current_transform = tr;
			}


		}
        

        

		if (window_flags[Flag_Profiler])
		{
			ImGui::SetCursorPos(ImVec2(150, 80));
			if (ImGui::BeginChild("Profiling", ImVec2(300, 600)))
			{

				ImGuiIO io = ImGui::GetIO();
				static bool enable_profiling = true;
				if (enable_profiling)
				{
					static ProfilerState state;
					static constexpr uint64_t NUM_FRAMES = 128;
					static float FRAME_TIME_ARRAY[NUM_FRAMES] = { 0 };
					static float RECENT_HIGHEST_FRAME_TIME = 0.0f;
					static constexpr int32_t FRAME_TIME_GRAPH_MAX_FPS[] = { 800, 240, 120, 90, 65, 45, 30, 15, 10, 5, 4, 3, 2, 1 };
					static float FRAME_TIME_GRAPH_MAX_VALUES[ARRAYSIZE(FRAME_TIME_GRAPH_MAX_FPS)] = { 0 };
					for (uint64_t i = 0; i < ARRAYSIZE(FRAME_TIME_GRAPH_MAX_FPS); ++i) { FRAME_TIME_GRAPH_MAX_VALUES[i] = 1000.f / FRAME_TIME_GRAPH_MAX_FPS[i]; }

					std::vector<Timestamp> time_stamps = engine->renderer->GetProfilerResults();
					FRAME_TIME_ARRAY[NUM_FRAMES - 1] = 1000.0f / io.Framerate;
					for (uint32_t i = 0; i < NUM_FRAMES - 1; i++) FRAME_TIME_ARRAY[i] = FRAME_TIME_ARRAY[i + 1];
					RECENT_HIGHEST_FRAME_TIME = std::max(RECENT_HIGHEST_FRAME_TIME, FRAME_TIME_ARRAY[NUM_FRAMES - 1]);
					float frameTime_ms = FRAME_TIME_ARRAY[NUM_FRAMES - 1];
					const int32_t fps = static_cast<int32_t>(1000.0f / frameTime_ms);

					ImGui::Text("FPS        : %d (%.2f ms)", fps, frameTime_ms);


					uint64_t i_max = 0;
					for (uint64_t i = 0; i < ARRAYSIZE(FRAME_TIME_GRAPH_MAX_VALUES); ++i)
					{
						if (RECENT_HIGHEST_FRAME_TIME < FRAME_TIME_GRAPH_MAX_VALUES[i]) // FRAME_TIME_GRAPH_MAX_VALUES are in increasing order
						{
							i_max = std::min(ARRAYSIZE(FRAME_TIME_GRAPH_MAX_VALUES) - 1, i + 1);
							break;
						}
					}
					ImGui::PlotLines("", FRAME_TIME_ARRAY, NUM_FRAMES, 0, "GPU frame time (ms)", 0.0f, FRAME_TIME_GRAPH_MAX_VALUES[i_max], ImVec2(0, 80));

					constexpr uint32_t avg_timestamp_update_interval = 1000;
					static auto MillisecondsNow = []()
						{
							static LARGE_INTEGER s_frequency;
							static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
							double milliseconds = 0;
							if (s_use_qpc)
							{
								LARGE_INTEGER now;
								QueryPerformanceCounter(&now);
								milliseconds = double(1000.0 * now.QuadPart) / s_frequency.QuadPart;
							}
							else milliseconds = double(GetTickCount64());
							return milliseconds;
						};
					const double current_time = MillisecondsNow();

					bool reset_accumulating_state = false;
					if ((state.accumulating_frame_count > 1) &&
						((current_time - state.last_reset_time) > avg_timestamp_update_interval))
					{
						std::swap(state.displayed_timestamps, state.accumulating_timestamps);
						for (uint32_t i = 0; i < state.displayed_timestamps.size(); i++)
						{
							state.displayed_timestamps[i].sum /= state.accumulating_frame_count;
						}
						reset_accumulating_state = true;
					}

					reset_accumulating_state |= (state.accumulating_timestamps.size() != time_stamps.size());
					if (reset_accumulating_state)
					{
						state.accumulating_timestamps.resize(0);
						state.accumulating_timestamps.resize(time_stamps.size());
						state.last_reset_time = current_time;
						state.accumulating_frame_count = 0;
					}

					float total_time_ms = 0.0f;
					for (uint64_t i = 0; i < time_stamps.size(); i++)
					{
						float value = time_stamps[i].time_in_ms;
						char const* pStrUnit = "ms";
						ImGui::Text("%-18s: %7.2f %s", time_stamps[i].name.c_str(), value, pStrUnit);

					}
					ImGui::Text("Total: %7.2f %s", total_time_ms, "ms");
					state.accumulating_frame_count++;

				}
				engine->renderer->SetProfiling(enable_profiling);

				ImGui::EndChild();
			}


		}


        ImGui::End();
    }

	void Editor::World()
	{
		ImGui::Begin(ICON_FA_GLOBE " World");

		if (ImGui::TreeNode("Volumetric Clouds"))
		{
			

			ImGui::Checkbox("Volumetric Clouds", &renderer_settings.clouds);
			ImGui::Checkbox("Depth of Field", &renderer_settings.dof);
			ImGui::Checkbox("Bloom", &renderer_settings.bloom);


			if (renderer_settings.clouds && ImGui::TreeNodeEx("Volumetric Clouds", 0))
			{
				ImGui::SliderFloat("Sun light absorption", &renderer_settings.light_absorption, 0.0f, 0.015f);
				ImGui::SliderFloat("Clouds bottom height", &renderer_settings.clouds_bottom_height, 1000.0f, 10000.0f);
				ImGui::SliderFloat("Clouds top height", &renderer_settings.clouds_top_height, 10000.0f, 50000.0f);
				ImGui::SliderFloat("Density", &renderer_settings.density_factor, 0.0f, 1.0f);
				ImGui::SliderFloat("Crispiness", &renderer_settings.crispiness, 0.0f, 100.0f);
				ImGui::SliderFloat("Curliness", &renderer_settings.curliness, 0.0f, 5.0f);
				ImGui::SliderFloat("Coverage", &renderer_settings.coverage, 0.0f, 1.0f);
				ImGui::SliderFloat("Cloud Type", &renderer_settings.cloud_type, 0.0f, 1.0f);
				renderer_settings.recreate_initial_spectrum = ImGui::SliderFloat2("Wind Direction", renderer_settings.wind_direction, 0.0f, 50.0f);
				ImGui::SliderFloat("Wind Speed Factor", &renderer_settings.wind_speed, 0.0f, 100.0f);
				ImGui::TreePop();
				ImGui::Separator();
			}
			
			if (renderer_settings.dof && ImGui::TreeNodeEx("Depth Of Field", 0))
			{

				ImGui::SliderFloat("DoF Near Blur", &renderer_settings.dof_near_blur, 0.0f, 200.0f);
				ImGui::SliderFloat("DoF Near", &renderer_settings.dof_near, renderer_settings.dof_near_blur, 500.0f);
				ImGui::SliderFloat("DoF Far", &renderer_settings.dof_far, renderer_settings.dof_near, 1000.0f);
				ImGui::SliderFloat("DoF Far Blur", &renderer_settings.dof_far_blur, renderer_settings.dof_far, 1500.0f);
				ImGui::Checkbox("Bokeh", &renderer_settings.bokeh);

				if (renderer_settings.bokeh)
				{
					static char const* const bokeh_types[] = { "HEXAGON", "OCTAGON", "CIRCLE", "CROSS" };
					static int bokeh_type_i = static_cast<int>(renderer_settings.bokeh_type);
					ImGui::ListBox("Bokeh Type", &bokeh_type_i, bokeh_types, IM_ARRAYSIZE(bokeh_types));
					renderer_settings.bokeh_type = static_cast<BokehType>(bokeh_type_i);

					ImGui::SliderFloat("Bokeh Blur Threshold", &renderer_settings.bokeh_blur_threshold, 0.0f, 1.0f);
					ImGui::SliderFloat("Bokeh Lum Threshold", &renderer_settings.bokeh_lum_threshold, 0.0f, 10.0f);
					ImGui::SliderFloat("Bokeh Color Scale", &renderer_settings.bokeh_color_scale, 0.1f, 10.0f);
					ImGui::SliderFloat("Bokeh Max Size", &renderer_settings.bokeh_radius_scale, 0.0f, 100.0f);
					ImGui::SliderFloat("Bokeh Fallout", &renderer_settings.bokeh_fallout, 0.0f, 2.0f);
				}
				ImGui::TreePop();
				ImGui::Separator();

			}
			if (renderer_settings.bloom && ImGui::TreeNodeEx("Bloom", 0))
			{
				ImGui::SliderFloat("Threshold", &renderer_settings.bloom_threshold, 0.1f, 2.0f);
				ImGui::SliderFloat("Bloom Scale", &renderer_settings.bloom_scale, 0.1f, 5.0f);
				ImGui::TreePop();
				ImGui::Separator();
			}
			

			if (ImGui::TreeNodeEx("Film Effects", ImGuiTreeNodeFlags_None))
			{
				ImGui::Checkbox("Lens Distortion", &renderer_settings.lens_distortion_enabled);
				ImGui::Checkbox("Chromatic Aberration", &renderer_settings.chromatic_aberration_enabled);
				ImGui::Checkbox("Vignette", &renderer_settings.vignette_enabled);
				ImGui::Checkbox("Film Grain", &renderer_settings.film_grain_enabled);
				if (renderer_settings.lens_distortion_enabled)
				{
					ImGui::SliderFloat("Lens Distortion Intensity", &renderer_settings.lens_distortion_intensity, -1.0f, 1.0f);
				}
				if (renderer_settings.chromatic_aberration_enabled)
				{
					ImGui::SliderFloat("Chromatic Aberration Intensity", &renderer_settings.chromatic_aberration_intensity, 0.0f, 40.0f);
				}
				if (renderer_settings.vignette_enabled)
				{
					ImGui::SliderFloat("Vignette Intensity", &renderer_settings.vignette_intensity, 0.0f, 2.0f);
				}
				if (renderer_settings.film_grain_enabled)
				{
					ImGui::SliderFloat("Film Grain Scale", &renderer_settings.film_grain_scale, 0.01f, 20.0f);
					ImGui::SliderFloat("Film Grain Amount", &renderer_settings.film_grain_amount, 0.0f, 20.0f);
					ImGui::SliderFloat("Film Grain Seed Update Rate", &renderer_settings.film_grain_seed_update_rate, 0.0f, 0.1f);
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Tone Mapping", 0))
			{
				ImGui::SliderFloat("Exposure", &renderer_settings.tone_map_exposure, 0.01f, 10.0f);
				static char const* const operators[] = { "REINHARD", "HABLE", "LINEAR" };
				static int tone_map_operator = static_cast<int>(renderer_settings.tone_map_op);
				ImGui::ListBox("Tone Map Operator", &tone_map_operator, operators, IM_ARRAYSIZE(operators));
				renderer_settings.tone_map_op = static_cast<ToneMap>(tone_map_operator);
				ImGui::TreePop();
				ImGui::Separator();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Fog"))
		{
			ImGui::Checkbox("Fog", &renderer_settings.fog);

			if (renderer_settings.fog && ImGui::TreeNodeEx("Settings Fog", 0))
			{
				const char* fog_types[] = { "Exponential", "Exponential Height" };
				static int current_fog_type = 0;
				const char* combo_label = fog_types[current_fog_type];
				if (ImGui::BeginCombo("Type", combo_label, 0))
				{
					for (int n = 0; n < IM_ARRAYSIZE(fog_types); n++)
					{
						const bool is_selected = (current_fog_type == n);
						if (ImGui::Selectable(fog_types[n], is_selected)) current_fog_type = n;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				renderer_settings.fog_type = static_cast<FogType>(current_fog_type);

				ImGui::SliderFloat("Falloff", &renderer_settings.fog_falloff, 0.0001f, 0.01f);
				ImGui::SliderFloat("Density", &renderer_settings.fog_density, 0.0001f, 0.01f);
				ImGui::SliderFloat("Start", &renderer_settings.fog_start, 0.1f, 10000.0f);
				ImGui::ColorEdit3("Color", renderer_settings.fog_color);

				ImGui::TreePop();
				ImGui::Separator();
			}

			ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("Point Light"))
		{
			static int light_count_to_add = 1;
			ImGui::SliderInt("Light Count", &light_count_to_add, 1, 128);
			if (ImGui::Button("Create Random Lights"))
			{
				static RealRandomGenerator real(0.0f, 1.0f);

				for (int32_t i = 0; i < light_count_to_add; ++i)
				{
					LightParameters light_params{};
					light_params.light_data.casts_shadows = false;
					light_params.light_data.color = Vector4(real() * 2, real() * 2, real() * 2, 1.0f);
					light_params.light_data.direction = Vector4(0.5f, -1.0f, 0.1f, 0.0f);
					light_params.light_data.position = Vector4(engine->camera->Position());
					light_params.light_data.type = LightType::Point;
					light_params.mesh_type = LightMesh::Quad;
					light_params.light_data.range = real() * 100.0f + 40.0f;
					light_params.light_data.active = true;
					light_params.light_data.volumetric = false;
					light_params.light_data.volumetric_strength = 1.0f;
					engine->model_importer->LoadLight(light_params);
				}
			}
			
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Ocean"))
		{
			static GridParameters ocean_params{};
			static int32_t tile_count[2] = { 512, 512 };
			static float tile_size[2] = { 40.0f, 40.0f };
			static float texture_scale[2] = { 20.0f, 20.0f };

			ImGui::SliderInt2("Tile Count", tile_count, 32, 1024);
			ImGui::SliderFloat2("Tile Size", tile_size, 1.0, 100.0f);
			ImGui::SliderFloat2("Texture Scale", texture_scale, 0.1f, 10.0f);

			ocean_params.tile_count_x = tile_count[0];
			ocean_params.tile_count_z = tile_count[1];
			ocean_params.tile_size_x = tile_size[0];
			ocean_params.tile_size_z = tile_size[1];
			ocean_params.texture_scale_x = texture_scale[0];
			ocean_params.texture_scale_z = texture_scale[1];

			if (ImGui::Button("Load Ocean"))
			{
				OceanParameters params{};
				params.ocean_grid = std::move(ocean_params);
				engine->model_importer->LoadOcean(params);
			}

			if (ImGui::Button("Clear"))
			{
				engine->reg.destroy<Ocean>();
			}

			if (ImGui::TreeNodeEx("Ocean Settings", 0))
			{
				ImGui::Checkbox("Tessellation", &renderer_settings.ocean_tesselation);
				ImGui::Checkbox("Wireframe", &renderer_settings.ocean_wireframe);

				ImGui::SliderFloat("Choppiness", &renderer_settings.ocean_choppiness, 0.0f, 10.0f);
				renderer_settings.ocean_color_changed = ImGui::ColorEdit3("Ocean Color", renderer_settings.ocean_color);
				ImGui::TreePop();
				ImGui::Separator();
			}

			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Particles"))
		{
			static EmitterParameters params{};
			static char NAME_BUFFER[128];
			ImGui::InputText("Name", NAME_BUFFER, sizeof(NAME_BUFFER));
			params.name = std::string(NAME_BUFFER);
			if (ImGui::Button("Select Texture"))
			{
				nfdchar_t* file_path = NULL;
				nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
				nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
				if (result == NFD_OKAY)
				{
					std::string texture_path(file_path);
					params.texture_path = texture_path;
					free(file_path);
				}
			}
			ImGui::Text(params.texture_path.c_str());

			ImGui::SliderFloat3("Position", params.position, -500.0f, 500.0f);
			ImGui::SliderFloat3("Velocity", params.velocity, -50.0f, 50.0f);
			ImGui::SliderFloat3("Position Variance", params.position_variance, -50.0f, 50.0f);
			ImGui::SliderFloat("Velocity Variance", &params.velocity_variance, -10.0f, 10.0f);
			ImGui::SliderFloat("Lifespan", &params.lifespan, 0.0f, 50.0f);
			ImGui::SliderFloat("Start Size", &params.start_size, 0.0f, 50.0f);
			ImGui::SliderFloat("End Size", &params.end_size, 0.0f, 10.0f);
			ImGui::SliderFloat("Mass", &params.mass, 0.0f, 10.0f);
			ImGui::SliderFloat("Particles Per Second", &params.particles_per_second, 1.0f, 1000.0f);
			ImGui::Checkbox("Alpha Blend", &params.blend);
			ImGui::Checkbox("Collisions", &params.collisions);
			ImGui::Checkbox("Sort", &params.sort);
			if (params.collisions) ImGui::SliderInt("Collision Thickness", &params.collision_thickness, 0, 40);

			if (ImGui::Button("Load Emitter"))
			{
				engine->model_importer->LoadEmitter(params);
			}
			if (ImGui::Button("Clear Particle Emitters"))
			{
				engine->reg.destroy<Emitter>();
			}

			ImGui::TreePop();
		}
	
		
		ImGui::End();
	}

	void Editor::SceneObjects()
	{
		auto all_entities = engine->reg.view<Tag>();

		ImGui::Begin(ICON_FA_TH_LIST " Scene Objects");

		ImVec2 v_min = ImGui::GetWindowContentRegionMin();
		ImVec2 v_max = ImGui::GetWindowContentRegionMax();
		v_min.x += ImGui::GetWindowPos().x;
		v_min.y += ImGui::GetWindowPos().y;
		v_max.x += ImGui::GetWindowPos().x;
		v_max.y += ImGui::GetWindowPos().y;
		ImVec2 size(v_max.x - v_min.x, v_max.y - v_min.y);


			std::function<void(tecs::entity, bool)> ShowEntity;
			ShowEntity = [&](tecs::entity e, bool first_iteration)
				{
					Relationship* relationship = engine->reg.get_if<Relationship>(e);
					if (first_iteration && relationship && relationship->parent != tecs::null_entity) return;
					auto& tag = all_entities.get(e);

					ImGuiTreeNodeFlags flags = ((selected_entity == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
					flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
					bool opened = ImGui::TreeNodeEx(tag.name.c_str(), flags);

					if (ImGui::IsItemClicked())
					{
						if (e == selected_entity) selected_entity = tecs::null_entity;
						else selected_entity = e;
					}

					if (opened)
					{
						if (relationship)
						{
							for (size_t i = 0; i < relationship->children_count; ++i)
							{
								ShowEntity(relationship->children[i], false);
							}
						}
						ImGui::TreePop();
					}
				};
			for (auto e : all_entities) ShowEntity(e, true);

		ImGui::End();
	}

	void Editor::ObjectOptions()
	{
		if (ImGui::Begin(ICON_FA_CIRCLE " Object Options"))
		{
			if (selected_entity != tecs::null_entity)
			{
				auto tag = engine->reg.get_if<Tag>(selected_entity);
				if (tag)
				{
					char buffer[256];
					memset(buffer, 0, sizeof(buffer));
					std::strncpy(buffer, tag->name.c_str(), sizeof(buffer));
					if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
						tag->name = std::string(buffer);
				}

				auto light = engine->reg.get_if<Light>(selected_entity);
				if (light && ImGui::CollapsingHeader("Light"))
				{

					if (light->type == LightType::Directional)			ImGui::Text("Directional Light");
					else if (light->type == LightType::Spot)			ImGui::Text("Spot Light");
					else if (light->type == LightType::Point)			ImGui::Text("Point Light");

					Vector4 light_color = light->color, light_direction = light->direction, light_position = light->position;
					float color[3] = { light_color.x, light_color.y, light_color.z };
					ImGui::ColorEdit3("Light Color", color);

					light->color = Vector4(color[0], color[1], color[2], 1.0f);

					ImGui::SliderFloat("Light Energy", &light->energy, 0.0f, 50.0f);

					if (engine->reg.has<Material>(selected_entity))
					{
						auto& material = engine->reg.get<Material>(selected_entity);
						material.diffuse = Vector3(color[0], color[1], color[2]);
					}

					if (light->type == LightType::Directional || light->type == LightType::Spot)
					{
						float direction[3] = { light_direction.x, light_direction.y, light_direction.z };

						ImGui::SliderFloat3("Light direction", direction, -1.0f, 1.0f);

						light->direction = Vector4(direction[0], direction[1], direction[2], 0.0f);

						if (light->type == LightType::Directional)
						{
							light->position = -light->direction * 1e3;
						}
					}

					if (light->type == LightType::Spot)
					{
						float inner_angle = DirectX::XMConvertToDegrees(acos(light->inner_cosine))
							, outer_angle = DirectX::XMConvertToDegrees(acos(light->outer_cosine));
						ImGui::SliderFloat("Inner Spot Angle", &inner_angle, 0.0f, 90.0f);
						ImGui::SliderFloat("Outer Spot Angle", &outer_angle, inner_angle, 90.0f);

						light->inner_cosine = cos(DirectX::XMConvertToRadians(inner_angle));
						light->outer_cosine = cos(DirectX::XMConvertToRadians(outer_angle));
					}

					if (light->type == LightType::Point || light->type == LightType::Spot)
					{
						float position[3] = { light_position.x, light_position.y, light_position.z };

						ImGui::SliderFloat3("Light position", position, -300.0f, 500.0f);

						light->position = Vector4(position[0], position[1], position[2], 1.0f);

						ImGui::SliderFloat("Range", &light->range, 50.0f, 1000.0f);
					}

					if (engine->reg.has<Transform>(selected_entity))
					{
						auto& tr = engine->reg.get<Transform>(selected_entity);

						tr.current_transform = XMMatrixTranslationFromVector(light->position);
					}

					ImGui::Checkbox("Active", &light->active);
					ImGui::Checkbox("God Rays", &light->god_rays);

					if (light->god_rays)
					{
						ImGui::SliderFloat("God Rays decay", &light->godrays_decay, 0.0f, 1.0f);
						ImGui::SliderFloat("God Rays weight", &light->godrays_weight, 0.0f, 0.5f);
						ImGui::SliderFloat("God Rays density", &light->godrays_density, 0.1f, 3.0f);
						ImGui::SliderFloat("God Rays exposure", &light->godrays_exposure, 0.1f, 10.0f);
					}

					ImGui::Checkbox("Casts Shadows", &light->casts_shadows);

					ImGui::Checkbox("Screen Space Contact Shadows", &light->screen_space_contact_shadows);
					if (light->screen_space_contact_shadows)
					{
						ImGui::SliderFloat("Thickness", &light->sscs_thickness, 0.0f, 1.0f);
						ImGui::SliderFloat("Max Ray Distance", &light->sscs_max_ray_distance, 0.0f, 0.3f);
						ImGui::SliderFloat("Max Depth Distance", &light->sscs_max_depth_distance, 0.0f, 500.0f);
					}

					ImGui::Checkbox("Volumetric Lighting", &light->volumetric);
					if (light->volumetric)
					{
						ImGui::SliderFloat("Volumetric lighting Strength", &light->volumetric_strength, 0.0f, 5.0f);
					}

					ImGui::Checkbox("Lens Flare", &light->lens_flare);

					if (light->type == LightType::Directional && light->casts_shadows)
					{
						bool use_cascades = static_cast<bool>(light->use_cascades);
						ImGui::Checkbox("Use Cascades", &use_cascades);
						light->use_cascades = use_cascades;
					}

				}

				auto material = engine->reg.get_if<Material>(selected_entity);
				if (material && ImGui::CollapsingHeader("Material"))
				{
					ImGui::Text("Albedo Texture");
					ImGui::Image(g_TextureManager.GetTextureView(material->albedo_texture), ImVec2(48.0f, 48.0f));

					ImGui::PushID(0);
					if (ImGui::Button("Remove")) material->albedo_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							material->albedo_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();

					ImGui::Text("Metallic-Roughness Texture");
					ImGui::Image(g_TextureManager.GetTextureView(material->metallic_roughness_texture), ImVec2(48.0f, 48.0f));

					ImGui::PushID(1);
					if (ImGui::Button("Remove")) material->metallic_roughness_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							material->metallic_roughness_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();

					ImGui::Text("Emissive Texture");
					ImGui::Image(g_TextureManager.GetTextureView(material->emissive_texture), ImVec2(48.0f, 48.0f));

					ImGui::PushID(2);
					if (ImGui::Button("Remove")) material->emissive_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							material->emissive_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();

					ImGui::ColorEdit3("Albedo Color", &material->diffuse.x);
					ImGui::SliderFloat("Albedo Factor", &material->albedo_factor, 0.0f, 1.0f);
					ImGui::SliderFloat("Metallic Factor", &material->metallic_factor, 0.0f, 1.0f);
					ImGui::SliderFloat("Roughness Factor", &material->roughness_factor, 0.0f, 1.0f);
					ImGui::SliderFloat("Emissive Factor", &material->emissive_factor, 0.0f, 32.0f);

					//add shader changing
					if (engine->reg.has<Forward>(selected_entity))
					{
						if (material->albedo_texture != INVALID_TEXTURE_HANDLE)
							material->shader = ShaderProgram::Texture;
						else material->shader = ShaderProgram::Solid;
					}
					else
					{
						material->shader = ShaderProgram::GBufferPBR;
					}
				}

				auto transform = engine->reg.get_if<Transform>(selected_entity);
				if (transform && ImGui::CollapsingHeader("Transform"))
				{
					Matrix tr = transform->current_transform;

					float translation[3], rotation[3], scale[3];
					ImGuizmo::DecomposeMatrixToComponents(tr.m[0], translation, rotation, scale);
					ImGui::InputFloat3("Translation", translation);
					ImGui::InputFloat3("Rotation", rotation);
					ImGui::InputFloat3("Scale", scale);
					ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, tr.m[0]);

					if (Emitter* emitter = engine->reg.get_if<Emitter>(selected_entity))
					{
						emitter->position = Vector4(translation[0], translation[1], translation[2], 1.0f);
					}

					if (AABB* aabb = engine->reg.get_if<AABB>(selected_entity))
					{
						aabb->bounding_box.Transform(aabb->bounding_box, transform->current_transform.Invert());
						aabb->bounding_box.Transform(aabb->bounding_box, tr);
						aabb->UpdateBuffer(engine->gfx.get());
					}

					if (Relationship* relationship = engine->reg.get_if<Relationship>(selected_entity))
					{
						for (size_t i = 0; i < relationship->children_count; ++i)
						{
							tecs::entity child = relationship->children[i];
							if (AABB* aabb = engine->reg.get_if<AABB>(child))
							{
								aabb->bounding_box.Transform(aabb->bounding_box, transform->current_transform.Invert());
								aabb->bounding_box.Transform(aabb->bounding_box, tr);
								aabb->UpdateBuffer(engine->gfx.get());
							}
						}
					}
					transform->current_transform = tr;
				}

				
				auto emitter = engine->reg.get_if<Emitter>(selected_entity);
				if (emitter && ImGui::CollapsingHeader("Emitter"))
				{
					ImGui::Text("Particle Texture");
					ImGui::Image(g_TextureManager.GetTextureView(emitter->particle_texture), ImVec2(48.0f, 48.0f));

					ImGui::PushID(3);
					if (ImGui::Button("Remove")) emitter->particle_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							emitter->particle_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();

					float pos[3] = { emitter->position.x, emitter->position.y, emitter->position.z },
						vel[3] = { emitter->velocity.x, emitter->velocity.y, emitter->velocity.z },
						pos_var[3] = { emitter->position_variance.x, emitter->position_variance.y, emitter->position_variance.z };

					ImGui::SliderFloat3("Position", pos, -500.0f, 500.0f);
					ImGui::SliderFloat3("Velocity", vel, -50.0f, 50.0f);
					ImGui::SliderFloat3("Position Variance", pos_var, -50.0f, 50.0f);
					emitter->position = Vector4(pos[0], pos[1], pos[2], 1.0f);
					emitter->velocity = Vector4(vel[0], vel[1], vel[2], 1.0f);
					emitter->position_variance = Vector4(pos_var[0], pos_var[1], pos_var[2], 1.0f);

					if (transform)
					{
						Matrix tr = transform->current_transform;
						float translation[3], rotation[3], scale[3];
						ImGuizmo::DecomposeMatrixToComponents(tr.m[0], translation, rotation, scale);
						ImGuizmo::RecomposeMatrixFromComponents(pos, rotation, scale, tr.m[0]);
						transform->current_transform = tr;
					}

					ImGui::SliderFloat("Velocity Variance", &emitter->velocity_variance, -10.0f, 10.0f);
					ImGui::SliderFloat("Lifespan", &emitter->particle_lifespan, 0.0f, 50.0f);
					ImGui::SliderFloat("Start Size", &emitter->start_size, 0.0f, 50.0f);
					ImGui::SliderFloat("End Size", &emitter->end_size, 0.0f, 10.0f);
					ImGui::SliderFloat("Mass", &emitter->mass, 0.0f, 10.0f);
					ImGui::SliderFloat("Particles Per Second", &emitter->particles_per_second, 1.0f, 1000.0f);

					ImGui::Checkbox("Alpha Blend", &emitter->alpha_blended);
					ImGui::Checkbox("Collisions", &emitter->collisions_enabled);
					if (emitter->collisions_enabled) ImGui::SliderInt("Collision Thickness", &emitter->collision_thickness, 0, 40);

					ImGui::Checkbox("Sort", &emitter->sort);
					ImGui::Checkbox("Pause", &emitter->pause);
					if (ImGui::Button("Reset")) emitter->reset_emitter = true;
				}

				auto skybox = engine->reg.get_if<Skybox>(selected_entity);
				if (skybox)
				{

					if (ImGui::TreeNode("Skybox"))
					{
						const char* sky_types[] = { "Skybox", "Uniform Color", "Hosek-Wilkie" };
						static int current_sky_type = 0;
						const char* combo_label = sky_types[current_sky_type];
						if (ImGui::BeginCombo("Type", combo_label, 0))
						{
							for (int n = 0; n < IM_ARRAYSIZE(sky_types); n++)
							{
								const bool is_selected = (current_sky_type == n);
								if (ImGui::Selectable(sky_types[n], is_selected)) current_sky_type = n;
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}


						static float color_sky[3] = { 0.0f, 0.0f, 0.0f };

						switch (current_sky_type)
						{
						case 0:

							renderer_settings.sky_type = SkyType::Skybox;

							break;

						case 1:

							renderer_settings.sky_type = SkyType::UniformColor;


							memcpy(renderer_settings.sky_color, color_sky, sizeof(color_sky));

							break;

						case 2:
							renderer_settings.sky_type = SkyType::HosekWilkie;
							ImGui::SliderFloat("Turbidity", &renderer_settings.turbidity, 2.0f, 30.0f);
							ImGui::SliderFloat("Ground Albedo", &renderer_settings.ground_albedo, 0.0f, 1.0f);
							break;

						}

						const char* sky_variants[] = { "Sky", "Desert", "Forest", "Grass", "Night", "Ocean", "Rocky Mountains", "Snow"};
						static int current_sky_variants = 0;
						const char* combo_label_variants = sky_variants[current_sky_variants];
						if (ImGui::BeginCombo("Variants", combo_label_variants, 0))
						{
							for (int n = 0; n < IM_ARRAYSIZE(sky_variants); n++)
							{
								const bool is_selected = (current_sky_variants == n);
								if (ImGui::Selectable(sky_variants[n], is_selected)) current_sky_variants = n;
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}

						std::wstring sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Sky.dds");

						switch (current_sky_variants)
						{
						case 0:
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 1:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Desert.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 2:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Forest.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 3:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Grass.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 4:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Night.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 5:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Ocean.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 6:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/RockyMountains.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						case 7:
							sky_texture_path = ToWideString("../../source/Resources/Textures/Skybox/Snow.dds");
							skybox->cubemap_texture = g_TextureManager.LoadTexture(sky_texture_path);
							break;
						}
					


						ImGui::Checkbox("Active", &skybox->active);
						if (ImGui::Button("Select"))
						{
							nfdchar_t* file_path = NULL;
							nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
							nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
							if (result == NFD_OKAY)
							{
								std::wstring texture_path = ToWideString(file_path);
								skybox->cubemap_texture = g_TextureManager.LoadTexture(texture_path);
								free(file_path);
							}
						}

						ImGui::TreePop();
					}
				}



				auto decal = engine->reg.get_if<Decal>(selected_entity);
				if (decal && ImGui::CollapsingHeader("Decal"))
				{
					ImGui::Text("Decal Albedo Texture");
					ImGui::Image(g_TextureManager.GetTextureView(decal->albedo_decal_texture), ImVec2(48.0f, 48.0f));

					ImGui::PushID(4);
					if (ImGui::Button("Remove")) decal->albedo_decal_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							decal->albedo_decal_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();

					ImGui::PushID(5);
					if (ImGui::Button("Remove")) decal->normal_decal_texture = INVALID_TEXTURE_HANDLE;
					if (ImGui::Button("Select"))
					{
						nfdchar_t* file_path = NULL;
						nfdchar_t const* filter_list = "jpg,jpeg,tga,dds,png";
						nfdresult_t result = NFD_OpenDialog(filter_list, NULL, &file_path);
						if (result == NFD_OKAY)
						{
							std::wstring texture_path = ToWideString(file_path);
							decal->normal_decal_texture = g_TextureManager.LoadTexture(texture_path);
							free(file_path);
						}
					}
					ImGui::PopID();
					ImGui::Checkbox("Modify GBuffer Normals", &decal->modify_gbuffer_normals);
				}

				if (AABB* aabb = engine->reg.get_if<AABB>(selected_entity))
				{
					aabb->draw_aabb = true;
				}
			}
		}
		ImGui::End();

	}

    void Editor::ShowFolder(const std::filesystem::path& folderPath, bool isFirstNode)
    {
       if (isFirstNode) {
           ImGui::SetNextTreeNodeOpen(true);
       }

       if (ImGui::TreeNode(folderPath.filename().string().c_str())) {
           for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
              if (entry.is_directory()) {
                  ShowFolder(entry.path(), false); // Передаем false для остальных узлов
              }
              else {
                  folder = folderPath.string().c_str();
              }
            }
            ImGui::TreePop();
        } 
    }

	
	void Editor::Explorer()
	{
		
		ImGui::Begin(ICON_FA_FOLDER " Assets");
		fs::path rootFolder = "../../source/Resources"; 

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		// Получаем размер окна ImGui
		ImVec2 windowSize = ImGui::GetWindowSize();


		// Открываем папку и проходим по всем файлам
		int buttonCount = 0;


		char buf[256];  // Create a buffer to hold the input text

		strcpy(buf, folder.c_str());

		ImGui::SetCursorPos(ImVec2(250, 50));

		if (ImGui::Button(ICON_FA_ARROW_LEFT ""))
		{
		}


		ImGui::SameLine();
		// Inside your ImGui render function
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 350);
		if (ImGui::InputText("##input", buf, IM_ARRAYSIZE(buf)) == ImGui::IsItemDeactivated())
		{
			folder = buf;
		}


		ImGui::SetCursorPos(ImVec2(260, 100));

		ImGui::BeginChild("Files", ImVec2(contentRegionAvailable.x / 1.15, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
                  
		ImGui::BeginGroup();


		for (const auto& entry : fs::directory_iterator(folder)) {


			std::string filePath = entry.path().string();

			// Создание кнопок в горизонтальной компоновке
			ImGui::BeginGroup();

			if (entry.is_directory()) {


				// Создаем кнопку ImGui с иконкой файла
				if (ImGui::Button("folder"))
				{
				    folder = entry.path().string().c_str();
				}

				if (entry.path().filename().string().length() > 10) {
					ImGui::TextWrapped("%s", entry.path().filename().string().substr(0, 10).c_str());
				}
				else {
					ImGui::TextWrapped("%s", entry.path().filename().string().c_str());
				}

				
			}
			else {
				
				// Создаем кнопку ImGui с иконкой файла
				if (ImGui::Button("File"))
				{

				}
				if (entry.path().filename().string().length() > 10) {
					ImGui::TextWrapped("%s", entry.path().filename().string().substr(0, 10).c_str());
				}
				else {
					ImGui::TextWrapped("%s", entry.path().filename().string().c_str());
				}

			}

			ImGui::EndGroup();
			ImGui::SameLine();

			// Увеличиваем счетчик кнопок
			buttonCount++;

			// Если количество объектов превышает ширину BeginChild, переносим некоторые объекты вниз
			if (buttonCount % 17 == 0) {
				ImGui::EndGroup();
				ImGui::NewLine();
				ImGui::BeginGroup();
			}
		}

		ImGui::EndGroup();
		

		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.11f, 0.12f, 10));
		ImGui::SetCursorPos(ImVec2(0, 35));
		ImGui::BeginChild("Assets", ImVec2(contentRegionAvailable.x / 8, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
		
		
		ShowFolder(rootFolder, true);

		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		


		ImGui::End();
	}

	void Editor::Preferences()
	{
		if (ShowPrefernces == true)
		{
			ImGui::Begin("Prefences");

			if (ImGui::BeginTabBar("Graphics", 0 )) {
				if (ImGui::BeginTabItem("Graphics")) 
				{

					if (ImGui::TreeNode("Anti-Aliasing"))
					{
						static bool fxaa = false, taa = false;
						ImGui::Checkbox("FXAA", &fxaa);
						ImGui::Checkbox("TAA", &taa);
						if (fxaa)
						{
							renderer_settings.anti_aliasing = static_cast<AntiAliasing>(renderer_settings.anti_aliasing | AntiAliasing_FXAA);
						}
						else
						{
							renderer_settings.anti_aliasing = static_cast<AntiAliasing>(renderer_settings.anti_aliasing & (~AntiAliasing_FXAA));
						}
						if (taa)
						{
							renderer_settings.anti_aliasing = static_cast<AntiAliasing>(renderer_settings.anti_aliasing | AntiAliasing_TAA);
						}
						else
						{
							renderer_settings.anti_aliasing = static_cast<AntiAliasing>(renderer_settings.anti_aliasing & (~AntiAliasing_TAA));
						}

						ImGui::TreePop();
					}


					{
						const char* ao_types[] = { "None", "SSAO", "HBAO" };
						static int current_ao_type = 1;
						const char* combo_label = ao_types[current_ao_type];
						if (ImGui::BeginCombo("Ambient Occlusion", combo_label, 0))
						{
							for (int n = 0; n < IM_ARRAYSIZE(ao_types); n++)
							{
								const bool is_selected = (current_ao_type == n);
								if (ImGui::Selectable(ao_types[n], is_selected)) current_ao_type = n;
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}

						renderer_settings.ambient_occlusion = static_cast<AmbientOcclusion>(current_ao_type);

						if (renderer_settings.ambient_occlusion == AmbientOcclusion::SSAO && ImGui::TreeNodeEx("SSAO", ImGuiTreeNodeFlags_OpenOnDoubleClick))
						{
							ImGui::SliderFloat("Power", &renderer_settings.ssao_power, 1.0f, 16.0f);
							ImGui::SliderFloat("Radius", &renderer_settings.ssao_radius, 0.5f, 4.0f);

							ImGui::TreePop();
							ImGui::Separator();
						}
						if (renderer_settings.ambient_occlusion == AmbientOcclusion::HBAO && ImGui::TreeNodeEx("HBAO", ImGuiTreeNodeFlags_OpenOnDoubleClick))
						{
							ImGui::SliderFloat("Power", &renderer_settings.hbao_power, 1.0f, 16.0f);
							ImGui::SliderFloat("Radius", &renderer_settings.hbao_radius, 0.25f, 8.0f);

							ImGui::TreePop();
							ImGui::Separator();
						}
					}

					ImGui::Checkbox("SSR", &renderer_settings.ssr);

					if (renderer_settings.ssr && ImGui::TreeNodeEx("Screen-Space Reflections", 0))
					{
						ImGui::SliderFloat("Ray Step", &renderer_settings.ssr_ray_step, 1.0f, 3.0f);
						ImGui::SliderFloat("Ray Hit Threshold", &renderer_settings.ssr_ray_hit_threshold, 0.25f, 5.0f);

						ImGui::TreePop();
						ImGui::Separator();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			

			ImGui::End();
		}



	}

    void Editor::Log()
    {
		if (ImGui::Begin(ICON_FA_TERMINAL " Console"))
        {
		  
        }
        ImGui::End();
    }

}
