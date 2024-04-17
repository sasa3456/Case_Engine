//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Graphics/GfxVertexFormat.h"


// Namespace Case_Engine
namespace Case_Engine
{
	class Terrain
	{
	public:
		Terrain(std::vector<TexturedNormalVertex> const& terrain_vertices, float tx, float tz, uint64_t xcount, uint64_t zcount);

		float HeightAt(float x, float z) const;
		
		Vector3 NormalAt(float x, float z) const;

		std::pair<float, float> TileSizes() const 
		{
			return { tile_size_x, tile_size_z };
		}

		std::pair<uint64_t, uint64_t> TileCounts() const
		{
			return { tile_count_x, tile_count_z };
		}

	private:
		std::vector<TexturedNormalVertex> terrain_vertices;
		float tile_size_x;
		float tile_size_z;
		uint64_t tile_count_x;
		uint64_t tile_count_z;
		Vector3 offset;

	private:

		uint64_t GetIndex(uint64_t x_, uint64_t z_) const
		{
			return x_ + z_ * (tile_count_x + 1);
		}

		bool CheckIndex(uint64_t i) const
		{
			return i >= 0 && i < terrain_vertices.size();
		}
	};
}