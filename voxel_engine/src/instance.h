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

		vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
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
		version = VK_MAKE_VERSION(1, 0, 0);

		vk::ApplicationInfo appInfo = vk::ApplicationInfo(
			applicationName,
			version,
			"voxel engine",
			version,
			version
		);

		uint32_t glfwExtCount{ 0 };
		const char** glfwExtentions;

		glfwExtentions = glfwGetRequiredInstanceExtensions(&glfwExtCount);

		if (debug)
		{
			std::cout << "GLFW Extension count: " << glfwExtCount << '\n';
		}

		std::vector<const char*> extensions(glfwExtentions, glfwExtentions + glfwExtCount);

		if (debug)
		{
			std::cout << "Extensions to be requested:\n";

			for (const char* ext : extensions)
			{
				std::cout << '\t' << ext << '\n';
			}
		}

		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			0, nullptr, //enabled layers
			static_cast<uint32_t>(extensions.size()), extensions.data() //enabled extensions
		);

		try
		{
			return vk::createInstance(createInfo, nullptr);
		}
		catch (vk::SystemError err)
		{
			if (debug)
			{
				std::cout << "Failed to create Instance!\n";
			}
			return nullptr;
		}
	}
}