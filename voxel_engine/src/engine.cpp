#include "engine.h"
#include "instance.h"

Engine::Engine()
{

	if (debugMode) {
		std::cout << "Creating Engine\n";
	}
	
	build_glfw_window();

	make_instance();
}

void Engine::build_glfw_window()
{

	//initialize glfw
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (window = glfwCreateWindow(width, height, "Voxel Engine", nullptr, nullptr)) {
		if (debugMode) {
			std::cout << "Window Created\n";
		}
	}
	else {
		if (debugMode) {
			std::cout << "Failed to create window\n";
		}
	}
}

void Engine::make_instance()
{
	instance = vkInit::make_instance(debugMode, "Voxel Engine");
}

Engine::~Engine()
{
	//destroy instance
	vkDestroyInstance(instance, nullptr);

	//destroy window
	glfwDestroyWindow(window);
	if (debugMode) {
		std::cout << "Destroying Engine\n";
	}

	//terminate glfw
	glfwTerminate();
}