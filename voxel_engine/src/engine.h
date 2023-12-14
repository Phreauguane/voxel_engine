#pragma once
#include "config.h"

class Engine {

public:

	Engine();

	~Engine();

private:

	//whether to print debug messages in functions
	bool debugMode = true;

	//glfw window parameters
	int width{ 640 };
	int height{ 480 };
	GLFWwindow* window{ nullptr };

	//vulkan instance variables
	vk::Instance instance{ nullptr };
	vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
	vk::DispatchLoaderDynamic dldi;

	//vulkan device variables
	vk::PhysicalDevice physicalDevice{ nullptr };
	vk::Device device{ nullptr };
	vk::Queue graphicsQueue{ nullptr };

	//glfw setup
	void build_glfw_window();

	//create instance
	void make_instance();

	//device setup
	void make_device();
};