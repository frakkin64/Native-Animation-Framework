#pragma once

#pragma warning(push)
#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#pragma warning(pop)

#define DLLEXPORT __declspec(dllexport)

using namespace std::literals;

#define M_PI 3.1415926535897

#include "Version.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/base_class.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/variant.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/optional.hpp"
#include "cereal/types/atomic.hpp"
#include "cereal/types/utility.hpp"
namespace cereal
{
	// CommonLibF4 does not provide serialization for its math types.
	template <class Archive>
	void serialize(Archive& ar, RE::NiPoint3& a_point)
	{
		ar(a_point.x, a_point.y, a_point.z);
	}
}
#include <pugixml.hpp>
#include "zstr.hpp"
#include "nlohmann/json.hpp"
#include "ik/ik.h"
#include "BodyAnimation/Spline.h"
#include "libzippp/libzippp.h"
#include "fp16.h"

// wingdi.h defines ERROR, which collides with REX::ERROR
#undef ERROR
