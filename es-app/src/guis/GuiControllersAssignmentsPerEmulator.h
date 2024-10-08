#pragma once

#include "GuiSettings.h"
#include <vector>

class Window;

// class InputConfigInfo
// {
// public:
// 	InputConfigInfo(const std::string& ideviceName, const std::string& ideviceGUIDString, const std::string& idevicePath)
// 	{
// 		name = ideviceName;
// 		guid = ideviceGUIDString;
// 		path = idevicePath;
// 	}

// 	std::string name;
// 	std::string guid;
// 	std::string path;
// };

class GuiControllersAssignmentsPerEmulator : public GuiSettings
{
public:
	static std::string getControllersSettingsLabel();
	static void openControllersSettings(Window* wnd, int autoSel = 0);
	
	GuiControllersAssignmentsPerEmulator(Window* wnd, int autosel = 0);
	// ~GuiControllersAssignmentsPerEmulator();

private:
// 	void openControllersSpecificSettings_sindengun();
// 	void openControllersSpecificSettings_wiigun();
// 	void openControllersSpecificSettings_steamdeckgun();

// 	// std::vector<InputConfigInfo*> mLoadedInput;
// 	void clearLoadedInput();
};
