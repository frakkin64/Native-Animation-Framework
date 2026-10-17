#pragma once

namespace RE
{
	class MovementControllerNPC
	{
	public:
		void SetMovementSpeed(float a_speed)
		{
			using func_t = decltype(&MovementControllerNPC::SetMovementSpeed);
			static REL::Relocation<func_t> func{ REL::VariantID{ 356321, 2234400 } };
			return func(this, a_speed);
		}
	};
}
