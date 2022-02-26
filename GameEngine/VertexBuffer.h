#pragma once

#include <d3d11.h>
#include <directxmath.h>

namespace engine {

class VertexBuffer {
public:
	size_t GetSize() const {
		return size_;
	}

	UINT GetStribes() const {
		return sizeof(DirectX::XMFLOAT4) * size_;
	}

	VertexBuffer(ID3D11Device& device, DirectX::XMFLOAT4* points, size_t size)
		: device_(device)
		, points_(points)
		, size_(size)
		, vertex_buffer_(nullptr)
	{}

	HRESULT Initialize() {
		D3D11_BUFFER_DESC vertex_buf_desc = {};
		vertex_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		vertex_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buf_desc.CPUAccessFlags = 0;
		vertex_buf_desc.MiscFlags = 0;
		vertex_buf_desc.StructureByteStride = 0;
		vertex_buf_desc.ByteWidth = sizeof(DirectX::XMFLOAT4) * size_;

		D3D11_SUBRESOURCE_DATA vertex_data = {};
		vertex_data.pSysMem = points_;
		vertex_data.SysMemPitch = 0;
		vertex_data.SysMemSlicePitch = 0;

		return device_.CreateBuffer(&vertex_buf_desc, &vertex_data, &vertex_buffer_);
	}

	void Release() {
		vertex_buffer_->Release();
	}

private:
	ID3D11Device& device_;
	DirectX::XMFLOAT4* points_;
	size_t size_;

	ID3D11Buffer* vertex_buffer_;
};

} // namespace engine