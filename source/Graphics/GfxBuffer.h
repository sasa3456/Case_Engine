//-----------------------------------------------------
// � Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <vector>
#include "GfxDevice.h"
#include "GfxResourceCommon.h"
#include "GfxView.h"
#include "GfxFormat.h"


// Namespace Case_Engine
namespace Case_Engine
{
	struct GfxBufferDesc
	{
		uint64_t size = 0;
		GfxResourceUsage resource_usage = GfxResourceUsage::Default;
		GfxCpuAccess cpu_access = GfxCpuAccess::None;
		GfxBindFlag bind_flags = GfxBindFlag::None;
		GfxBufferMiscFlag misc_flags = GfxBufferMiscFlag::None;
		uint32_t stride = 0;
		GfxFormat format = GfxFormat::UNKNOWN;
		std::strong_ordering operator<=>(GfxBufferDesc const& other) const = default;
	};

	static GfxBufferDesc VertexBufferDesc(uint64_t vertex_count, uint32_t stride)
	{
		GfxBufferDesc desc{};
		desc.bind_flags = GfxBindFlag::VertexBuffer;
		desc.cpu_access = GfxCpuAccess::None;
		desc.resource_usage = GfxResourceUsage::Immutable;
		desc.size = vertex_count * stride;
		desc.stride = stride;
		desc.misc_flags = GfxBufferMiscFlag::None;
		return desc;
	}
	static GfxBufferDesc IndexBufferDesc(uint64_t index_count, bool small_indices)
	{
		GfxBufferDesc desc{};
		desc.bind_flags = GfxBindFlag::IndexBuffer;
		desc.cpu_access = GfxCpuAccess::None;
		desc.resource_usage = GfxResourceUsage::Immutable;
		desc.stride = small_indices ? 2 : 4;
		desc.size = index_count * desc.stride;
		desc.misc_flags = GfxBufferMiscFlag::None;
		desc.format = small_indices ? GfxFormat::R16_UINT : GfxFormat::R32_UINT;
		return desc;
	}

	template<typename T>
	static GfxBufferDesc StructuredBufferDesc(uint64_t count, bool uav = true, bool dynamic = false)
	{
		GfxBufferDesc desc{};
		desc.resource_usage = (uav || !dynamic) ? GfxResourceUsage::Default : GfxResourceUsage::Dynamic;
		desc.bind_flags = GfxBindFlag::ShaderResource;
		if (uav) desc.bind_flags |= GfxBindFlag::UnorderedAccess;
		desc.misc_flags = GfxBufferMiscFlag::BufferStructured;
		desc.cpu_access = !dynamic ? GfxCpuAccess::None : GfxCpuAccess::Write;
		desc.stride = sizeof(T);
		desc.size = desc.stride * count;
		return desc;
	}
	static GfxBufferDesc AppendBufferDesc(uint64_t count, uint32_t stride)
	{
		GfxBufferDesc desc{};
		desc.bind_flags = GfxBindFlag::UnorderedAccess;
		desc.resource_usage = GfxResourceUsage::Default;
		desc.misc_flags = GfxBufferMiscFlag::BufferStructured;
		desc.stride = stride;
		desc.size = count * stride;
		return desc;
	}
	static GfxBufferDesc IndirectArgsBufferDesc(uint64_t size)
	{
		GfxBufferDesc desc{};
		desc.bind_flags = GfxBindFlag::UnorderedAccess;
		desc.resource_usage = GfxResourceUsage::Default;
		desc.size = size;
		desc.resource_usage = GfxResourceUsage::Default;
		desc.misc_flags = GfxBufferMiscFlag::IndirectArgs;
		return desc;
	}

	enum FlagsUAV : uint8_t
	{
		UAV_None = 0x0,
		UAV_Raw = 0x1,
		UAV_Append = 0x2,
		UAV_Counter = 0x4
	};
	DEFINE_ENUM_BIT_OPERATORS(FlagsUAV);
	inline constexpr uint32_t ParseBufferUAVFlags(FlagsUAV flags)
	{
		uint32_t _flags = 0;
		if (HasAnyFlag(flags, UAV_Raw)) _flags |= D3D11_BUFFER_UAV_FLAG_RAW;
		if (HasAnyFlag(flags, UAV_Append)) _flags |= D3D11_BUFFER_UAV_FLAG_APPEND;
		if (HasAnyFlag(flags, UAV_Counter)) _flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;
		return _flags;
	}

	struct GfxBufferSubresourceDesc
	{
		uint64_t offset = 0;
		uint64_t size = uint64_t(-1);
		FlagsUAV uav_flags = UAV_None;
		std::strong_ordering operator<=>(GfxBufferSubresourceDesc const& other) const = default;
	};
	using GfxBufferInitialData = void const*;

	class GfxBuffer
	{
	public:
		GfxBuffer(GfxDevice* gfx, GfxBufferDesc const& desc, GfxBufferInitialData initial_data = nullptr)
			: gfx(gfx), desc(desc)
		{
			D3D11_BUFFER_DESC buffer_desc{};
			buffer_desc.ByteWidth = (uint32_t)desc.size;
			buffer_desc.Usage = ConvertUsage(desc.resource_usage);
			buffer_desc.BindFlags = ParseBindFlags(desc.bind_flags);
			buffer_desc.CPUAccessFlags = ParseCPUAccessFlags(desc.cpu_access);
			buffer_desc.MiscFlags = ParseMiscFlags(desc.misc_flags);
			buffer_desc.StructureByteStride = desc.stride;

			D3D11_SUBRESOURCE_DATA data{};
			if (initial_data != nullptr)
			{
				data.pSysMem = initial_data;
				data.SysMemPitch = (uint32_t)desc.size;
				data.SysMemSlicePitch = 0;
			}
			ID3D11Device* device = gfx->GetDevice();
			HRESULT hr = device->CreateBuffer(&buffer_desc, initial_data == nullptr ? nullptr : &data, resource.ReleaseAndGetAddressOf());
			GFX_CHECK_HR(hr);
		}

		GfxBuffer(GfxBuffer const&) = delete;
		GfxBuffer& operator=(GfxBuffer const&) = delete;
		~GfxBuffer() = default;

		GfxShaderResourceRO SRV(uint64_t i = 0) const { return srvs[i].Get(); }
		GfxShaderResourceRW UAV(uint64_t i = 0) const { return uavs[i].Get(); }

		[[maybe_unused]] uint64_t CreateSRV(GfxBufferSubresourceDesc const* desc = nullptr)
		{
			GfxBufferSubresourceDesc _desc = desc ? *desc : GfxBufferSubresourceDesc{};
			return CreateDescriptor(GfxSubresourceType_SRV, _desc);
		}
		[[maybe_unused]] uint64_t CreateUAV(GfxBufferSubresourceDesc const* desc = nullptr)
		{
			GfxBufferSubresourceDesc _desc = desc ? *desc : GfxBufferSubresourceDesc{};
			return CreateDescriptor(GfxSubresourceType_UAV, _desc);
		}

		ID3D11Buffer* GetNative() const { return resource.Get(); }
		ID3D11Buffer* Detach()
		{
			return resource.Detach();
		}

		GfxBufferDesc const& GetDesc() const { return desc; }
		uint32_t GetCount() const
		{
			CASE_ENGINE_ASSERT(desc.stride != 0);
			return static_cast<uint32_t>(desc.size / desc.stride);
		}

		[[maybe_unused]] void* Map()
		{
			ID3D11DeviceContext* ctx = gfx->GetContext();
			if (desc.resource_usage == GfxResourceUsage::Dynamic && desc.cpu_access == GfxCpuAccess::Write)
			{
				D3D11_MAPPED_SUBRESOURCE mapped_buffer{};
				HRESULT hr = ctx->Map(resource.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_buffer);
				GFX_CHECK_HR(hr);
				return mapped_buffer.pData;
			}
			CASE_ENGINE_ASSERT(false);
			return nullptr;
		}
		[[maybe_unused]] void* MapForRead()
		{
			ID3D11DeviceContext* ctx = gfx->GetContext();
			if (desc.cpu_access == GfxCpuAccess::Read)
			{
				D3D11_MAPPED_SUBRESOURCE mapped_buffer{};
				HRESULT hr = ctx->Map(resource.Get(), 0u, D3D11_MAP_READ, 0u, &mapped_buffer);
				GFX_CHECK_HR(hr);
				return mapped_buffer.pData;
			}
			CASE_ENGINE_ASSERT(false);
			return nullptr;
		}
		void Unmap()
		{
			ID3D11DeviceContext* ctx = gfx->GetContext();
			ctx->Unmap(resource.Get(), 0);
		}
		void Update(void const* src_data, uint64_t data_size)
		{
			ID3D11DeviceContext* ctx = gfx->GetContext();
			if (desc.resource_usage == GfxResourceUsage::Dynamic)
			{
				D3D11_MAPPED_SUBRESOURCE mapped_buffer{};
				ZeroMemory(&mapped_buffer, sizeof(D3D11_MAPPED_SUBRESOURCE));
				HRESULT hr = ctx->Map(resource.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_buffer);
				GFX_CHECK_HR(hr);
				memcpy(mapped_buffer.pData, src_data, data_size);
				ctx->Unmap(resource.Get(), 0);
			}
			else ctx->UpdateSubresource(resource.Get(), 0, nullptr, src_data, 0, 0);
		}
		template<typename T>
		void Update(T const& src_data)
		{
			Update(&src_data, sizeof(T));
		}

	private:
		GfxDevice* gfx;
		GfxBufferDesc desc;
		ArcPtr<ID3D11Buffer> resource;
		std::vector<GfxArcShaderResourceRO> srvs;
		std::vector<GfxArcShaderResourceRW> uavs;

	private:
		uint64_t CreateDescriptor(GfxSubresourceType type, GfxBufferSubresourceDesc const& subresource_desc)
		{
			HRESULT hr = E_FAIL;
			ID3D11Device* device = gfx->GetDevice();
			switch (type)
			{
			case GfxSubresourceType_SRV:
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
				if (HasAnyFlag(desc.misc_flags, GfxBufferMiscFlag::BufferRaw))
				{
					// This is a Raw Buffer
					srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
					srv_desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
					srv_desc.BufferEx.FirstElement = (UINT)subresource_desc.offset / sizeof(uint32_t);
					srv_desc.BufferEx.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / sizeof(uint32_t);
				}
				else if (HasAnyFlag(desc.misc_flags, GfxBufferMiscFlag::BufferStructured))
				{
					// This is a Structured Buffer
					srv_desc.Format = DXGI_FORMAT_UNKNOWN;
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
					srv_desc.BufferEx.FirstElement = (UINT)subresource_desc.offset / desc.stride;
					srv_desc.BufferEx.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / desc.stride;
				}
				else
				{
					// This is a Typed Buffer
					uint32_t stride = GetGfxFormatStride(desc.format);
					srv_desc.Format = ConvertGfxFormat(desc.format);
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
					srv_desc.Buffer.FirstElement = (UINT)subresource_desc.offset / stride;
					srv_desc.Buffer.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / stride;
				}

				ArcPtr<ID3D11ShaderResourceView> srv;
				hr = device->CreateShaderResourceView(resource.Get(), &srv_desc, srv.GetAddressOf());

				if (SUCCEEDED(hr))
				{
					srvs.push_back(srv);
					return srvs.size() - 1;
				}
				else CASE_ENGINE_ASSERT(false && "Failed SRV Creation");
			}
			break;
			case GfxSubresourceType_UAV:
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
				uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uav_desc.Buffer.Flags = ParseBufferUAVFlags(subresource_desc.uav_flags);
				if (HasAnyFlag(desc.misc_flags, GfxBufferMiscFlag::BufferRaw))
				{
					// This is a Raw Buffer
					uav_desc.Format = DXGI_FORMAT_R32_TYPELESS; 
					uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
					uav_desc.Buffer.FirstElement = (UINT)subresource_desc.offset / sizeof(uint32_t);
					uav_desc.Buffer.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / sizeof(uint32_t);
				}
				else if (HasAnyFlag(desc.misc_flags, GfxBufferMiscFlag::BufferStructured))
				{
					// This is a Structured Buffer
					uav_desc.Format = DXGI_FORMAT_UNKNOWN;      
					uav_desc.Buffer.FirstElement = (UINT)subresource_desc.offset / desc.stride;
					uav_desc.Buffer.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / desc.stride;
				}
				else if (HasAnyFlag(desc.misc_flags, GfxBufferMiscFlag::IndirectArgs))
				{
					// This is a Indirect Args Buffer
					uav_desc.Format = DXGI_FORMAT_R32_UINT;
					uav_desc.Buffer.FirstElement = (UINT)subresource_desc.offset / sizeof(uint32_t);
					uav_desc.Buffer.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / sizeof(uint32_t);
				}
				else
				{
					// This is a Typed Buffer
					uint32_t stride = GetGfxFormatStride(desc.format);
					uav_desc.Format = ConvertGfxFormat(desc.format);
					uav_desc.Buffer.FirstElement = (UINT)subresource_desc.offset / stride;
					uav_desc.Buffer.NumElements = (UINT)std::min(subresource_desc.size, desc.size - subresource_desc.offset) / stride;
				}

				ArcPtr<ID3D11UnorderedAccessView> uav;
				hr = device->CreateUnorderedAccessView(resource.Get(), &uav_desc, uav.GetAddressOf());

				if (SUCCEEDED(hr))
				{
					uavs.push_back(uav);
					return uavs.size() - 1;
				}
				else CASE_ENGINE_ASSERT(false && "Failed UAV Creation");
			}
			break;
			default:
				break;
			}
			return -1;
		}
	};

	static void BindIndexBuffer(ID3D11DeviceContext* ctx,GfxBuffer* ib, uint32_t offset = 0)
	{
		ctx->IASetIndexBuffer(ib->GetNative(), ConvertGfxFormat(ib->GetDesc().format), offset);
	}
	static void BindVertexBuffer(ID3D11DeviceContext* ctx, GfxBuffer* vb, uint32_t slot = 0, uint32_t offset = 0)
	{
		ID3D11Buffer* const vbs[] = { vb->GetNative() };
		uint32_t strides[] = { vb->GetDesc().stride };
		ctx->IASetVertexBuffers(slot, 1u, vbs, strides, &offset);
	}
	static void BindNullVertexBuffer(ID3D11DeviceContext* ctx)
	{
		static ID3D11Buffer* vb = nullptr;
		static const UINT stride = 0;
		static const UINT offset = 0;
		ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	}
}