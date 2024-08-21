#include "GuiControllersAssignmentForEmulator.h"

#include "components/SwitchComponent.h"
#include "components/OptionListComponent.h"

#include "guis/GuiDetectDevice.h"
#include "guis/GuiBluetoothPair.h"
#include "ThreadedBluetooth.h"
#include "guis/GuiBluetoothDevices.h"

#include "guis/GuiMsgBox.h"
#include "InputManager.h"
#include "SystemConf.h"

#define gettext_controllers_settings				_("CONTROLLER SETTINGS")
#define gettext_controllers_and_bluetooth_settings  _("CONTROLLER ASSIGNMENT PER EMULATOR")

#define gettext_controllers_priority _("CONTROLLERS PRIORITY")
#define gettext_controllers_player_assigments _("PLAYER ASSIGNMENTS")

#define gettext_controllerid _("CONTROLLER #%i")
#define gettext_playerid _("P%i'S CONTROLLER")

// Windows build does not have bluetooth support, so affect the label for Windows
#if WIN32
#define controllers_settings_label		gettext_controllers_settings
#define controllers_group_label		gettext_controllers_priority
#else
#define controllers_settings_label		gettext_controllers_and_bluetooth_settings
#define controllers_group_label		gettext_controllers_player_assigments
#endif

std::string GuiControllersAssignmentForEmulator::getControllersSettingsLabel()
{
	return controllers_settings_label;
}

void GuiControllersAssignmentForEmulator::openControllersSettings(Window* wnd, const char* systemName)
{
	wnd->pushGui(new GuiControllersAssignmentForEmulator(wnd, systemName));
}

GuiControllersAssignmentForEmulator::GuiControllersAssignmentForEmulator(Window* wnd, const char* systemName) : GuiSettings(wnd, controllers_settings_label.c_str())
{
	Window* window = mWindow;

	addGroup(systemName);

}

void GuiControllersAssignmentForEmulator::test_func()
{
	LOG(LogError) << "test_func()";
}
// mStringMap["INPUT P1NAME"] = "DEFAULT";
// 	mStringMap["INPUT P2NAME"] = "DEFAULT";
// 	mStringMap["INPUT P3NAME"] = "DEFAULT";
// 	mStringMap["INPUT P4NAME"] = "DEFAULT";
// 	mStringMap["INPUT P5NAME"] = "DEFAULT";
// 	mStringMap["INPUT P6NAME"] = "DEFAULT";
// 	mStringMap["INPUT P7NAME"] = "DEFAULT";
// 	mStringMap["INPUT P8NAME"] = "DEFAULT";