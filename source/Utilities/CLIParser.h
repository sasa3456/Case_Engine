//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <shellapi.h>
#include <vector>
#include <string>
#include "StringUtil.h"
#include "../Core/Defines.h"


// Namespace Case_Engine
namespace Case_Engine
{
	class CLIArg
	{
		friend class CLIParser;
	public:
		CLIArg(std::vector<std::string>&& prefixes, bool has_value)
			: prefixes(std::move(prefixes)), has_value(has_value)
		{}

		bool AsBool(bool default_value = false) const
		{
			CASE_ENGINE_ASSERT(has_value);
			if (value == "true" || value == "1") return true;
			if (value == "false" || value == "0") return false;
			CASE_ENGINE_ASSERT_MSG(false, "Invalid bool argument!");
			CASE_ENGINE_UNREACHABLE();
		}
		bool AsBoolOr(bool def) const
		{
			if (IsPresent()) return AsBool();
			else return def;
		}

		int32_t AsInt() const
		{
			CASE_ENGINE_ASSERT(has_value);
			return (int32_t)strtol(value.c_str(), nullptr, 10);
		}
		int32_t AsIntOr(int32_t def) const
		{
			if (IsPresent()) return AsInt();
			else return def;
		}

		float AsFloat() const
		{
			CASE_ENGINE_ASSERT(has_value);
			return (float)std::strtod(value.c_str(), nullptr);
		}
		float AsFloatOr(float def) const
		{
			if (IsPresent()) return AsFloat();
			else return def;
		}
		
		std::string AsString() const
		{
			CASE_ENGINE_ASSERT(has_value);
			return value;
		}
		std::string AsStringOr(std::string const& def) const
		{
			if (IsPresent()) return AsString();
			else return def;
		}

		bool IsPresent() const
		{
			return is_present;
		}
		operator bool() const
		{
			return IsPresent();
		}

	private:
		std::vector<std::string> prefixes;
		bool has_value;
		std::string value;
		bool is_present = false;

		void SetValue(std::string const& _value)
		{
			CASE_ENGINE_ASSERT(has_value);
			value = _value;
		}
		void SetIsPresent()
		{
			is_present = true;
		}
	};

	class CLIParser
	{
	public:
		CLIParser() 
		{
			args.reserve(128);
		}

		[[nodiscard]] CLIArg& AddArg(bool has_value, std::convertible_to<std::string> auto... prefixes)
		{
			args.emplace_back(std::vector<std::string>{prefixes...}, has_value);
			return args.back();
		}

		void Parse(int argc, wchar_t** argv)
		{
			std::vector<std::wstring> cmdline(argv, argv + argc);
			for (size_t i = 0; i < cmdline.size(); ++i)
			{
				bool found = false;
				for (CLIArg& arg : args)
				{
					for (auto const& prefix : arg.prefixes) 
					{
						bool prefix_found = cmdline[i] == ToWideString(prefix);
						if (prefix_found)
						{
							found = true;
							arg.SetIsPresent();
							if (arg.has_value) arg.SetValue(ToString(cmdline[++i]));
							break;
						}
					}
					if (found) break;
				}
			}
		}

		void Parse(char* cmd_line[])
		{

			int size = sizeof(cmd_line) / sizeof(cmd_line[0]);

			// определение размера буфера для новой переменной wchar_t **
			size_t bufferSize = 0;
			for (int i = 0; i < size; i++)
			{
				bufferSize += mbstowcs(nullptr, cmd_line[i], 0) + 1; // +1 для нулевого символа на конце
			}

			// выделение памяти для новой переменной wchar_t **
			wchar_t** wcharStrings = new wchar_t* [size];
			for (int i = 0; i < size; i++)
			{
				wcharStrings[i] = new wchar_t[bufferSize];
				std::memset(wcharStrings[i], 0, bufferSize * sizeof(wchar_t));
			}

			// перевод символов из char* в wchar_t*
			std::locale::global(std::locale("")); // установка локали
			for (int i = 0; i < size; i++)
			{
				mbstowcs(wcharStrings[i], cmd_line[i], bufferSize);
			}

			// вывод результата
			for (int i = 0; i < size; i++)
			{
				std::wcout << wcharStrings[i] << std::endl;
			}

			// освобождение памяти
			for (int i = 0; i < size; i++)
			{
				delete[] wcharStrings[i];
			}
			delete[] wcharStrings;

			int argc = 0;
			wchar_t** argv = wcharStrings;
			Parse(argc, argv);
		}

	private:
		std::vector<CLIArg> args;
	};
}
