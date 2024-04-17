//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------

#pragma once


// Namespace Case_Engine
namespace Case_Engine
{
	template<typename T>
	class Singleton
	{
	public:
		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;

		static T& Get()
		{
			static T instance;
			return instance;
		}

	protected:
		Singleton() {}
		~Singleton() {}
	};
}