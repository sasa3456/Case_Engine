//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <span>
#include "GfxStates.h"
#include "GfxView.h"
#include "GfxResourceCommon.h"
#include "GfxShader.h"


// Namespace Case_Engine
namespace Case_Engine
{
	class GfxDevice;
	class GfxVertexShader;
	class GfxPixelShader;
	class GfxHullShader;
	class GfxDomainShader;
	class GfxGeometryShader;
	class GfxComputeShader;

	class GfxBuffer;
	class GfxTexture;
	class GfxQuery;

	struct GfxRenderPassDesc;
	class GfxRasterizerState;
	class GfxBlendState;
	class GfxDepthStencilState;
	class  GfxInputLayout;

	class GfxCommandContext
	{
		friend class GfxDevice;
	public:

		void Begin();
		void End();
		void Flush();
		void WaitForGPU();

		void Draw(uint32_t vertex_count, uint32_t instance_count = 1, uint32_t start_vertex_location = 0, uint32_t start_instance_location = 0);
		void DrawIndexed(uint32_t index_count, uint32_t instance_count = 1, uint32_t index_offset = 0, uint32_t base_vertex_location = 0, uint32_t start_instance_location = 0);
		void Dispatch(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z = 1);
		void DrawIndirect(GfxBuffer const& buffer, uint32_t offset);
		void DrawIndexedIndirect(GfxBuffer const& buffer, uint32_t offset);
		void DispatchIndirect(GfxBuffer const& buffer, uint32_t offset);
		
		void CopyBuffer(GfxBuffer& dst, GfxBuffer const& src);
		void CopyBuffer(GfxBuffer& dst, uint32_t dst_offset, GfxBuffer const& src, uint32_t src_offset, uint32_t size);
		void CopyTexture(GfxTexture& dst, GfxTexture const& src);
		void CopyTexture(GfxTexture& dst, uint32_t dst_mip, uint32_t dst_array, GfxTexture const& src, uint32_t src_mip, uint32_t src_array);

		void CopyStructureCount(GfxBuffer* dst_buffer, uint32_t dst_buffer_offset, GfxShaderResourceRW src_view);

		GfxMappedSubresource MapBuffer(GfxBuffer* buffer, GfxMapType map_type);
		void UnmapBuffer(GfxBuffer* buffer);
		GfxMappedSubresource MapTexture(GfxTexture* texture, GfxMapType map_type, uint32_t subresource = 0);
		void UnmapTexture(GfxTexture* texture, uint32_t subresource = 0);
		void UpdateBuffer(GfxBuffer* buffer, void const* data, uint32_t data_size);
		template<typename T>
		void UpdateBuffer(GfxBuffer* buffer, T const& data)
		{
			UpdateBuffer(buffer, &data, sizeof(data));
		}

		void BeginRenderPass(GfxRenderPassDesc const& render_pass_desc);
		void EndRenderPass();

		void SetTopology(GfxPrimitiveTopology topology);
		void SetIndexBuffer(GfxBuffer* index_buffer, uint32_t offset = 0);
		void SetVertexBuffer(GfxBuffer* vertex_buffer, uint32_t slot = 0);
		void SetVertexBuffers(std::span<GfxBuffer*> vertex_buffers, uint32_t start_slot = 0);
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void ClearReadWriteDescriptorFloat(GfxShaderResourceRW descriptor, const float v[4]);
		void ClearReadWriteDescriptorUint(GfxShaderResourceRW descriptor, const uint32_t v[4]);
		void ClearRenderTarget(GfxRenderTarget rtv, float const* clear_color);
		void ClearDepth(GfxDepthTarget dsv, float depth = 1.0f, uint8_t stencil = 0, bool clear_stencil = false);
		
		void SetInputLayout(GfxInputLayout* il);
		void SetDepthStencilState(GfxDepthStencilState* dss, uint32_t stencil_ref);
		void SetRasterizerState(GfxRasterizerState* rs);
		void SetBlendState(GfxBlendState* bs, float* blend_factors = nullptr, uint32_t mask = 0xffffffff);
		void SetVertexShader(GfxVertexShader* shader);
		void SetPixelShader(GfxPixelShader* shader);
		void SetHullShader(GfxHullShader* shader);
		void SetDomainShader(GfxDomainShader* shader);
		void SetGeometryShader(GfxGeometryShader* shader);
		void SetComputeShader(GfxComputeShader* shader);

		void SetConstantBuffer(GfxShaderStage stage, uint32_t slot, GfxBuffer* buffer);
		void SetConstantBuffers(GfxShaderStage stage, uint32_t start, std::span<GfxBuffer*> buffers);
		void SetSampler(GfxShaderStage stage, uint32_t start, GfxSampler* sampler);
		void SetSamplers(GfxShaderStage stage, uint32_t start, std::span<GfxSampler*> samplers);
		void SetShaderResourceRO(GfxShaderStage stage, uint32_t slot, GfxShaderResourceRO srv);
		void SetShaderResourcesRO(GfxShaderStage stage, uint32_t start, std::span<GfxShaderResourceRO> srvs);
		void UnsetShaderResourcesRO(GfxShaderStage stage, uint32_t start, uint32_t count);
		void SetShaderResourceRW(uint32_t slot, GfxShaderResourceRW uav);
		void SetShaderResourcesRW(uint32_t start, std::span<GfxShaderResourceRW> uavs);
		void SetShaderResourcesRW(uint32_t start, std::span<GfxShaderResourceRW> uavs, std::span<uint32_t> initial_counts);
		void UnsetShaderResourcesRW(uint32_t start, uint32_t count);

		void SetRenderTarget(GfxRenderTarget rtv, GfxDepthTarget dsv = nullptr);
		void SetRenderTargets(std::span<GfxRenderTarget> rtvs, GfxDepthTarget dsv = nullptr);

		void SetRenderTargetsAndShaderResourcesRW(std::span<GfxRenderTarget> rtvs, GfxDepthTarget dsv,
			uint32_t start_slot, std::span<GfxShaderResourceRW> uavs, std::span<uint32_t> initial_counts = {});

		void GenerateMips(GfxShaderResourceRO srv);

		void BeginQuery(GfxQuery* query);
		void EndQuery(GfxQuery* query);
		bool GetQueryData(GfxQuery* query, void* data, uint32_t data_size);

		void BeginEvent(char const* event_name);
		void EndEvent();

		ID3D11DeviceContext4* GetNative() const { return command_context.Get(); }
	private:
		GfxDevice* gfx = nullptr;
		uint32_t frame_count = 0;
		ArcPtr<ID3D11DeviceContext4> command_context = nullptr;
		ArcPtr<ID3DUserDefinedAnnotation> annotation = nullptr;

		GfxVertexShader* current_vs = nullptr;
		GfxPixelShader* current_ps = nullptr;
		GfxHullShader* current_hs = nullptr;
		GfxDomainShader* current_ds = nullptr;
		GfxGeometryShader* current_gs = nullptr;
		GfxComputeShader* current_cs = nullptr;

		GfxBlendState* current_blend_state = nullptr;
		GfxRasterizerState* current_rasterizer_state = nullptr;
		GfxDepthStencilState* current_depth_state = nullptr;
		GfxPrimitiveTopology current_topology = GfxPrimitiveTopology::Undefined;

		GfxRenderPassDesc* current_render_pass = nullptr;
		GfxInputLayout* current_input_layout = nullptr;

	private:
		explicit GfxCommandContext(GfxDevice* gfx) : gfx(gfx) {}
		void Create(ID3D11DeviceContext* ctx) 
		{
			HRESULT hr = ctx->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)command_context.GetAddressOf());
			GFX_CHECK_HR(hr);
			ctx->Release();
			hr = command_context->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), (void**)annotation.GetAddressOf());
			GFX_CHECK_HR(hr);
		}
	};
}