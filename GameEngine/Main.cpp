#include <directxmath.h>
#include <array>

#include "Game.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class TriangleGamObject : public engine::Game::GameObject {
public:
	std::array<DirectX::XMFLOAT4, 6> points = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};

	TriangleGamObject(engine::Game& game)
		: GameObject(game)
		, vertex_buffer_(game.GetRenderer(), points.data(), 6)
		, index_buffer_(game.GetRenderer(), indices_, 3)
		, shader_(game.GetRenderer(), L"../Shaders/MyVeryFirstShader.hlsl")
	{}

	void Init() override {
		vertex_buffer_.Init();
		index_buffer_.Init();
		shader_.Init();
	}

	void Update() override {
		
	}

	void Render() override {
		vertex_buffer_.SetBuffer();
		index_buffer_.SetBuffer();
		shader_.SetShader();

		GetGame().GetRenderer().GetContext()->DrawIndexed(3, 0, 0);
	}

 	void Destroy() override {
		
	}

private:
	int indices_[3] = {
		0,1,2,
	};

	std::array<DirectX::XMFLOAT4, 6> points_ = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
	};

	engine::VertexBuffer vertex_buffer_;
	engine::IndexBuffer index_buffer_;
	engine::Shader shader_;
};

int main() {
	engine::Game game(GetModuleHandle(nullptr), L"Engine", 800, 800);
	TriangleGamObject& triangle_go1 = game.CreateGameObject<TriangleGamObject>();
	TriangleGamObject& triangle_go2 = game.CreateGameObject<TriangleGamObject>();
	triangle_go2.points = {
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	game.Init();

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

		game.Render();
	}

	game.Destroy();
	return 0;
}