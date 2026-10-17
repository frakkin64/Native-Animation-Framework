#pragma once
#include <fstream>
#pragma warning(push)
#pragma warning(disable: 4100)

namespace LooksMenu
{
	// Known f4ee.dll builds, identified by (PE TimeDateStamp, SizeOfImage) of the loaded module (same key as F4EEPatch).
	// Offsets are RVAs: hookRVA is the code the ApplyMorphsToShapes detour is placed on (OG 1.6.18/1.6.20 keep the
	// offsets NAF always used; the 1.7.x builds hook the function start, vtable slot 18 of BodyMorphInterface),
	// instanceRVA is the BodyMorphInterface singleton (its vptr is the vtable).
	struct BuildInfo
	{
		const char* name;
		std::uint32_t timestamp;
		std::uint32_t imageSize;
		std::uint64_t hookRVA;
		std::uint64_t instanceRVA;
	};

	inline constexpr BuildInfo kBuilds[] = {
		{ "1.6.18", 0x5DEDC245, 0x10E000, 0x10040, 0xFB440 },
		{ "1.6.20", 0x5FA27A28, 0x10F000, 0xFFCC, 0xF4DF0 },
		{ "1.7.0.1", 0x665384C3, 0x11B000, 0x11480, 0x107000 },
		{ "1.7.0.2", 0x696BD132, 0x11B000, 0x11480, 0x107320 },
		{ "1.7.0.3", 0x6A234220, 0x11B000, 0x11400, 0x107E00 },
		{ "1.7.0.4", 0x6A95CC57, 0x122000, 0x11780, 0x10DD30 },
	};

	class BodyMorphInterface
	{
	public:
		virtual void Save(const F4SE::SerializationInterface*, uint32_t) {}
		virtual bool Load(const F4SE::SerializationInterface*, bool, uint32_t, const std::unordered_map<uint32_t, std::string>&) { return false; }
		virtual void Revert() {}

		virtual void LoadBodyGenSliderMods() {}
		virtual void ClearBodyGenSliders() {}

		virtual bool LoadBodyGenSliders(const std::string& filePath) { return false; }

		virtual void ForEachSlider(uint8_t gender, std::function<void(const std::shared_ptr<uint8_t>& slider)> func) {}

		virtual std::shared_ptr<uint8_t> GetTrishapeMap(const char* relativePath) { return nullptr; }
		virtual std::shared_ptr<uint8_t> GetMorphMap(RE::Actor* actor, bool isFemale) { return nullptr; }

		virtual void SetMorph(RE::Actor* actor, bool isFemale, const RE::BSFixedString& morph, RE::BGSKeyword* keyword, float value) {}
		virtual float GetMorph(RE::Actor* actor, bool isFemale, const RE::BSFixedString& morph, RE::BGSKeyword* keyword) { return 0.0f; }

		virtual void GetKeywords(RE::Actor* actor, bool isFemale, const RE::BSFixedString& morph, std::vector<RE::BGSKeyword*>& keywords) {}
		virtual void GetMorphs(RE::Actor* actor, bool isFemale, std::vector<RE::BSFixedString>& morphs) {}
		virtual void RemoveMorphsByName(RE::Actor* actor, bool isFemale, const RE::BSFixedString& morph) {}
		virtual void RemoveMorphsByKeyword(RE::Actor* actor, bool isFemale, RE::BGSKeyword* keyword) {}
		virtual void ClearMorphs(RE::Actor* actor, bool isFemale) {}
		virtual void CloneMorphs(RE::Actor* source, RE::Actor* target) {}

		virtual void GetMorphableShapes(RE::NiAVObject* node, std::vector<std::shared_ptr<uint8_t>>& shapes) {}
		virtual bool ApplyMorphsToShapes(RE::Actor* actor, RE::NiAVObject* slotNode) { return false; }
		virtual bool ApplyMorphsToShape(RE::Actor* actor, const std::shared_ptr<uint8_t>& morphableShape) { return false; }
		virtual bool UpdateMorphs(RE::Actor* actor) { return false; }
	};

	bool isInstalled = false;
	const BuildInfo* build = nullptr;

	namespace detail
	{
		typedef bool(ApplyMorphsToShapes)(BodyMorphInterface*, RE::Actor*, RE::NiAVObject*);
		typedef void(LoadBodyGenSliderMods)(BodyMorphInterface*);
		DetourXS applyMorphsHook;
		ApplyMorphsToShapes* originalApply;

		BodyMorphInterface* g_bodyMorphInterface = nullptr;

		bool HookedApplyMorphs(BodyMorphInterface* a1, RE::Actor* a2, RE::NiAVObject* a3) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			return originalApply(a1, a2, a3);
		}

		void RegisterHook() {
			uintptr_t baseAddr = reinterpret_cast<uintptr_t>(GetModuleHandleA("f4ee.dll"));

			if (!applyMorphsHook.Create(reinterpret_cast<LPVOID>(baseAddr + build->hookRVA), &HookedApplyMorphs)) {
				REX::WARN("Failed to create ApplyMorphsToShapes hook!");
			} else {
				originalApply = reinterpret_cast<ApplyMorphsToShapes*>(applyMorphsHook.GetTrampoline());
			}

			g_bodyMorphInterface = reinterpret_cast<BodyMorphInterface*>(baseAddr + build->instanceRVA);
		}
	}

	void Init() {
		// F4EE has never changed its plugin version between builds, so the loaded image is identified instead.
		const auto module = GetModuleHandleA("f4ee.dll");
		if (!module) {
			REX::INFO("LooksMenu not installed, support disabled.");
			return;
		}

		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(module);
		const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const std::uint8_t*>(module) + dos->e_lfanew);
		const auto timestamp = nt->FileHeader.TimeDateStamp;
		const auto imageSize = nt->OptionalHeader.SizeOfImage;

		for (auto& b : kBuilds) {
			if (b.timestamp == timestamp && b.imageSize == imageSize) {
				build = &b;
				break;
			}
		}

		if (build) {
			isInstalled = true;
			REX::INFO("LooksMenu version {} detected.", build->name);
			detail::RegisterHook();
		} else {
			REX::INFO("Unsupported LooksMenu version. Timestamp: {:X}, image size: {:X}", timestamp, imageSize);
		}
	}

	void SetMorph(RE::Actor* actor, const RE::BSFixedString& morph, RE::BGSKeyword* keyword, float value)
	{
		if (isInstalled && actor && detail::g_bodyMorphInterface) {
			bool isFemale = (actor->GetSex() == RE::SEX::kFemale);
			detail::g_bodyMorphInterface->SetMorph(actor, isFemale, morph, keyword, value);
		}
	}

	void UpdateMorphs(RE::Actor* actor)
	{
		if (isInstalled && actor && detail::g_bodyMorphInterface) {
			detail::g_bodyMorphInterface->UpdateMorphs(actor);
		}
	}

	void RemoveMorphsByName(RE::Actor* actor, const RE::BSFixedString& morph) {
		if (isInstalled && actor && detail::g_bodyMorphInterface) {
			bool isFemale = (actor->GetSex() == RE::SEX::kFemale);
			detail::g_bodyMorphInterface->RemoveMorphsByName(actor, isFemale, morph);
		}
	}
}

#pragma warning(pop)
