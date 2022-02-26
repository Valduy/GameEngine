#pragma once

#include <iostream>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include "Window.h"

namespace engine {

class Renderer {
public:
	Renderer()
		: window_(800, 800)
		, swap_chain_(nullptr)
		, device_(nullptr)
		, context_(nullptr)
		, back_texture_(nullptr)
		, render_target_view_(nullptr)
	{}

	HRESULT Initialize() {
		HRESULT result;
		
		if(result = CreateDeviceAndSwapChain(); FAILED(result)) {
			return result;
		}
		if (result = GetBackTexture(); FAILED(result)) {
			swap_chain_->Release();
			device_->Release();
			context_->Release();
			return result;
		}
		if (result = CreateRenderTargetView(); FAILED(result)) {
			swap_chain_->Release();
			device_->Release();
			context_->Release();
			back_texture_->Release();
			return result;
		}

		return result;
	}

	void Release() {
		swap_chain_->Release();
		device_->Release();
		context_->Release();		
		back_texture_->Release();
		render_target_view_->Release();
	}

	void Run() {
		
	}

private:
	Window window_;
	IDXGISwapChain* swap_chain_;
	ID3D11Device* device_;
	ID3D11DeviceContext* context_;	
	ID3D11Texture2D* back_texture_;
	ID3D11RenderTargetView* render_target_view_;	

	HRESULT CreateDeviceAndSwapChain() {
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.BufferDesc.Width = window_.GetWidth();
		swap_chain_desc.BufferDesc.Height = window_.GetHeight();
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = window_.GetHandler();
		swap_chain_desc.Windowed = true;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;

		D3D_FEATURE_LEVEL feature_level[] = { D3D_FEATURE_LEVEL_11_1 };

		return D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			feature_level,
			1,
			D3D11_SDK_VERSION,
			&swap_chain_desc,
			&swap_chain_,
			&device_,
			nullptr,
			&context_);
	}

	HRESULT GetBackTexture() {
		return swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, reinterpret_cast<void**>(&back_texture_));
	}

	HRESULT CreateRenderTargetView() {
		return device_->CreateRenderTargetView(back_texture_, nullptr, &render_target_view_);
	}
};

} // namespace engine