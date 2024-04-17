//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "TextEditor.h"
#include "../EditorLogger.h"
#include "Core/Logger.h"

char text[10000] = "";
int textLength = 10000;

// Namespace Case_Engine
namespace Case_Engine
{

	TextEditor::TextEditor()
	{

	}

	TextEditor::~TextEditor()
	{

	}

	void DrawTextLines(const char* text) {
		std::vector<std::string> lines;
		std::string line;
		for (int i = 0; text[i] != '\0'; i++) {
			if (text[i] == '\n') {
				lines.push_back(line);
				line.clear();
			}
			else {
				line += text[i];
			}
		}
		if (!line.empty()) {
			lines.push_back(line);
		}


		for (int i = 0; i < lines.size(); i++) {
			ImGui::Text("%d %s", i, "");
		}


	}

	void TextEditor::EditorText()
	{
		bool show_window = true;


		if (show_window == true)
		{
			ImGui::Begin("Text Editor", &show_window);

			ImGui::BeginGroup();

			DrawTextLines(text);

			ImGui::EndGroup();
			ImGui::SameLine();


			ImGui::InputTextMultiline("##Input", text, textLength, ImVec2(ImGui::GetWindowHeight(), ImGui::GetWindowWidth()), ImGuiInputTextFlags_AllowTabInput);
			

			ImGui::End();
		}


	}



}
