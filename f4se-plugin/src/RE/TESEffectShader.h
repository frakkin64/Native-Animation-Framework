#pragma once

// TESEffectShader::Play / Stop are not part of CommonLibF4, so they live here as free functions.
// IDs are { OG 1.10.163, NG 1.10.984 / AE 1.11.240 }.
namespace RE
{
	inline ReferenceEffect* InstantiateTransientHitEffect(TESObjectREFR* a_refr, TESEffectShader* a_shader, float a_f = -1.0f, TESObjectREFR* a_refr2 = nullptr, bool a_b1 = false, bool a_b2 = false, NiAVObject* a_obj = nullptr, bool a_b3 = false)
	{
		using func_t = decltype(&InstantiateTransientHitEffect);
		static REL::Relocation<func_t> func{ REL::VariantID{ 652173, 2205201 } };
		return func(a_refr, a_shader, a_f, a_refr2, a_b1, a_b2, a_obj, a_b3);
	}

	inline void FinishShaderEffect(ProcessLists* a_lists, TESObjectREFR* a_refr, TESEffectShader* a_shader)
	{
		using func_t = decltype(&FinishShaderEffect);
		static REL::Relocation<func_t> func{ REL::VariantID{ 631860, 2234097 } };
		return func(a_lists, a_refr, a_shader);
	}

	inline ReferenceEffect* PlayEffectShader(TESEffectShader* a_shader, TESObjectREFR* a_refr)
	{
		if (a_refr != nullptr && a_refr->parentCell != nullptr && a_refr->parentCell->loadedData != nullptr && a_refr->GetFullyLoaded3D() != nullptr) {
			return InstantiateTransientHitEffect(a_refr, a_shader);
		} else {
			return nullptr;
		}
	}

	inline void StopEffectShader(TESEffectShader* a_shader, TESObjectREFR* a_refr)
	{
		FinishShaderEffect(ProcessLists::GetSingleton(), a_refr, a_shader);
	}
}
