//-----------------------------------------------------
// � Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once


// Namespace Case_Engine
namespace Case_Engine
{
	struct SceneViewport
	{
		float scene_viewport_pos_x;
		float scene_viewport_pos_y;
		float scene_viewport_size_x;
		float scene_viewport_size_y;
		bool  scene_viewport_focused;
		float mouse_position_x;
		float mouse_position_y;
	};
}