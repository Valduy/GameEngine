#include <directxmath.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

int main() {
	engine::Renderer renderer;
	auto res = renderer.Init();

	engine::Shader shader(renderer, L"../Shaders/MyVeryFirstShader.hlsl");
	res = shader.Init();

	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	engine::VertexBuffer vb(renderer, points, 8);
	res = vb.Init();

	int indices[] = {
		0,1,2,
		1,0,3
	};

	engine::IndexBuffer ib(renderer, indices, 6);
	res = ib.Init();

	MSG msg = {};
	bool is_exit_requested = false;

	while(!is_exit_requested) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) {
			is_exit_requested = true;
		}

		renderer.BeginRender();

		ib.SetBuffer();
		vb.SetBuffer();
		shader.SetShader();

		renderer.GetContext()->DrawIndexed(6, 0, 0);
		renderer.EndRender();
	}

	return 0;
}