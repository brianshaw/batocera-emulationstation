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
#include "GuiControllersSettings.h"

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

void GuiControllersAssignmentForEmulator::openControllersSettings(Window* wnd, std::string systemName)
{
	wnd->pushGui(new GuiControllersAssignmentForEmulator(wnd, systemName));
}

GuiControllersAssignmentForEmulator::GuiControllersAssignmentForEmulator(Window* wnd, std::string systemName) : GuiSettings(wnd, controllers_settings_label.c_str())
{
	Window* window = mWindow;

	addGroup(systemName);

	// Here we go; for each player
	std::list<int> alreadyTaken = std::list<int>();

	// clear the current loaded inputs
	clearLoadedInput();

	std::vector<std::shared_ptr<OptionListComponent<InputConfigInfo*>>> options;

	auto configList = InputManager::getInstance()->getInputConfigs();

#if WIN32
	for (int player = 0; player < MAX_PLAYERS; player++)
	{
		std::string label = Utils::String::format(gettext_controllerid.c_str(), player + 1);

		auto inputOptionList = std::make_shared<OptionListComponent<InputConfigInfo*> >(mWindow, label, false);
		inputOptionList->add(_("default"), nullptr, false);
		options.push_back(inputOptionList);

		// Checking if a setting has been saved, else setting to default
		std::string configuratedName = Settings::getInstance()->getString(Utils::String::format("INPUT P%iNAME", player + 1));
		std::string configuratedGuid = Settings::getInstance()->getString(Utils::String::format("INPUT P%iGUID", player + 1));
		std::string configuratedPath = Settings::getInstance()->getString(Utils::String::format("INPUT P%iPATH", player + 1));

		bool found = false;

		// Add configurated controller even if it's disconnected
		InputConfigInfo* defaultInputConfig = nullptr;

		if (!configuratedPath.empty())
		{
			InputConfigInfo* newInputConfig = new InputConfigInfo(configuratedName, configuratedGuid, configuratedPath);
			mLoadedInput.push_back(newInputConfig);

			auto it = std::find_if(configList.cbegin(), configList.cend(), [configuratedPath](InputConfig* x) { return x->getSortDevicePath() == configuratedPath; });
			if (it != configList.cend())
			{
				if (std::find(alreadyTaken.begin(), alreadyTaken.end(), (*it)->getDeviceId()) == alreadyTaken.end())
				{
					inputOptionList->addEx(configuratedName, configuratedPath, newInputConfig, true, false, false);
					alreadyTaken.push_back((*it)->getDeviceId());
					defaultInputConfig = newInputConfig;
				}
			}
			else
				inputOptionList->addEx(configuratedName + " (" + _("NOT CONNECTED") + ")", configuratedPath, newInputConfig, true, false, false);

			found = true;
		}

		// for each available and configured input
		for (auto config : configList)
		{
			if (defaultInputConfig != nullptr && defaultInputConfig->name == config->getDeviceName() && defaultInputConfig->guid == config->getDeviceGUIDString() && defaultInputConfig->path == config->getSortDevicePath())
				continue;

			std::string displayName = config->getDeviceName();

			bool foundFromConfig = !configuratedPath.empty() ? config->getSortDevicePath() == configuratedPath : configuratedName == config->getDeviceName() && configuratedGuid == config->getDeviceGUIDString();
			int deviceID = config->getDeviceId();

			InputConfigInfo* newInputConfig = new InputConfigInfo(config->getDeviceName(), config->getDeviceGUIDString(), config->getSortDevicePath());
			mLoadedInput.push_back(newInputConfig);

			if (foundFromConfig && std::find(alreadyTaken.begin(), alreadyTaken.end(), deviceID) == alreadyTaken.end() && !found)
			{
				found = true;
				alreadyTaken.push_back(deviceID);

				LOG(LogWarning) << "adding entry for player" << player << " (selected): " << config->getDeviceName() << "  " << config->getDeviceGUIDString() << "  " << config->getDevicePath();
				inputOptionList->addEx(displayName, config->getDevicePath(), newInputConfig, true, false, false);
			}
			else
			{
				LOG(LogInfo) << "adding entry for player" << player << " (not selected): " << config->getDeviceName() << "  " << config->getDeviceGUIDString() << "  " << config->getDevicePath();
				inputOptionList->addEx(displayName, config->getDevicePath(), newInputConfig, false, false, false);
			}
		}

		if (!inputOptionList->hasSelection())
			inputOptionList->selectFirstItem();

		// Populate controllers list
		addWithLabel(label, inputOptionList);
	}
#else
	for (int player = 0; player < MAX_PLAYERS; player++)
	{
		std::string confName = Utils::String::format("INPUT P%iNAME", player + 1);
		std::string confGuid = Utils::String::format("INPUT P%iGUID", player + 1);
		std::string confPath = Utils::String::format("INPUT P%iPATH", player + 1);

		std::string label = Utils::String::format(gettext_playerid.c_str(), player + 1);

		auto inputOptionList = std::make_shared<OptionListComponent<InputConfigInfo*> >(mWindow, label, false);
		inputOptionList->add(_("default"), nullptr, false);
		options.push_back(inputOptionList);

		// Checking if a setting has been saved, else setting to default
		std::string configuratedName = Settings::getInstance()->getString(confName);
		std::string configuratedGuid = Settings::getInstance()->getString(confGuid);
		std::string configuratedPath = Settings::getInstance()->getString(confPath);

		bool found = false;

		// For each available and configured input
		for (auto config : configList)
		{
			std::string displayName = "#" + std::to_string(config->getDeviceIndex()) + " " + config->getDeviceName();
			bool foundFromConfig = !configuratedPath.empty() ? config->getSortDevicePath() == configuratedPath : configuratedName == config->getDeviceName() && configuratedGuid == config->getDeviceGUIDString();

			int deviceID = config->getDeviceId();

			InputConfigInfo* newInputConfig = new InputConfigInfo(config->getDeviceName(), config->getDeviceGUIDString(), config->getSortDevicePath());
			mLoadedInput.push_back(newInputConfig);

			if (foundFromConfig && std::find(alreadyTaken.begin(), alreadyTaken.end(), deviceID) == alreadyTaken.end() && !found)
			{
				found = true;
				alreadyTaken.push_back(deviceID);

				LOG(LogWarning) << "adding entry for player" << player << " (selected): " << config->getDeviceName() << "  " << config->getDeviceGUIDString() << "  " << config->getDevicePath();
				inputOptionList->add(displayName, newInputConfig, true);
			}
			else
			{
				LOG(LogInfo) << "adding entry for player" << player << " (not selected): " << config->getDeviceName() << "  " << config->getDeviceGUIDString() << "  " << config->getDevicePath();
				inputOptionList->add(displayName, newInputConfig, false);
			}
		}

		if (!inputOptionList->hasSelection())
			inputOptionList->selectFirstItem();

		// Populate controllers list
		addWithLabel(label, inputOptionList);
	}
#endif

	addSaveFunc([this, options, window]
	{
		bool changed = false;

		for (int player = 0; player < MAX_PLAYERS; player++)
		{
			std::string confName = Utils::String::format("INPUT P%iNAME", player + 1);
			std::string confGuid = Utils::String::format("INPUT P%iGUID", player + 1);
			std::string confPath = Utils::String::format("INPUT P%iPATH", player + 1);

			auto input = options.at(player);

			InputConfigInfo* selected = input->getSelected();
			if (selected == nullptr)
			{
				changed |= Settings::getInstance()->setString(confName, "DEFAULT");
				changed |= Settings::getInstance()->setString(confGuid, "");
				changed |= Settings::getInstance()->setString(confPath, "");
			}
			else if (input->changed())
			{
				LOG(LogInfo) << "Found the selected controller : " << input->getSelectedName() << ", " << selected->guid << ", " << selected->path;

				changed |= Settings::getInstance()->setString(confName, selected->name);
				changed |= Settings::getInstance()->setString(confGuid, selected->guid);
				changed |= Settings::getInstance()->setString(confPath, selected->path);
			}
		}

		if (changed)
			Settings::getInstance()->saveFile();

		// this is dependant of this configuration, thus update it
		InputManager::getInstance()->computeLastKnownPlayersDeviceIndexes();
	});

}

void GuiControllersAssignmentForEmulator::clearLoadedInput() 
{
	for (int i = 0; i < mLoadedInput.size(); i++) 
		delete mLoadedInput[i];

	mLoadedInput.clear();
}

GuiControllersAssignmentForEmulator::~GuiControllersAssignmentForEmulator() 
{
	clearLoadedInput();
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