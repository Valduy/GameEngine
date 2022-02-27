#pragma once

#include <d3d11.h>
#include <iostream>
#include <d3dcompiler.h>

namespace engine {

class Shader {
public:
	Shader(Renderer& renderer, LPCWSTR path)
		: renderer_(renderer)
		, path_(path)
		, vertex_byte_code_(nullptr)
		, pixel_byte_code_(nullptr)
		, vertex_shader_(nullptr)
		, pixel_shader_(nullptr)
		, layout_(nullptr) 
	{}

	HRESULT Init() {
		HRESULT result;

		if (result = CompileVertexByteCode(); FAILED(result)) {
			return result;
		}
		if (result = CompilePixelByteCode(); FAILED(result)) {
			vertex_byte_code_->Release();
			return result;
		}
		if (result = CreateVertexShader(); FAILED(result)) {
			vertex_byte_code_->Release();
			pixel_byte_code_->Release();
			return result;
		}
		if (result = CreatePixelShader(); FAILED(result)) {
			vertex_byte_code_->Release();
			pixel_byte_code_->Release();
			vertex_shader_->Release();
			return result;
		}
		if (result = CreateInputLayout(); FAILED(result)) {
			vertex_byte_code_->Release();
			pixel_byte_code_->Release();
			vertex_shader_->Release();
			pixel_shader_->Release();
			return result;
		}

		return result;
	}

	void Release() {
		vertex_byte_code_->Release();
		pixel_byte_code_->Release();
		vertex_shader_->Release();
		pixel_shader_->Release();
		layout_->Release();
	}

	void SetShader() {
		renderer_.GetContext()->IASetInputLayout(layout_);
		renderer_.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderer_.GetContext()->VSSetShader(vertex_shader_, nullptr, 0);
		renderer_.GetContext()->PSSetShader(pixel_shader_, nullptr, 0);
	}
	
private:
	Renderer& renderer_;
	LPCWSTR path_;

	ID3DBlob* vertex_byte_code_;
	ID3DBlob* pixel_byte_code_;
	ID3D11VertexShader* vertex_shader_;
	ID3D11PixelShader* pixel_shader_;
	ID3D11InputLayout* layout_;

	HRESULT CompileVertexByteCode() {
		return CompileByteCode(
			path_,
			"VSMain",
			"vs_5_0",
			&vertex_byte_code_);
	}

	HRESULT CompilePixelByteCode() {
		return CompileByteCode(
			path_,
			"PSMain",
			"ps_5_0",
			&pixel_byte_code_);
	}

	HRESULT CompileByteCode(LPCWSTR path, LPCSTR entry_point, LPCSTR target, ID3DBlob** byte_code) {
		ID3DBlob* error_vertex_code_ = nullptr;
		HRESULT result = D3DCompileFromFile(
			path,
			nullptr,
			nullptr,
			entry_point,
			target,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			byte_code,
			&error_vertex_code_);

		if (FAILED(result)) {
			if (error_vertex_code_) {
				const char* message = static_cast<char*>(error_vertex_code_->GetBufferPointer());
				std::cout << message << std::endl;
				error_vertex_code_->Release();
			}

			if (vertex_byte_code_) {
				vertex_byte_code_->Release();
			}

			return result;
		}

		return result;
	}

	HRESULT CreateVertexShader() {
		return renderer_.GetDevice()->CreateVertexShader(
			vertex_byte_code_->GetBufferPointer(),
			vertex_byte_code_->GetBufferSize(),
			nullptr,
			&vertex_shader_);
	}

	HRESULT CreatePixelShader() {
		return renderer_.GetDevice()->CreatePixelShader(
			pixel_byte_code_->GetBufferPointer(),
			pixel_byte_code_->GetBufferSize(),
			nullptr,
			&pixel_shader_);
	}

	HRESULT CreateInputLayout() {
		D3D11_INPUT_ELEMENT_DESC input_elements[] = {
			D3D11_INPUT_ELEMENT_DESC {
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			},
			D3D11_INPUT_ELEMENT_DESC {
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			},
		};

		return renderer_.GetDevice()->CreateInputLayout(
			input_elements,
			2,
			vertex_byte_code_->GetBufferPointer(),
			vertex_byte_code_->GetBufferSize(),
			&layout_);
	}
};

} // namespace engine