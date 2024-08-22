#include "GuiControllersAssignmentsPerEmulator.h"

#include "components/SwitchComponent.h"
#include "components/OptionListComponent.h"

#include "guis/GuiDetectDevice.h"
#include "guis/GuiBluetoothPair.h"
#include "ThreadedBluetooth.h"
#include "guis/GuiBluetoothDevices.h"

#include "guis/GuiMsgBox.h"
#include "InputManager.h"
#include "SystemConf.h"
#include "SystemData.h"
#include "GuiControllersAssignmentForEmulator.h"

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

std::string GuiControllersAssignmentsPerEmulator::getControllersSettingsLabel()
{
	return controllers_settings_label;
}

void GuiControllersAssignmentsPerEmulator::openControllersSettings(Window* wnd, int autoSel)
{
	wnd->pushGui(new GuiControllersAssignmentsPerEmulator(wnd, autoSel));
}

GuiControllersAssignmentsPerEmulator::GuiControllersAssignmentsPerEmulator(Window* wnd, int autoSel) : GuiSettings(wnd, controllers_settings_label.c_str())
{
	Window* window = mWindow;

	addGroup(_("Game Systems"));
	std::string systemName;

	for (auto system : SystemData::sSystemVector)
	{
		if (system->isVisible() && system->isGameSystem())
		{
			// if (system->getName() != "all")
			// {
				systemName = system->getName();
				addEntry(systemName, true, [window, systemName] { GuiControllersAssignmentForEmulator::openControllersSettings(window, systemName); });
				// addEntry(GuiControllersAssignmentsPerEmulator::getControllersSettingsLabel(), true, [window] { GuiControllersAssignmentsPerEmulator::openControllersSettings(window); }, "iconControllers");
			// }
		}
	}
}