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

	//no default rendering client, we'll hook vulkan up
	//to the window later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//resizing breaks the swapchain, we'll disable it for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
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

	if (debugMode) {
		std::cout << "Destroying Engine\n";
	}

	//terminate glfw
	glfwTerminate();
}