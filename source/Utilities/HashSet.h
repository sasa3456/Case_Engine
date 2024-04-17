//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#if _DEBUG
#include <unordered_set>
#else 
#include <unordered_set>
#endif


// Namespace case_Engine
namespace Case_Engine
{
#if _DEBUG
	template<typename K>
	using HashSet = std::unordered_set<K>;
#else 
	template<typename K>
	using HashSet = std::unordered_set<K>;
#endif
}