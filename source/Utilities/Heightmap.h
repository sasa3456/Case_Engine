//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <vector>
#include <string_view>


// Namespace Case_Engine
namespace Case_Engine
{
	enum class NoiseType
	{
		OpenSimplex2,
		OpenSimplex2S,
		Cellular,
		Perlin,
		ValueCubic,
		Value
	};
	enum class FractalType
	{
		None,
		FBM,
		Ridged,
		PingPong
	};
	struct NoiseDesc
	{
		uint32_t width;
		uint32_t depth;
		uint32_t max_height;
		FractalType fractal_type = FractalType::None;
		NoiseType noise_type = NoiseType::Perlin;
		int32_t seed = 1337;
		float   frequency = 0.01f;
		float   persistence = 0.5f;
		float   lacunarity = 2.0f;
		int32_t octaves = 3;
		float   noise_scale = 10;
	};


	struct ThermalErosionDesc
	{
		int32_t iterations;
		float   c;
		float   talus;
	};

	
	struct HydraulicErosionDesc
	{
		int32_t iterations;
		int32_t drops;
		float   carrying_capacity;
		float   deposition_speed;
	};

	class Heightmap
	{
	public:
		
		explicit Heightmap(NoiseDesc const& desc);
		Heightmap(std::string_view heightmap_path, uint32_t max_height);

		float HeightAt(uint64_t x, uint64_t z) const;
		uint64_t Width() const;
		uint64_t Depth() const;

		void ApplyThermalErosion(ThermalErosionDesc const& desc);
		void ApplyHydraulicErosion(HydraulicErosionDesc const& desc);

	private:
		std::vector<std::vector<float>> hm;
	};
}

