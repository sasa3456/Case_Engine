#include "Core/Window.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Editors/Editor.h"
#include "Utilities/MemoryDebugger.h"
#include "Utilities/CLIParser.h"
#include "Utilities/Timer.h"
#include <filesystem>

using namespace Case_Engine;


bool copyFile(const char* SRC, const char* DEST)
{
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	return src && dest;
}

int main( int argc, char* argv[])
{
	CLIParser parser{};
	CLIArg& width = parser.AddArg(true, "-w", "--width");
	CLIArg& height = parser.AddArg(true, "-h", "--height");
	CLIArg& title = parser.AddArg(true, "-title");
	CLIArg& config = parser.AddArg(true, "-cfg", "--config");
	CLIArg& scene = parser.AddArg(true, "-scene", "--scenefile");
	CLIArg& log = parser.AddArg(true, "-log", "--logfile");
	CLIArg& loglevel = parser.AddArg(true, "-loglvl", "--loglevel");
	CLIArg& maximize = parser.AddArg(false, "-max", "--maximize");
	CLIArg& vsync = parser.AddArg(false, "-vsync");

    MemoryDebugger::SetBreak(275);
	MemoryDebugger::Checkpoint();
	parser.Parse(argv);
    {
		std::string log_file = log.AsStringOr("case-engine.log");
		int32_t log_level = loglevel.AsIntOr(0);
		CASE_ENGINE_REGISTER_LOGGER(new FileLogger(log_file.c_str(), static_cast<LogLevel>(log_level)));
		CASE_ENGINE_REGISTER_LOGGER(new OutputDebugStringLogger(static_cast<LogLevel>(log_level)));

		WindowInit window_init{};
		window_init.width = width.AsIntOr(1920);
		window_init.height = height.AsIntOr(1000);
		std::string window_title = title.AsStringOr("Case Engine");
		window_init.title = window_title.c_str();
		window_init.maximize = maximize;
		Window window(window_init);
		g_Input.Initialize(&window);

        EngineInit engine_init{};
        engine_init.vsync = vsync;
		engine_init.window = &window;
        engine_init.scene_file = scene.AsStringOr("scene.json");

        EditorInit editor_init{};
        editor_init.engine_init = std::move(engine_init);

        Editor editor{ editor_init };
		window.GetWindowEvent().Add([&](WindowEventData const& msg_data) {editor.OnWindowEvent(msg_data); });

		WindowEventData msg_data;

        //Main loop flag
        bool quit = false;

		char* dropped_filedir;


		static Timer timer;


        //While application is running
        while (!quit)
        {
			float dt = timer.MarkInSeconds() * 50;
            //Handle events on queue
            while (SDL_PollEvent(&msg_data.Event) != 0)
            {

				ImGui_ImplSDL2_ProcessEvent(&msg_data.Event);

                //User requests quit
                if (msg_data.Event.type == SDL_QUIT)
                {
					SDL_VideoQuit();
                }

				//User requests quit
				if (msg_data.Event.type == SDL_DROPFILE)
				{
					dropped_filedir = msg_data.Event.drop.file;
					printf("%s\n", dropped_filedir);

					copyFile(dropped_filedir, "../../source/Resources");

					std::filesystem::copy(dropped_filedir, editor.folder);

					SDL_free(dropped_filedir);
				}


				if (msg_data.Event.type == SDL_MOUSEWHEEL)
				{
					if (msg_data.Event.wheel.y > 0) // scroll up
					{
						editor.engine->camera->Walk(editor.engine->camera->speed_factor * dt);
					}

					if (msg_data.Event.wheel.y < 0) // scroll down
					{
						editor.engine->camera->Walk(-editor.engine->camera->speed_factor * dt);
					}
				}

            }



            editor.Run();
        }
        

        
    }
    MemoryDebugger::CheckLeaks();

	return 0;

}


