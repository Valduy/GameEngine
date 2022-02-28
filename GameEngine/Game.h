#pragma once

#include "Renderer.h"

namespace engine {

class Game {
public:
	void Init() {
		HRESULT result = renderer_.Init();
	}

	void Update() {
		
	}

	void Render() {
		
	}

	void Destroy() {
		
	}

private:
	Renderer renderer_;

};

} // namespace engine