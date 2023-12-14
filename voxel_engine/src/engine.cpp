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
	VkSurfaceKHR c_surface;
	if (glfwCreateWindowSurface(instance, window, nullptr, &c_surface) != VK_SUCCESS)
	{
		if (debugMode)
		{
			std::cout << "Failed to abstract GLFW surface for Vulkan\n";
		}
	}
	else if (debugMode)
	{
		std::cout << "Successfully abstracted GLFW surface for Vulkan\n";
	}
	surface = c_surface;
}

void Engine::make_device()
{
	physicalDevice = vkInit::choose_physical_device(instance, debugMode);
	device = vkInit::create_logical_device(physicalDevice, surface, debugMode);
	std::array<vk::Queue, 2> queues = vkInit::get_queues(physicalDevice, device, surface, debugMode);
	graphicsQueue = queues[0];
	presentQueue = queues[1];
	vkInit::query_swapchain_support(physicalDevice, surface, debugMode);
}

Engine::~Engine()
{
	if (debugMode) {
		std::cout << "Destroying Engine\n";
	}

	//destroy window
	glfwDestroyWindow(window);

	//destroy device
	device.destroy();

	//destroy surface
	instance.destroySurfaceKHR(surface);

	//destroy messenger
	if (debugMode)
	{
		instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
	}

	//destroy instance
	instance.destroy();

	//terminate glfw
	glfwTerminate();
}