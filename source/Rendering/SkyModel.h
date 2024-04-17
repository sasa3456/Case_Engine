//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <array>


// Namespace Case_Engine
namespace Case_Engine
{

	enum ESkyParams : uint16_t
	{
		ESkyParam_A = 0,
		ESkyParam_B,
		ESkyParam_C,
		ESkyParam_D,
		ESkyParam_E,
		ESkyParam_F,
		ESkyParam_G,
		ESkyParam_I,
		ESkyParam_H,
		ESkyParam_Z,
		ESkyParam_Count
	};

	using SkyParameters = std::array<Vector3, ESkyParam_Count>;

	SkyParameters CalculateSkyParameters(float turbidity, float albedo, Vector3 const& sun_direction);
}