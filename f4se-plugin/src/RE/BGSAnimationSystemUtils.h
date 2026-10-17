#pragma once

// Extends RE::BGSAnimationSystemUtils (CommonLibF4 provides GetActiveSyncInfo,
// InitializeActorInstant and IsActiveGraphInTransition) with the functions NAF
// needs. IDs are { OG 1.10.163, NG 1.10.984 / AE 1.11.240 }.
namespace RE
{
	namespace BGSAnimationSystemUtils
	{
		inline bool IsIdleLoading(RE::TESObjectREFR* a_ref, const BSFixedString& a_idl)
		{
			using func_t = decltype(&IsIdleLoading);
			static REL::Relocation<func_t> func{ REL::VariantID{ 686648, 2236391 } };
			return func(a_ref, a_idl);
		}

		inline bool WillEventChangeState(const TESObjectREFR& ref, const BSFixedString& evn)
		{
			using func_t = decltype(&WillEventChangeState);
			static REL::Relocation<func_t> func{ REL::VariantID{ 35074, 2214271 } };
			return func(ref, evn);
		}

		inline float GetAnimationMaxDuration(TESObjectREFR* a_refr)
		{
			using func_t = decltype(&GetAnimationMaxDuration);
			static REL::Relocation<func_t> func{ REL::VariantID{ 177755, 2214279 } };
			return func(a_refr);
		}

		// Replaces AnimationSystemUtils::GetEventSourcePointersFromGraph(TESObjectREFR&,
		// BSScrapArray<BSTEventSource<BSAnimationGraphEvent>*>&) (OG 897074), which has no
		// standalone function in NG/AE. It gets the graph manager through the holder and,
		// under the manager's updateLock, collects the BSTEventSource<BSAnimationGraphEvent>
		// (a BShkbAnimationGraph member at +0x68) of every graph.
		inline void NotifyGraphSources(const TESObjectREFR* a_refr, RE::BSAnimationGraphEvent& a_event)
		{
			constexpr std::size_t kEventSourceOffset = 0x68;

			auto manager = GetAnimationGraphManager(a_refr);
			if (!manager) {
				return;
			}

			std::vector<BSTEventSource<BSAnimationGraphEvent>*> sources;
			{
				BSAutoLock lock{ manager->updateLock };
				for (auto& graph : manager->graph) {
					if (graph) {
						sources.push_back(reinterpret_cast<BSTEventSource<BSAnimationGraphEvent>*>(reinterpret_cast<std::uintptr_t>(graph.get()) + kEventSourceOffset));
					}
				}
			}

			for (auto s : sources) {
				s->Notify(a_event);
			}
		}

		inline BGSAction* GetDefaultObjectForActionInitializeToBaseState()
		{
			using func_t = decltype(&GetDefaultObjectForActionInitializeToBaseState);
			static REL::Relocation<func_t> func{ REL::VariantID{ 639576, 2214309 } };
			return func();
		}

		inline bool RunActionOnActor(Actor* a_actor, TESActionData& a_action, bool a_unk = false)
		{
			using func_t = decltype(&RunActionOnActor);
			static REL::Relocation<func_t> func{ REL::VariantID{ 22408, 2236388 } };
			return func(a_actor, a_action, a_unk);
		}
	}
}
