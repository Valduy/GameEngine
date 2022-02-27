#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "Renderer.h"

namespace engine {
	class Renderer;

	class IndexBuffer {
public:
	IndexBuffer(Renderer& renderer, int* indices, size_t size)
		: renderer_(renderer)
		, indices_(indices)
		, size_(size)
		, index_buffer_(nullptr)
	{}

	HRESULT Init() {
		D3D11_BUFFER_DESC index_buf_desc = {};
		index_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buf_desc.CPUAccessFlags = 0;
		index_buf_desc.MiscFlags = 0;
		index_buf_desc.StructureByteStride = 0;
		index_buf_desc.ByteWidth = sizeof(int) * size_;

		D3D11_SUBRESOURCE_DATA index_data = {};
		index_data.pSysMem = indices_;
		index_data.SysMemPitch = 0;
		index_data.SysMemSlicePitch = 0;

		return renderer_.GetDevice()->CreateBuffer(&index_buf_desc, &index_data, &index_buffer_);
	}

	void SetBuffer() {
		renderer_.GetContext()->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

private:
	Renderer& renderer_;
	int* indices_;
	size_t size_;

	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer_;
};

} // namespace engine