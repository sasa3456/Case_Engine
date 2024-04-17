//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "GfxScopedAnnotation.h"
#include "GfxCommandContext.h"


// Namespace Case_Engine
namespace Case_Engine
{


	GfxScopedAnnotation::GfxScopedAnnotation(GfxCommandContext* context, char const* name) : context(context)
	{
		if (context) context->BeginEvent(name);
	}

	GfxScopedAnnotation::~GfxScopedAnnotation()
	{
		if (context) context->EndEvent();
	}

}

