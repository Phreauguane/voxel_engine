#pragma once
#include "config.h"

namespace vkInit
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	void log_device_properties(vk::PhysicalDevice& device)
	{
		vk::PhysicalDeviceProperties props = device.getProperties();

		std::cout << "Name: " << props.deviceName << '\n';

		std::cout << "Type: ";
		switch (props.deviceType)
		{
		case (vk::PhysicalDeviceType::eCpu):
			std::cout << "CPU\n"; break;
		case (vk::PhysicalDeviceType::eDiscreteGpu):
			std::cout << "Discrete GPU\n"; break;
		case (vk::PhysicalDeviceType::eIntegratedGpu):
			std::cout << "Integrated GPU\n"; break;
		case (vk::PhysicalDeviceType::eVirtualGpu):
			std::cout << "Virtual GPU\n"; break;
		default:
			std::cout << "Unknown\n"; break;
		}
	}

	bool checkDeviceExtensionSupport(
		const vk::PhysicalDevice& device, 
		const std::vector<const char*>& requestedExtensions, 
		const bool& debug)
	{
		std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());

		if (debug)
		{
			std::cout << "Device can support extensions:\n";
		}

		for (vk::ExtensionProperties& ext : device.enumerateDeviceExtensionProperties())
		{
			if (debug)
			{
				std::cout << "\t\"" << ext.extensionName << "\"\n";
			}

			requiredExtensions.erase(ext.extensionName);
		}

		return requiredExtensions.empty();
	}

	bool isSuitable(vk::PhysicalDevice& device, bool debug)
	{
		if (debug)
		{
			std::cout << "Checking if device is suitable\n";
		}

		const std::vector<const char*> requestedExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		if (debug)
		{
			std::cout << "Requested device extensions:\n";

			for (const char* ext : requestedExtensions)
			{
				std::cout << "\t\"" << ext << "\"\n";
			}
		}

		if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug))
		{
			if (debug)
			{
				std::cout << "Device can support the requested extensions\n";
			}
		}
		else
		{
			if (debug)
			{
				std::cout << "Device can't support the requested extensions\n";
			}
			return false;
		}
		return true;
	}

	vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debug)
	{
		if (debug)
		{
			std::cout << "Choosing physical device...\n";
		}
		
		std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();

		if (debug)
		{
			std::cout << "There are " << availableDevices.size() << " available devices\n";
		}

		for (vk::PhysicalDevice device : availableDevices)
		{
			if (debug)
			{
				log_device_properties(device);
			}
			if (isSuitable(device, debug))
			{
				return device;
			}
		}

		return  nullptr;
	}

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice& device, bool debug)
	{
		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

		if (debug)
		{
			std::cout << "System can support " << queueFamilies.size() << " queue families\n";
		}

		for (int i = 0; i < queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
				indices.presentFamily = i;

				if (debug)
				{
					std::cout << "Queue family " << i << " is suitable\n";
				}
			}

			if (indices.isComplete())
			{
				break;
			}
		}

		return indices;
	}

	vk::Device create_logical_device(vk::PhysicalDevice& physicalDevice, bool debug)
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, debug);
		float queuePriority{ 1.0f };

		vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo(
			vk::DeviceQueueCreateFlags(),
			indices.graphicsFamily.value(),
			1, &queuePriority
		);

		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

		std::vector<const char*> enabledLayers;

		if (debug)
		{
			enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
		}

		vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
			vk::DeviceCreateFlags(), 
			1, &queueCreateInfo, 
			enabledLayers.size(), enabledLayers.data(),
			0, nullptr,
			&deviceFeatures
		);

		try
		{
			vk::Device device = physicalDevice.createDevice(deviceInfo);
			if (debug)
			{
				std::cout << "GPU Successfully abstracted\n";
			}
			return device;
		}
		catch (vk::SystemError err)
		{
			if (debug)
			{
				std::cout << "Device creation failed\n";
			}
			return nullptr;
		}
	}

	vk::Queue get_queue(vk::PhysicalDevice physicalDevice, vk::Device device, bool debug)
	{
		QueueFamilyIndices indices = findQueueFamilies(physicalDevice, debug);

		return device.getQueue(indices.graphicsFamily.value(), 0);
	}
}