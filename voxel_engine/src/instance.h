#pragma once
#include "config.h"

namespace vkInit
{
	vk::Instance make_instance(bool debug, const char* applicationName)
	{
		if (debug)
		{
			std::cout << "Making instance" << '\n';
		}
		uint32_t version{ 0 };
		vkEnumerateInstanceVersion(&version);
		if (debug)
		{
			std::cout << "Supported Vulkan version: "
				<< VK_API_VERSION_MAJOR(version)   << '.'
				<< VK_API_VERSION_MINOR(version)   << '.'
				<< VK_API_VERSION_PATCH(version)   << '.'
				<< VK_API_VERSION_VARIANT(version) << '\n';
		}
		return nullptr;
	}
}