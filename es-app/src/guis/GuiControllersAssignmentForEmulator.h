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

class GuiControllersAssignmentForEmulator : public GuiSettings
{
public:
	static std::string getControllersSettingsLabel();
	static void openControllersSettings(Window* wnd, const char* systemName);
	
	GuiControllersAssignmentForEmulator(Window* wnd, const char* systemName);
	// ~GuiControllersAssignmentForEmulator();

private:
  void test_func();
// 	void openControllersSpecificSettings_sindengun();
// 	void openControllersSpecificSettings_wiigun();
// 	void openControllersSpecificSettings_steamdeckgun();

// 	// std::vector<InputConfigInfo*> mLoadedInput;
// 	void clearLoadedInput();
};
