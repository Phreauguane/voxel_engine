#pragma once
#include <GLFW/glfw3.h>
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

	//vulkan instance
	VkInstance instance{ nullptr };

	//init engine
	void init_engine();

	//glfw setup
	void build_glfw_window();

	//main loop
	void main_loop();

	//create instance
	void make_instance();
};