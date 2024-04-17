//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include "Core/Logger.h"


// Namespace Case_Engine
namespace Case_Engine
{
	struct ImGuiLogger;
	class EditorLogger : public ILogger
	{
	public:
		explicit EditorLogger(LogLevel logger_level = LogLevel::LOG_DEBUG);
		virtual void Log(LogLevel level, char const* entry, char const* file, uint32_t line) override;
		void Draw(const char* title, bool* p_open);

	private:
		std::unique_ptr<ImGuiLogger> logger;
		LogLevel logger_level;
	};
}