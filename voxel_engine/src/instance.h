#pragma once
#include "config.h"

namespace vkInit
{
	VkInstance make_instance(bool debug, const char* applicationName)
	{
		if (debug)
		{
			std::cout << "Making instance" << '\n';
		}
		
		//getting latest supported version
		uint32_t version{ 0 };
		uint32_t extCount{ 0 };
		std::vector<VkExtensionProperties> extensions(extCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data());

		if (debug)
		{
			std::cout << "Available Extensions:\n";

			for (const auto& extension : extensions)
			{
				std::cout << '\t' << extension.extensionName << '\n';
			}
		}

		vkEnumerateInstanceVersion(&version);
		if (debug)
		{
			std::cout << "Supported Vulkan version: "
				<< VK_API_VERSION_MAJOR(version)   << '.'
				<< VK_API_VERSION_MINOR(version)   << '.'
				<< VK_API_VERSION_PATCH(version)   << '.'
				<< VK_API_VERSION_VARIANT(version) << '\n';
			std::cout << "Extension count: " << extCount << '\n';
		}
		
		//using version 1.0.0
		VK_MAKE_VERSION(1, 0, 0);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = applicationName;
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "voxel_engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		uint32_t glfwExtCount{ 0 };
		const char** glfwExtentions;

		glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtCount);

		if (debug)
		{
			std::cout << "GLFW Extension count: " << glfwExtCount << '\n';
		}

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtCount;
		createInfo.ppEnabledExtensionNames = glfwExtentions;
		createInfo.enabledLayerCount = 0;

		VkInstance instance;
		VkResult result;

		result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create instance");
		}

		return nullptr;
	}
}