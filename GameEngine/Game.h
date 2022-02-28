#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Renderer.h"

namespace engine {

class Game {
public:
	class GameObject {
	public:
		Game& GetGame() {
			return game_;
		}

		GameObject(Game& game)
			: game_(game) {
		}

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Destroy() = 0;

	private:
		Game& game_;
	};

	Renderer& GetRenderer() {
		return renderer_;
	}

	Game(HINSTANCE instance, LPCWSTR window_name, unsigned int width, unsigned int height)
		: is_initialized_(false)
		, renderer_(instance, window_name, width, height)
	{}

	template<typename TGameObject>
	TGameObject& CreateGameObject() {
		// TODO: init
		TGameObject* go = new TGameObject(*this);
		game_objects_.push_back(go);
		return *go;
	}

	void RemoveGameObject(GameObject* go) {
		// TODO: destroy
		auto it = std::find(game_objects_.begin(), game_objects_.end(), go);
		game_objects_.erase(it);
	}

	void Init() {
		if (HRESULT result = renderer_.Init(); FAILED(result)) {
			std::cout << "Renderer initialization failed with code: " << result << "." << std::endl;
		}

		for (GameObject* go : game_objects_) {
			go->Init();
		}

		is_initialized_ = true;
	}

	void Update() {
		assert(is_initialized_ && "Game is not initialized.\n");

		for (GameObject* go : game_objects_) {
			go->Update();
		}
	}

	void Render() {
		assert(is_initialized_ && "Game is not initialized.\n");

		renderer_.BeginRender();

		for (GameObject* go : game_objects_) {
			go->Render();
		}

		renderer_.EndRender();
	}

	void Destroy() {
		for (GameObject* go : game_objects_) {
			go->Destroy();
		}

		is_initialized_ = false;
	}

private:
	bool is_initialized_;
	Renderer renderer_;
	std::vector<GameObject*> game_objects_;
};

} // namespace engine