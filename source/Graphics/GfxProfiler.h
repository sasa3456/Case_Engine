//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <string>
#include <array>
#include <unordered_map>
#include <d3d11.h>
#include "GfxDefines.h"
#include "Utilities/Singleton.h"


// Namespace Case_Engine
namespace Case_Engine
{
	struct Timestamp
	{
		std::string name;
		float time_in_ms;
	};

	class GfxDevice;
	class GfxCommandContext;
	class GfxQuery;

	class GfxProfiler : public Singleton<GfxProfiler>
	{
		friend class Singleton<GfxProfiler>;

		static constexpr uint64_t FRAME_COUNT = GFX_BACKBUFFER_COUNT;
		static constexpr uint64_t MAX_QUERIES = 256;
		struct QueryData
		{
			std::unique_ptr<GfxQuery> disjoint_query;
			std::unique_ptr<GfxQuery> timestamp_query_start;
			std::unique_ptr<GfxQuery> timestamp_query_end;
			bool begin_called = false, end_called = false;
		};

	public:
		void Initialize(GfxDevice* gfx);
		void Destroy();
		void NewFrame();

		void BeginProfileScope(GfxCommandContext* context, char const* name);
		void EndProfileScope(GfxCommandContext* context, char const* name);
		std::vector<Timestamp> GetProfilingResults();

	private:
		GfxDevice* gfx = nullptr;
		uint64_t current_frame = 0;
		std::array<std::array<QueryData, MAX_QUERIES>, FRAME_COUNT> queries;
		std::unordered_map<std::string, uint32_t> name_to_index_map;
		uint32_t scope_counter = 0;

	private:
		GfxProfiler();
		~GfxProfiler();
	};
	#define g_GfxProfiler GfxProfiler::Get()

#if GFX_PROFILING
	struct GfxProfileScope
	{
		GfxProfileScope(GfxCommandContext* context, char const* name, bool active = true)
			: name{ name }, context{ context }, active{ active }
		{
			if (active) g_GfxProfiler.BeginProfileScope(context, name);
		}

		~GfxProfileScope()
		{
			if (active)
				g_GfxProfiler.EndProfileScope(context, name);
		}

		GfxCommandContext* context;
		char const* name;
		bool active;
	};
	#define CaseEngineGfxProfileScope(context, name) GfxProfileScope CASE_ENGINE_CONCAT(gfx_profile, __COUNTER__)(context, name)
	#define CaseEngineGfxProfileCondScope(context, name, cond) GfxProfileScope CASE_ENGINE_CONCAT(gfx_profile, __COUNTER__)(context, name, cond)
	#define CaseEngineGfxProfileScopeOld(context, name) 
	#define CaseEngineGfxProfileCondScopeOld(context, name, active) 
#else
	#define CaseEngineGfxProfileScope(context, name) 
	#define CaseEngineGfxProfileCondScope(context, name, cond) 
	#define CaseEngineGfxProfileScopeOld(context, name) 
	#define CaseEngineGfxProfileCondScopeOld(context, name, active) 
#endif
}