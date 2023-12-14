#include "engine.h"
#include "instance.h"
#include "logging.h"
#include "device.h"

Engine::Engine()
{

	if (debugMode) {
		std::cout << "Creating Engine\n";
	}
	
	build_glfw_window();

	make_instance();

	make_device();
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
	if (debugMode)
	{
		debugMessenger = vkInit::make_debug_messenger(instance, dldi);
	}
}

void Engine::make_device()
{
	physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	device = vkInit::create_logical_device(physicalDevice, debugMode);
	graphicsQueue = vkInit::get_queue(physicalDevice, device, debugMode);
}

Engine::~Engine()
{
	if (debugMode) {
		std::cout << "Destroying Engine\n";
	}

	//destroy window
	glfwDestroyWindow(window);

	//destroy messenger
	if (debugMode)
	{
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	//destroy device
	device.destroy();

	//destroy instance
	instance.destroy();

	//terminate glfw
	glfwTerminate();
}