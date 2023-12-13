#pragma once
#include "config.h"

namespace vkInit
{
	bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug)
	{
		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

		if (debug)
		{
			std::cout << "Supported extensions:\n";
			for (vk::ExtensionProperties extension : supportedExtensions)
			{
				std::cout << '\t' << extension.extensionName << '\n';
			}
		}

		//check extension support
		bool found;
		for (const char* extension : extensions)
		{
			found = false;
			for (vk::ExtensionProperties supportedExtension : supportedExtensions)
			{
				if (strcmp(supportedExtension.extensionName, extension) == 0)
				{
					found = true;
					if (debug)
					{
						std::cout << "Extension \"" << extension << "\" is supported\n";
					}
				}
			}
			if (!found)
			{
				if (debug)
				{
					std::cout << "Extension \"" << extension << "\" is not supported\n";
				}
				return false;
			}
		}

		std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

		if (debug)
		{
			std::cout << "Supported layers:\n";
			for (vk::LayerProperties layer : supportedLayers)
			{
				std::cout << '\t' << layer.layerName << '\n';
			}
		}

		//check layer support
		for (const char* layer : layers)
		{
			found = false;
			for (vk::LayerProperties supportedLayer : supportedLayers)
			{
				if (strcmp(supportedLayer.layerName, layer) == 0)
				{
					found = true;
					if (debug)
					{
						std::cout << "Layer \"" << layer << "\" is supported\n";
					}
				}
			}
			if (!found)
			{
				if (debug)
				{
					std::cout << "Layer \"" << layer << "\" is not supported\n";
				}
				return false;
			}
		}

		return true;
	}

	vk::Instance make_instance(bool debug, const char* applicationName)
	{
		if (debug)
		{
			std::cout << "Making instance" << '\n';
		}
		
		//getting latest supported version
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
			extensions.push_back("VK_EXT_debug_utils");
		}

		if (debug)
		{
			std::cout << "Extensions to be requested:\n";

			for (const char* ext : extensions)
			{
				std::cout << '\t' << ext << '\n';
			}
		}

		std::vector<const char*> layers;
		if (debug)
		{
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}

		if (!supported(extensions, layers, debug))
		{
			return nullptr;
		}

		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			static_cast<uint32_t>(layers.size()), layers.data(), //enabled layers
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