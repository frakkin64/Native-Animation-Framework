#pragma once

// Event registration wrappers that the game exposes as TESObjectREFR_Events::Register*/Unregister*.
// CommonLibF4 exposes GetEventSource() for TESPackageEvent, TESHitEvent and TESDeathEvent; these
// wrappers (OG 1.10.163 ID, NG/AE ID) do the same lookup + RegisterSink/UnregisterSink.
namespace RE
{
	class TESActorLocationChangeEvent
	{
	public:
		NiPointer<TESObjectREFR> refr;
		BGSLocation*             oldLocation;
		BGSLocation*             newLocation;
	};

	namespace TESObjectREFR_Events
	{
		inline void RegisterForPackage(BSTEventSink<TESPackageEvent>* a_sink)
		{
			using func_t = decltype(&RegisterForPackage);
			static REL::Relocation<func_t> func{ REL::VariantID{ 1448133, 2201672 } };
			return func(a_sink);
		}

		inline void UnregisterForPackage(BSTEventSink<TESPackageEvent>* a_sink)
		{
			using func_t = decltype(&UnregisterForPackage);
			static REL::Relocation<func_t> func{ REL::VariantID{ 1182113, 2201673 } };
			return func(a_sink);
		}

		inline void RegisterForHit(BSTEventSink<TESHitEvent>* a_sink)
		{
			using func_t = decltype(&RegisterForHit);
			static REL::Relocation<func_t> func{ REL::VariantID{ 1240328, 2201635 } };
			return func(a_sink);
		}

		inline void UnregisterForHit(BSTEventSink<TESHitEvent>* a_sink)
		{
			using func_t = decltype(&UnregisterForHit);
			static REL::Relocation<func_t> func{ REL::VariantID{ 973940, 2201636 } };
			return func(a_sink);
		}

		inline void RegisterForDeath(BSTEventSink<TESDeathEvent>* a_sink)
		{
			using func_t = decltype(&RegisterForDeath);
			static REL::Relocation<func_t> func{ REL::VariantID{ 33618, 2201601 } };
			return func(a_sink);
		}

		inline void RegisterForActorLocationChange(BSTEventSink<TESActorLocationChangeEvent>* a_sink)
		{
			using func_t = decltype(&RegisterForActorLocationChange);
			static REL::Relocation<func_t> func{ REL::VariantID{ 78267, 2201562 } };
			return func(a_sink);
		}
	}
}
