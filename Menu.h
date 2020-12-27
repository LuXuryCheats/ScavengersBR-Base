#include "stdafx.h"
#include "Settings.h"

namespace Config
{
	bool Menu = true;
	
	void MenuINIT()
	{
		if (Config::Menu)
		{
			ImGui::SetNextWindowSize({300,300});
			ImGui::Begin("Scavengers Private Beta", 0);
			{
				ImGui::Checkbox("FOV Circle", &Settings::FOV);
				if (Settings::FOV)
				{
					ImGui::SliderFloat("Size", &Settings::Size, 0.0f, 950.f);
				}
				ImGui::End();
			}
		}
	}

}