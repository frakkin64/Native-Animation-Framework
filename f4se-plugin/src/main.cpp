bool g_gameDataReady = false;

#include "Data/Constants.h"
#include "Misc/Utility.h"
#include "Data/Events.h"
#include "RE/ExtraREClasses.h"
#include "Serialization/General.h"
#include "Tasks/TaskFunctor.h"
#include "PackageOverride/EvalHook.h"
#include "LooksMenu/LooksMenu.h"
#include "FaceAnimation/AnimationData.h"
#include "Data/Global.h"
#include "Data/CommandEngine.h"
#include "BodyAnimation/GraphHook.h"
#include "BodyAnimation/SmartIdle.h"
#include "CamHook/CamHook.h"
#include "Misc/GameUtil.h"
#include "Data/Uid.h"
#include "Menu/NAFHUDMenu/SceneHUD.h"
#include "Scene/IScene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneBase.h"
#include "FaceAnimation/FaceUpdateHook.h"
#include "Tasks/GameLoopHook.h"
#include "Menu/Menu.h"
#include "Scripts/Papyrus.h"
#include "Serialization/Serialization.h"
#include "API/API.h"
namespace
{
	void MessageHandler(F4SE::MessagingInterface::Message* a_msg)
	{
		if (!a_msg) {
			return;
		}

		switch (a_msg->type) {
			case F4SE::MessagingInterface::kGameDataReady:
			{
				if (static_cast<bool>(a_msg->data)) {
					REX::INFO("Game data finished loading, registering NAF menu...");

					auto ui = RE::UI::GetSingleton();
					if (ui) {
						ui->RegisterSink(Tasks::TimerThread::GetSingleton());
						ui->RegisterSink(Menu::HUDManager::GetSingleton());
						Menu::Register(ui);
					}
					
					REX::INFO("Linking references...");
					Utility::StartPerformanceCounter();

					Data::Global::InitGameData();

					double performanceSeconds = Utility::GetPerformanceCounter();
					REX::INFO("Finished linking in {:.3f}s", performanceSeconds);
					REX::INFO("Ready!");

					g_gameDataReady = true;
					Data::Events::Send(Data::Events::GAME_DATA_READY);
				}

				break;
			}
			case F4SE::MessagingInterface::kPostLoad:
			{
				LooksMenu::Init();
			}
			default:
				break;
		}
	}

	void InitializeHooking() {
		auto& trampoline = REL::GetTrampoline();
		Tasks::GameLoopHook::RegisterHook(trampoline);
		BodyAnimation::GraphHook::RegisterHook();
		FaceAnimation::FaceUpdateHook::RegisterHook(trampoline);
		Menu::HUDManager::RegisterHook(trampoline);
		PackageOverride::EvalHook::RegisterHook();
		CamHook::RegisterHook();
	}
}

F4SE_PLUGIN_QUERY(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
	if (const auto data = F4SE::PluginVersionData::GetSingleton()) {
		a_info->infoVersion = F4SE::PluginInfo::kVersion;
		a_info->name = data->GetPluginName().data();
		a_info->version = data->GetPluginVersion().pack();
	}

	if (a_f4se->IsEditor()) {
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < REL::Version(F4SE::RUNTIME_1_10_163)) {
		return false;
	}

	return true;
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .logName = "NAF", .trampoline = true, .trampolineSize = 256 });
	InitializeHooking();

	const auto messaging = F4SE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler)) {
		REX::CRITICAL("Failed to get F4SE messaging interface, marking as incompatible.");
		return false;
	} else {
		REX::INFO("Registered with F4SE messaging interface.");
	}

	const auto serialization = F4SE::GetSerializationInterface();
	if (!serialization) {
		REX::CRITICAL("Failed to get F4SE serialization interface, marking as incompatible.");
		return false;
	} else {
		serialization->SetUniqueID('NAF');
		serialization->SetSaveCallback(Serialization::SaveCallback);
		serialization->SetLoadCallback(Serialization::LoadCallback);
		serialization->SetRevertCallback(Serialization::RevertCallback);
		REX::INFO("Registered with F4SE serialization interface.");
	}

	const auto papyrus = F4SE::GetPapyrusInterface();
	if (!papyrus || !papyrus->Register(Papyrus::RegisterFunctions)) {
		REX::CRITICAL("Failed to register Papyrus functions!");
	} else {
		REX::INFO("Registered Papyrus functions.");
	}

	Data::Global::Init();

	REX::INFO("{:s} initialization successful, waiting for game data load.", Version::PROJECT);

	return true;
}
