//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <d3d11.h>

// Namespace Case_Engine
namespace Case_Engine
{
	using GfxRenderTarget	 = ID3D11RenderTargetView*;
	using GfxDepthTarget	 = ID3D11DepthStencilView*;
	using GfxShaderResourceRO  = ID3D11ShaderResourceView*;
	using GfxShaderResourceRW = ID3D11UnorderedAccessView*;

	using GfxArcRenderTarget		= ArcPtr<ID3D11RenderTargetView>;
	using GfxArcDepthTarget		= ArcPtr<ID3D11DepthStencilView>;
	using GfxArcShaderResourceRO	= ArcPtr<ID3D11ShaderResourceView>;
	using GfxArcShaderResourceRW = ArcPtr<ID3D11UnorderedAccessView>;
}