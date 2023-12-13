#include "engine.h"
#include "instance.h"
#include "logging.h"

Engine::Engine()
{

	if (debugMode) {
		std::cout << "Creating Engine\n";
	}
	
	build_glfw_window();

	make_instance();

	make_debug_messenger();
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
	dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
}

void Engine::make_debug_messenger()
{
	debugMessenger = vkInit::make_debug_messenger(instance, dldi);
}

Engine::~Engine()
{
	if (debugMode) {
		std::cout << "Destroying Engine\n";
	}

	//destroy window
	glfwDestroyWindow(window);

	//destroy messenger
	instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);

	//destroy instance
	instance.destroy();

	//terminate glfw
	glfwTerminate();
}