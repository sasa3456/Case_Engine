//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------
#pragma once


// Namespace Case_Engine
namespace Case_Engine
{
	class GfxCommandContext;
	class GfxScopedAnnotation
	{
	public:
		GfxScopedAnnotation(GfxCommandContext* context, char const* name);

		~GfxScopedAnnotation();

	private:
		GfxCommandContext* context;
	};
	#define CaseEngineGfxScopedAnnotation(ctx, name) GfxScopedAnnotation CASE_ENGINE_CONCAT(_annotation, __COUNTER__)(ctx, name)
}