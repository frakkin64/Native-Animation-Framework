#pragma once

namespace RE
{
	class GeometryReader
	{
	private:
		template <typename T>
		T get_val(uint8_t*& blockPos)
		{
			T result = *reinterpret_cast<T*>(blockPos);
			blockPos += sizeof(T);
			return result;
		}

	public:
		GeometryReader(RE::BSTriShape* geo)
		{
			uint64_t vertDesc = geo->vertexDesc.desc;
			uint32_t vertSize = static_cast<uint32_t>((vertDesc << 2) & 0x3C);
			uint32_t numVerts = geo->numVertices;
			vertices.resize(numVerts);

			uint8_t* vertBlock = static_cast<uint8_t*>(static_cast<RE::BSGraphics::TriShape*>(geo->rendererData)->vertexBuffer->data);
			for (uint32_t i = 0; i < numVerts; i++) {
				uint8_t* blockPos = &vertBlock[i * vertSize];

				if (geo->vertexDesc.HasFlag(RE::BSGraphics::Vertex::VF_FULLPREC)) {
					vertices[i].x = get_val<float>(blockPos);
					vertices[i].y = get_val<float>(blockPos);
					vertices[i].z = get_val<float>(blockPos);
				} else {
					vertices[i].x = fp16_ieee_to_fp32_value(get_val<uint16_t>(blockPos));
					vertices[i].y = fp16_ieee_to_fp32_value(get_val<uint16_t>(blockPos));
					vertices[i].z = fp16_ieee_to_fp32_value(get_val<uint16_t>(blockPos));
				}
			}
		}

		std::vector<RE::NiPoint3> vertices;
	};
}
