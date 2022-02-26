#include "Window.h"

int main() {
	engine::Window window(800, 800);

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
	}

	return 0;
}