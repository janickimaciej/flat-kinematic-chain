#include "gui/gui.hpp"
#include "scene.hpp"
#include "window.hpp"

int main()
{
	Window window{};
	Scene scene{window.viewportSize()};
	GUI gui{window.getPtr(), scene, window.viewportSize()};
	window.init(scene);

	while (!window.shouldClose())
	{
		gui.update();
		scene.update();
		scene.render();
		gui.render();
		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}
