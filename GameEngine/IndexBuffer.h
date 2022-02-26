#pragma once

#include <d3d11.h>

namespace engine {

class IndexBuffer {
public:
	IndexBuffer(ID3D11Device& device, int* indices, size_t size)
		: device_(device)
		, indices_(indices)
		, size_(size)
		, index_buffer_()
	{}

	void Initialize() {
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

		device_.CreateBuffer(&index_buf_desc, &index_data, &index_buffer_);
	}

	void Release() {
		index_buffer_->Release();
	}

private:
	ID3D11Device& device_;
	int* indices_;
	size_t size_;

	ID3D11Buffer* index_buffer_;
};

} // namespace engine