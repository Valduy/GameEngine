#pragma once

#include <iostream>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include "Window.h"

namespace engine {

class Renderer {
public:
	ID3D11Device* GetDevice() const {
		return device_.Get();
	}

	ID3D11DeviceContext* GetContext() const {
		return context_.Get();
	}

	Renderer(HINSTANCE instance, LPCWSTR window_name, unsigned int width, unsigned int height)
		: window_(instance, window_name, width, height)
		, swap_chain_(nullptr)
		, device_(nullptr)
		, context_(nullptr)
		, back_texture_(nullptr)
		, render_target_view_(nullptr)
		, raster_state_(nullptr)
	{}

	HRESULT Init() {
		HRESULT result;
		window_.Show();

		if(result = CreateDeviceAndSwapChain(); FAILED(result)) {
			return result;
		}
		if (result = GetBackTexture(); FAILED(result)) {
			return result;
		}
		if (result = CreateRenderTargetView(); FAILED(result)) {
			return result;
		}
		if (result = CreateRasterState(); FAILED(result)) {
			return result;
		}

		return result;
	}

	void BeginRender() {
		context_->ClearState();
		context_->RSSetState(raster_state_.Get());

		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(window_.GetWidth());
		viewport.Height = static_cast<float>(window_.GetHeight());
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		context_->RSSetViewports(1, &viewport);
		context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), nullptr);
	}

	void EndRender() {
		context_->OMSetRenderTargets(0, nullptr, nullptr);
		swap_chain_->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
	}

private:
	Window window_;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain_;
	Microsoft::WRL::ComPtr<ID3D11Device> device_;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_texture_;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> raster_state_;

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
		ID3D11Texture2D* ptr = nullptr;
		HRESULT result = swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, reinterpret_cast<void**>(&ptr));

		if (FAILED(result)) {
			return result;
		}

		back_texture_ = ptr;
		return result;
	}

	HRESULT CreateRenderTargetView() {
		return device_->CreateRenderTargetView(back_texture_.Get(), nullptr, &render_target_view_);
	}

	HRESULT CreateRasterState() {
		CD3D11_RASTERIZER_DESC raster_desc = {};
		raster_desc.CullMode = D3D11_CULL_NONE;
		raster_desc.FillMode = D3D11_FILL_SOLID;

		return device_->CreateRasterizerState(&raster_desc, &raster_state_);
	}
};

} // namespace engine