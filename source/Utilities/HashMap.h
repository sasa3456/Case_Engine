//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#if _DEBUG
#include <unordered_map>
#else 
#include <unordered_map>
#endif


// Namespace Case_Engine
namespace Case_Engine
{
#if _DEBUG
	template<typename K, typename V>
	using HashMap = std::unordered_map<K, V>;
#else 
	template<typename K, typename V>
	using HashMap = std::unordered_map<K, V>;
#endif
}