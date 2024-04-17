//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <unordered_map>
#include <DirectXMath.h>
#include "Enums.h"
#include "Components.h"
#include "tecs/registry.h"
#include "Graphics/GfxShaderProgram.h"
#include "Graphics/GfxConstantBuffer.h"
#include "Graphics/GfxBuffer.h"
#include "Graphics/GfxTexture.h"
#include "Graphics/GfxDevice.h"


// Namespace Case_Engine
namespace Case_Engine
{
	//based on AMD GPU Particles Sample: https://github.com/GPUOpen-LibrariesAndSDKs/GPUParticles11

	class ParticleRenderer
	{
		static constexpr uint32_t MAX_PARTICLES = 400 * 1024;

		struct GPUParticleA
		{
			Vector4		TintAndAlpha;
			float		Rotation;					
			uint32_t    IsSleeping;					
		};
		struct GPUParticleB
		{
			Vector3		Position;
			float		Mass;						

			Vector3		Velocity;
			float		Lifespan;					

			float		DistanceToEye;				
			float		Age;						
			float		StartSize;					
			float		EndSize;					
		};
		struct EmitterCBuffer
		{
			Vector4	EmitterPosition;
			Vector4	EmitterVelocity;
			Vector4	PositionVariance;

			int32_t	MaxParticlesThisFrame;
			float	ParticleLifeSpan;
			float	StartSize;
			float	EndSize;
			
			float	VelocityVariance;
			float	Mass;
			float	ElapsedTime;
			int32_t Collisions;

			int32_t CollisionThickness;
		};
		struct IndexBufferElement
		{
			float	distance;	
			float	index;		
		};
		struct ViewSpacePositionRadius
		{
			Vector3 viewspace_position;
			float radius;
		};
		struct SortDispatchInfo
		{
			int32_t x, y, z, w;
		};
	public:
		explicit ParticleRenderer(GfxDevice* gfx);

		void Update(float dt, Emitter& emitter_params);

		void Render(Emitter const& emitter_params,
					GfxShaderResourceRO depth_srv, 
					GfxShaderResourceRO particle_srv);

	private:
		GfxDevice* gfx;

		std::unique_ptr<GfxTexture> random_texture;
		GfxBuffer dead_list_buffer;
		GfxBuffer particle_bufferA;
		GfxBuffer particle_bufferB;
		GfxBuffer view_space_positions_buffer;
		GfxBuffer alive_index_buffer;

		GfxConstantBuffer<uint32_t> dead_list_count_cbuffer;
		GfxConstantBuffer<uint32_t> active_list_count_cbuffer;
		GfxConstantBuffer<EmitterCBuffer> emitter_cbuffer;
		GfxConstantBuffer<SortDispatchInfo> sort_dispatch_info_cbuffer;

		GfxBuffer indirect_render_args_buffer;
		GfxBuffer indirect_sort_args_buffer;
		std::unique_ptr<GfxBuffer> index_buffer;

	private:
		void CreateViews();
		void CreateIndexBuffer();
		void CreateRandomTexture();

		void InitializeDeadList();
		void ResetParticles();
		void Emit(Emitter const& emitter_params);
		void Simulate(GfxShaderResourceRO depth_srv);
		void Rasterize(Emitter const& emitter_params, GfxShaderResourceRO depth_srv, GfxShaderResourceRO particle_srv);
		void Sort();
		
		bool SortInitial();
		bool SortIncremental(uint32_t presorted);
	};
}